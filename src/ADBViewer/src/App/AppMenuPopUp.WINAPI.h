
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
#define IDS_MENUMSG5 10185
#define IDS_MENUMSG6 10186
#define IDS_MENUMSG7 10187
#define IDS_MENUMSG8 10188
#define IDS_MENUMSG9 10189
#define IDS_MENUMSG10 10190
#define IDS_MENUMSG11 10191
#define IDS_MENUMSG12 10192
#define IDS_MENUMSG13 10193
#define IDS_MENUMSG14 10194
#define IDS_MENUMSG15 10195
#define IDS_MENUMSG16 10196

/// Android direct command
#define IDS_MENUMSG17 10197
#define IDS_MENUMSG18 10198
#define IDS_MENUMSG19 10199
#define IDS_MENUMSG20 10200
#define IDS_MENUMSG21 10201
#define IDS_MENUMSG22 10202
#define IDS_MENUMSG23 10203
#define IDS_MENUMSG24 10204
#define IDS_MENUMSG25 10205
#define IDS_MENUMSG26 10206

#define IDS_MENUEND  10207
