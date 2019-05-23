
#define MENU_ITEM_ADD(ID1,ID2,MAP,M) __extension__ ({                        \
    auto wtxt = f_GetStringFromMap(ID2,MAP);                                 \
    AppendMenuW(M, MF_STRING, ID1, wtxt);                     \
    })

#define MENU_ADD(ID1,ID2,MAP,M) __extension__ ({                              \
    auto wtxt = f_GetStringFromMap(ID2,MAP);                                  \
    AppendMenuW(M, MF_STRING | MF_POPUP, (UINT_PTR)ID1, wtxt); \
    })

#define RESOURCE_MAP(ID1,ID2,MAP)                                             \
        std::map<int32_t, std::wstring> MAP;                                  \
        {                                                                     \
            HINSTANCE hinst = hmodule_get();                                  \
            string_from_res<std::wstring> wres{};                             \
                                                                              \
            for (int32_t i = ID1; i <= ID2; i++)                              \
                MAP.insert(                                                   \
                    std::make_pair(                                           \
                        i,                                                    \
                        wres.go(hinst, i)                                     \
                    )                                                         \
                );                                                            \
        }

#define IDS_MENUMSG0 10180
#define IDS_MENUMSG1 10181
#define IDS_MENUMSG2 10182
#define IDS_MENUMSG3 10183
#define IDS_MENUMSG4 10184
#define IDS_MENUEND  10185
