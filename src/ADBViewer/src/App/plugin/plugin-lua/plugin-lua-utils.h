
#ifndef __LUA_UTILS_PLUGIN_H__
#define __LUA_UTILS_PLUGIN_H__

static inline int32_t utils_getpad(int32_t w)
    {
        int32_t pad = 0;
        while (((w + pad) % 4) != 0)
            pad++;
        return pad;
    }

static inline int32_t utils_checkpos(int32_t w, int32_t h, int32_t pad)
    {
        return (((((int32_t)w * 3) + pad) * (int32_t)h) - 3);
    }

static inline int32_t utils_getpos(int32_t x, int32_t y, int32_t w, int32_t h)
    {
        int32_t pad = utils_getpad(w),
                pos = (((((int32_t)w * 3) + pad) * y) + (x * 3));
        if (pos > utils_checkpos(w, h, pad))
            return -1;
        return pos;
    }

#endif
