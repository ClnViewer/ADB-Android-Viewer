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

#include <string>
#include <array>
#include <vector>
#include <functional>
#include <lua.hpp>
#include <ImageLite.h>
#include "LuaObject.h"

namespace LuaObject
{
        bool getLuaImage(lua_State *L, ImageLite::ImageData & data)
        {
            try
            {
                do
                {
                    if (!lua_istable(L, -1))
                        break;

                    uint32_t itype = 0;

                    if (!getfield<int32_t>(L, "width",  data.point.x))
                        break;
                    if (!getfield<int32_t>(L, "height", data.point.y))
                        break;
                    if (!getfield<uint32_t>(L, "itype",  itype))
                        break;
                    if ((!data.point.x) || (!data.point.y) || (itype))
                        break;
                    if (!getfield<uint32_t>(L, "data", itype, LUA_TTABLE))
                    {
                        ::lua_pop(L, 1);
                        break;
                    }
                    data.buffer = LuaObject::getT<uint8_t, 3>(L);
                    ::lua_pop(L, 1);

                    if (!data.buffer.size())
                        break;
                    return true;
                }
                while (0);
                return false;
            }
            catch (...)
            {
                throw;
            }
        }

        void setLuaImage(lua_State *L, ImageLite::ImageData const & data)
        {
            ::lua_newtable(L);
            {
                LuaObject::setfieldT<int32_t>(L, "width",  data.point.x);
                LuaObject::setfieldT<int32_t>(L, "height", data.point.y);
                LuaObject::setfieldT<int32_t>(L, "itype",  0);
                LuaObject::setfieldT<int32_t>(L, "data");
                LuaObject::setimagedataT<ImageLite::ImgBuffer>(L, data.buffer);
            }
            ::lua_settable(L, -3);
        }

        bool setformatLuaImage(lua_State *L, ImageLite::ImageRGBbuffer & img)
        {
            return LuaObject::setformatLuaImage(L, img.getbuffer(), img.ispad(), img.getpad());
        }

        bool setformatLuaImage(
            lua_State *L,
            ImageLite::ImageData & src,
            ImageLite::BufferType t,
            int32_t ipad)
        {
            if (t == ImageLite::BufferType::IBT_PAD)
            {
                ImageLite::ImageData dst{};
                if(!ImageLite::Formater::iconvert(
                        &ImageLite::Formater::CONVERT_TO_NOPAD,
                        src.buffer,
                        dst.buffer,
                        src.point.x,
                        src.point.y,
                        src.stride,
                        ipad,
                    nullptr
                    ))
                    return false;
                dst.point  = src.point;
                dst.stride = src.stride;
                LuaObject::setLuaImage(L, dst);
            }
            else if (t == ImageLite::BufferType::IBT_NOPAD)
                LuaObject::setLuaImage(L, src);
            else
                return false;
            return true;
        }
};
