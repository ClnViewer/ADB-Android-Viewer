
#if defined(_BUILD_LUA_EDITOR)

    __LUA_FUNC_STATIC_TEMPLATE(imageTableShow,

        __LUA_LINT_UNUSED(ret);

        LINT_TRY__
        {
            __LUA_IF_LINT(
                LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
            )
            //
            if (args != 1)
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, args);
                )
                break;
            }
            if (!lua_istable(L, -1))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_30, LuaLint::g_lint_str_43);
                )
                break;
            }

            ImageLite::ImageData data{};
            if (!LuaObject::getLuaImage(L, data))
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_33, LuaLint::g_lint_str_48);
                )
                break;
            }

            __LUA_IF_LINT(
                LuaLint::print_LuaImage(data);
                /// LuaLint::print_LuaImage_data(data)
            )

            ImageLite::ImageRGBbuffer img(data, ImageLite::BufferType::IBT_RGB);
            if (img.empty())
            {
                __LUA_IF_LINT(
                    LuaLint::print_traceT(LuaLint::g_lint_str_34, 0);
                )
                break;
            }
            f_image_show(0, le, "lua image", img, img);
        }
        LINT_CATCH__
    )
#else
    __LUA_FUNC_STATIC_TEMPLATE(imageTableShow,

        __LUA_LINT_UNUSED(ret);
        __LUA_LINT_UNUSED(args);
        __LUA_IF_LINT(
            LuaLint::print_traceT(LuaLint::g_lint_str_32, functionname);
        )
        break;
    )
#endif
