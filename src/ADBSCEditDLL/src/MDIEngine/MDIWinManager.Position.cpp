/*
    MIT License

    Android remote Viewer, GUI ADB tools

    Android Viewer developed to view and control your android device from a PC.
    ADB exchange Android Viewer, support scale view, input tap from mouse,
    input swipe from keyboard, save/copy screenshot, etc..

    Copyright (c) 2016-2019 PS
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer
    GitHub: https://github.com/ClnViewer/ADB-Android-Viewer/ADBSCEditDLL/ADBSCEdit

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

#include "../SCEditInternal.h"

namespace MDIWin
{
    void Manager::moved(HWND hwnd, int32_t x_, int32_t y_)
    {
        try
        {
            auto a = find(getwinid(hwnd));
            if (!a)
                return;

            ImageLite::IPOINT<int32_t> ip = { x_, y_ };
            a->setsize(ip);
        }
        catch (...) {}
    }

    void Manager::closed(HWND hwnd)
    {
        try
        {
            do
            {
                uint32_t id = getwinid(hwnd);
                if (!id)
                    break;
                if (!remove(id))
                    break;
                //
                auto a = find(
                        static_cast<uint32_t>(BaseData::MDIWinType::MWTYPE_CHILD_EDIT)
                    );
                if (!a)
                    break;

                a->setsizedefault();
                setsize();
            }
            while (0);
        }
        catch (...) {}
    }

    void Manager::setsize()
    {
        try
        {
            ImageLite::IRECT<int32_t> irm{};
            ImageLite::IRECT<int32_t> irc{};
            ImageLite::IRECT<int32_t> ire{};
            //
            for (auto const & [key, val] : m_winmap)
            {
                auto a = val.get();
                if (!a)
                    continue;
                if (a->ispined())
                    continue;

                bool isinit = a->getsize().empty();

                switch (a->getgroup())
                {
                    case BaseData::MDIWinType::MWTYPE_MAIN:
                        {
                            if (isinit)
                                return;
                            //
                            a->setsize();
                            irm   = a->getsize();
                            irm.x = 0;
                            irm.y = 0;
                            break;
                        }
                    case BaseData::MDIWinType::MWTYPE_TOOLBAR:
                        {
                            if (isinit)
                            {
                                if (irm.empty())
                                    break;
                                a->setsizedefault(irm);
                            }
                            else
                                a->setsize();
                            //
                            {
                                ImageLite::IRECT<int32_t> irt = a->getsize();
                                if (irt.h > 0)
                                    irc.y += irt.h;
                            }
                            break;
                        }
                    case BaseData::MDIWinType::MWTYPE_CLIENT:
                        {
                            if (isinit)
                            {
                                if (irm.empty())
                                    break;
                                a->setsizedefault(irm);
                            }
                            else
                                a->setsize();
                            //
                            ImageLite::IRECT<int32_t> irt = a->getsize();
                            if (!irt.empty())
                            {
                                irc.w = irt.w;
                                irc.h = ((irc.y) ? (irt.h - irc.y) : irt.h);
                            }
                            break;
                        }
                    case BaseData::MDIWinType::MWTYPE_CHILD:
                    case BaseData::MDIWinType::MWTYPE_CUSTOM:
                        {
                            if (irc.empty())
                                break;

                            switch (a->gettype())
                            {
                                case BaseData::MDIWinType::MWTYPE_CHILD_EDIT:
                                    {
                                        if (isinit)
                                            a->setsizedefault(irc);
                                        a->setsize();         // get real window size :(
                                        a->setsize(irc, ire); // expand ToolBar check & set
                                        ire = a->getsize();
                                        break;
                                    }
                                case BaseData::MDIWinType::MWTYPE_CHILD_TOOLS:
                                    {
                                        if (ire.empty())
                                            break;
                                        a->setsize(irc, ire);
                                        //
                                        ImageLite::IRECT<int32_t> irt = a->getsize();
                                        if (irt.w > 0)
                                        {
                                            auto e = find(
                                                    static_cast<uint32_t>(BaseData::MDIWinType::MWTYPE_CHILD_EDIT)
                                                );
                                            if (!e)
                                                break;

                                            int32_t woff = (irc.w - irt.w);
                                            ImageLite::IRECT<int32_t> ird = e->getsize();
                                            if (ird.w != woff)
                                            {
                                                ire.w = woff;
                                                e->setsize(irc, ire);
                                                e->show();
                                            }
                                        }
                                        break;
                                    }
                                default:
                                    {
                                        if (ire.empty())
                                            break;
                                        a->setsize(irc, ire);
                                        break;
                                    }
                            }
                            break;
                        }
                    default:
                        break;
                }
            }
        }
        catch (...) {}
    }

};

