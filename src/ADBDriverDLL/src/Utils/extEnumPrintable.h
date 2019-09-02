
#pragma once

/*
    MIT License

    C++17 Enumerator printable, header only

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/printable-enumerator-template

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

    Algorithm variadic arguments foreach based from
    https://github.com/swansontec/map-macro (2012) William Swanson

 */

/** \brief Example using ENUM_PRINTABLE -std=c++17 code
 *
 * class youTest {
 *   public:
 *   ENUM_PRINTABLE(EnumeratorName, int32_t,
 *       ENUM_TAG_OKAY,  // == 0
 *       ENUM_TAG_FAIL,
 *       ENUM_TAG_DONE,
 *       ENUM_TAG_SEND,
 *       ENUM_TAG_RECV,
 *       ENUM_TAG_DATA
 *   );
 * };
 *
 *   Get enum size by labels:
 *   std::size_t s = youTest::sizeEnumeratorName();
 *
 *   Get string by number:
 *   std::cout << youTest::getEnumeratorName(2);
 *
 *   Get number by enum value:
 *   int32_t n = enumtoint(EnumeratorName::ENUM_TAG_DONE);
 *
 *   Get string by enum value:
 *   std::cout << enumtostring(EnumeratorName::ENUM_TAG_DONE);
 *
 *   Get string by enum value to std::ostream:
 *   std::cout << EnumeratorName::ENUM_TAG_DONE;
 *
 */

#define ELE_(A) (sizeof(A) / sizeof(A[0]))

#define EVAL0(...) __VA_ARGS__
#define EVAL1(...) EVAL0 (EVAL0 (EVAL0 (__VA_ARGS__)))
#define EVAL2(...) EVAL1 (EVAL1 (EVAL1 (__VA_ARGS__)))
#define EVAL3(...) EVAL2 (EVAL2 (EVAL2 (__VA_ARGS__)))
#define EVAL4(...) EVAL3 (EVAL3 (EVAL3 (__VA_ARGS__)))
#define EVALX(...) EVAL4 (EVAL4 (EVAL4 (__VA_ARGS__)))

#define STR_(A) #A
#define ARG_(A) STR_(A),
#define ARG_EMPTY_
#define ARG_END_(...)
#define ARG_END() 0, ARG_END_
#define ARG_NEXT1(I, N, ...) N ARG_EMPTY_
#define ARG_NEXT2(I, N) ARG_NEXT1 (I, N, 0)
#define ARG_NEXT0(I, N) ARG_NEXT2 (ARG_END I, N)
#define ARGS_A0(A, P, ...) ARG_(A) ARG_NEXT0 (P, ARGS_A1) (P, __VA_ARGS__)
#define ARGS_A1(A, P, ...) ARG_(A) ARG_NEXT0 (P, ARGS_A0) (P, __VA_ARGS__)
#define ARGS(...) EVALX (ARGS_A1 (__VA_ARGS__, (), 0))

# define ENUM_DECLARE_(N, T, ...)                                                                             \
    enum class N : T                                                                                          \
    {                                                                                                         \
        __VA_ARGS__                                                                                           \
    };                                                                                                        \
    friend T enumtoint(N enumval)                                                                             \
    {                                                                                                         \
        return static_cast<T>(enumval);                                                                       \
    }

# define ENUM_DECLARE_PRINT_(N, T, ...)                                                                       \
    static inline const char *N##EnumString[] =                                                               \
    {                                                                                                         \
        ARGS(__VA_ARGS__)                                                                                     \
        nullptr                                                                                               \
    };                                                                                                        \
    static std::size_t  size##N()                                                                             \
    {                                                                                                         \
        return (ELE_(N##EnumString) - 1);                                                                     \
    };                                                                                                        \
    static const char * get##N(T val)                                                                         \
    {                                                                                                         \
        if constexpr (std::is_signed<T>::value)                                                               \
            if (val < 0)                                                                                      \
                return l_ErrorGetEnumString_;                                                                 \
                                                                                                              \
        if (val >= static_cast<T>(ELE_(N##EnumString) - 1))                                                   \
            return l_ErrorGetEnumString_;                                                                     \
        return N##EnumString[val];                                                                            \
    }                                                                                                         \
    friend const char * enumtostring(N enumval)                                                               \
    {                                                                                                         \
        return N##EnumString[static_cast<T>(enumval)];                                                        \
    }                                                                                                         \
    friend std::ostream &operator << (std::ostream & os, N enumval)                                           \
    {                                                                                                         \
        os << enumtostring(enumval);                                                                          \
        return os;                                                                                            \
    }                                                                                                         \

# define ENUM_DECLARE_PRINT_EMPTY_(N,T)                                                                       \
    static std::size_t  size##N()                                                                             \
    {                                                                                                         \
        return 0U;                                                                                            \
    };                                                                                                        \
    static const char * get##N(T)                                                                             \
    {                                                                                                         \
        return l_ErrorGetEnumString_;                                                                         \
    }                                                                                                         \
    friend const char * enumtostring(N)                                                                       \
    {                                                                                                         \
        return l_ErrorGetEnumString_;                                                                         \
    }                                                                                                         \
    friend std::ostream &operator << (std::ostream & os, N)                                                   \
    {                                                                                                         \
        os << l_ErrorGetEnumString_;                                                                          \
        return os;                                                                                            \
    }                                                                                                         \

#if (defined(_BUILD_DLL) && defined(_BUILD_TRACE) && (_BUILD_TRACE == 1))
   static inline const char *l_ErrorGetEnumString_ = "! out of range";
#  define ENUM_PRINTABLE(N, T, ...)                                                                           \
            ENUM_DECLARE_(N, T, __VA_ARGS__)                                                                  \
                ENUM_DECLARE_PRINT_(N, T, __VA_ARGS__)

#else
   static inline const char *l_ErrorGetEnumString_ = "! not build this feature - " __FILE__  ":" STR_(__LINE__);
#  define ENUM_PRINTABLE(N, T, ...)                                                                           \
            ENUM_DECLARE_(N, T, __VA_ARGS__)                                                                  \
                ENUM_DECLARE_PRINT_EMPTY_(N, T)

#endif
