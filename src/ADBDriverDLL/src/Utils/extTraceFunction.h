
#pragma once

/*
    MIT License

    C++17 trace function call template, one header source

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/trace-function-call-template

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sub license, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.

 */

#include <atomic>
#include <string>
#include <sstream>
#include <functional>
#include <cstdarg>
#include <errno.h>
#include <string.h> // strerror()

#if (defined(_BUILD_TRACE) && (_BUILD_TRACE == 1))

#   define tracer()                                                                                           \
        tracer_(__FILE__, __FUNCTION__, __LINE__, __PRETTY_FUNCTION__)
#   define tracer_(A,B,C,D)                                                                                   \
        TraceLogger l_## A ##_trace(A,B,C,D)
#   define trace_init(V)                                                                                      \
        TraceFunction::instance().init(V);                                                                    \
        tracer()
#   define trace_start()                                                                                      \
        std::string f(__FILE__);                                                                              \
        {                                                                                                     \
            size_t pos = f.find_last_of("/\\");                                                               \
            if (pos != std::string::npos)                                                                     \
                f = f.substr(pos + 1, (f.length() - pos - 1)).c_str();                                        \
            f.append(".tlog");                                                                                \
        }                                                                                                     \
        trace_init(f);
#   define trace_info(V,...)                                                                                  \
        TraceFunction::instance().trace(__FUNCTION__, __LINE__, V, __VA_ARGS__)
#   define trace_enter()                                                                                      \
        TraceFunction::instance().trace(__FILE__, __FUNCTION__, __LINE__, __PRETTY_FUNCTION__)
#   define trace_return(R)                                                                                    \
    {                                                                                                         \
        TraceFunction::instance().trace(__FUNCTION__, __LINE__, R);                                           \
        return R;                                                                                             \
    }
#   define trace_return_cb(R,T,F)                                                                             \
    {                                                                                                         \
        TraceFunction::instance().trace_cb<T>(__FUNCTION__, __LINE__, R, F);                                  \
        return R;                                                                                             \
    }
#   define trace_call(F,...) __extension__                                                                    \
        ({ TraceFunction::instance().args(#F, __VA_ARGS__); F(__VA_ARGS__); })
#   define trace_break(V)                                                                                     \
    {                                                                                                         \
        TraceFunction::instance().trace(                                                                      \
                        __FUNCTION__, __LINE__,                                                               \
                        "break from line: %d, reason: %d",                                                    \
                        __LINE__, static_cast<int32_t>(V)                                                     \
                );                                                                                            \
        break;                                                                                                \
    }

#else

#   define tracer()
#   define tracer_(...)
#   define trace_start(...)
#   define trace_info(...)
#   define trace_enter()
#   define trace_return(R)                                                                                    \
    {                                                                                                         \
        return R;                                                                                             \
    }
#   define trace_return_cb(R,...)                                                                             \
    {                                                                                                         \
        return R;                                                                                             \
    }
#   define trace_call(F,...)                                                                                  \
        F(__VA_ARGS__)
#   define trace_break(V) break

#endif

namespace stdext
{

class TraceFunction
{
private:
    //
    FILE *m_fp  = nullptr;
    FILE *m_ufp = nullptr;
    //
    static inline const char m_tab[145]   =
        "| | | | | | | | | | | | | | | | | | | | | | | | " \
        "| | | | | | | | | | | | | | | | | | | | | | | | " \
        "| | | | | | | | | | | | | | | | | | | | | | | | ";
    static inline const char *m_nl[2]        = { "\n", "" };
    static inline const char *sfmt_return    = "%s%*.*s- (return) :[%s|%d] -> %s\n";
    static inline const char *sfmt_enter     = "%s%*.*s- (enter)  :[%s|%d|%s] -> %s\n";
    static inline const char *sfmt_exit      = "%s%*.*s- (exit)   :[%s]\n";
    static inline const char *sfmt_info      = "%s%*.*s- (info)   :[%s|%d] -> ";
    static inline const char *sfmt_args      = "%s%*.*s- (args)   :[%s] -> %s\n";
    static inline const char *sfmt_begin     = " *-> (compiled)   :[%s %s]\n";
    static inline const char *sfmt_error     = " !-> (open file)  :[%s] -> %d, %s\n";
    static inline const char *already_error  = " !-> (open file)  :[file already open] -> %p\n";
    //
    std::atomic<int32_t> m_indent;
    std::atomic<bool>    m_newline;
    std::atomic<bool>    m_declsig;
    //
    TraceFunction& operator=(const TraceFunction&) = delete;
    TraceFunction& operator=(TraceFunction&&) = delete;
    TraceFunction()
        : m_indent(1), m_newline(false), m_declsig(true) {}
    ~TraceFunction()
    {
        if (m_fp)
            ::fclose(m_fp);
        m_fp  = nullptr;
    }

    FILE * out_select()
    {
        return ((m_fp) ? m_fp : ((m_ufp) ? m_ufp : stderr));
    }
    int32_t indent_get()
    {
        if (!m_indent.load())
            m_indent = 1;

        int32_t  indent = (m_indent.load() * 2),
                 tabsz  = static_cast<int32_t>(std::size(m_tab));
        if (tabsz <= indent)
            return (tabsz - 1);
        return indent;
    }
    void indent_up()
    {
        int32_t indent = m_indent.load();
        m_indent = ((indent > 0) ? (indent + 1) : 1);
    }
    void indent_down()
    {
        int32_t indent = m_indent.load();
        m_indent = ((indent > 1) ? (indent - 1) : 1);
    }
    const char * get_newline()
    {
        return ((m_newline.load()) ? m_nl[0] : m_nl[1]);
    }
    void startline()
    {
        ::fprintf(out_select(), sfmt_begin, __DATE__, __TIME__);
    }

public:
    //
    void init(bool n = false, bool d = true)
    {
        m_newline = n;
        m_declsig = d;
        m_indent  = 1;
        startline();
    }
    void init(FILE *f, bool n = false, bool d = true)
    {
        m_ufp     = f;
        m_newline = n;
        m_declsig = d;
        m_indent  = 1;
        startline();
    }
    void init(std::string const & s, bool n = false, bool d = true)
    {
        if (s.empty())
            return;
        if (m_fp)
        {
            ::fprintf(out_select(), already_error, m_fp);
            return;
        }
        m_newline = n;
        m_declsig = d;
        m_indent  = 1;
        m_fp = ::fopen(s.c_str(), "wt");
        if (!m_fp)
            ::fprintf(out_select(), sfmt_error, s.c_str(), errno, ::strerror(errno));
        startline();
    }
    /** \brief Enter function
     *
     * \param f file name
     * \param s function name
     * \param l file line
     * \param p function signature string
     *
     *  using from stdext::TraceLogger() constructor,
     *  or macro trace_enter(), warning, indent increase
     */
    void trace(std::string const & f, std::string const & s, int32_t l, std::string const & p)
    {
        int32_t indent = indent_get();
        size_t pos = f.find_last_of("/\\");

        ::fprintf(
                out_select(),
                sfmt_enter,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str(),
                l,
                ((pos == std::string::npos) ?
                    f.c_str() :
                    f.substr(pos + 1, (f.length() - pos - 1)).c_str()
                ),
                ((m_declsig.load()) ? p.c_str() : "")
            );
        indent_up();
    }
    /** \brief Exit function
     *
     * \param s function name
     *
     *  using from stdext::TraceLogger() destructor
     *  or macro trace_exit(), warning, indent decrease
     */
    void trace(std::string const & s)
    {
        int32_t indent = indent_get();

        ::fprintf(
                out_select(),
                sfmt_exit,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str()
            );
        indent_down();
    }
    /** \brief Return from function
     *
     * \param s   function name
     * \param l   file line
     * \param obj function return (auto type)
     *
     *  using macro trace_return(return value)
     *  return function object do have operator << ostream is print same.
     */
    template <typename T>
    void trace(std::string const & s, int32_t l, T obj)
    {
        int32_t indent = indent_get();
        std::stringstream ss;
        ss << obj;

        ::fprintf(
                out_select(),
                sfmt_return,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str(),
                l,
                ss.str().c_str()
            );
    }
    /** \brief Return from function using callback to print custom user object
     *
     * \param s   function name
     * \param l   file line
     * \param fun function ref
     *
     *  using macro trace_return_cb(auto value, std::function<std::string(auto&)>)
     *  example: trace_return_cb(value, valueType, [](valueType & v) { return to_string(v); });
     */
    template<typename T>
    void trace_cb(std::string const & s, int32_t l, T obj, std::function<std::string(T&)> fun)
    {
        int32_t indent = indent_get();
        std::string ss = fun(obj);

        ::fprintf(
                out_select(),
                sfmt_return,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str(),
                l,
                ss.c_str()
            );
    }
    /** \brief Print function arguments value
     *
     * \param s function name
     * \param l file line
     * \param fmt string arguments format
     * \param (...) arguments list
     *
     *  using macro trace_call(function name, function arguments)
     */
    template<class... Args>
    void args(std::string const & s, Args... a)
    {
        int32_t indent = indent_get();
        std::stringstream ss;
        ((ss << a << ", "), ...);

        ::fprintf(
                out_select(),
                sfmt_args,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str(),
                ss.str().c_str()
            );
    }
    /** \brief Print user defined information, like as printf()
     *
     * \param s function name
     * \param l file line
     * \param fmt string arguments format
     * \param (...) arguments list
     *
     *  using macro trace_info(format, arguments)
     */
    void trace(std::string const & s, int32_t l, const char *fmt, ...)
    {
        va_list args;
        va_start(args, fmt);
        {
            int32_t indent = indent_get();
            FILE *f = out_select();
            ::fprintf(
                f,
                sfmt_info,
                get_newline(),
                indent, (indent - 1), m_tab,
                s.c_str(),
                l
            );
            ::vfprintf(f, fmt, args);
            ::fwrite(m_nl[0], 1, 1, f);
        }
        va_end(args);
    }
    //
    static TraceFunction& instance()
    {
        static TraceFunction m_instance{};
        return m_instance;
    }
};

class TraceLogger
{
private:
    //
    std::string m_src;
    std::string m_fun;
    std::string m_pfun;
    int32_t     m_line;

public:
    //
    TraceLogger() = delete;
    TraceLogger(const TraceLogger&) = delete;
    TraceLogger(const TraceLogger&&) = delete;
    TraceLogger& operator=(const TraceLogger&) = delete;
    TraceLogger(std::string const & s, std::string const & f, int32_t l, std::string const & p)
        : m_src(s), m_fun(f), m_pfun(p), m_line(l)
    {
        TraceFunction::instance().trace(m_src, m_fun, m_line, m_pfun);
    }
    ~TraceLogger()
    {
        TraceFunction::instance().trace(m_fun);
    }
};

}

using stdext::TraceFunction;
using stdext::TraceLogger;
