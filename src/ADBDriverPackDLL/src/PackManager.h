#pragma once

namespace Resources
{
    class DLL_EXPORT PackManager
    {
        public:

            enum IndexStringBinary
            {
                PACK_ARM64V8A,
                PACK_ARMEABIV7A,
                PACK_X86,
                PACK_X86_64
            };

            PackManager();
            ~PackManager();

            static bool checkbin(std::string&, std::string&, std::string&);

        private:
            static bool savebin(const uint8_t*, size_t);

    };
}
