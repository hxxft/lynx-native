#ifndef BASE_BIG_ENDIAN_H_
#define BASE_BIG_ENDIAN_H_

namespace base {
    template<typename T>
    inline void ReadBigEndian(const char buf[], T* out) {
        *out = buf[0];
        for (size_t i = 1; i < sizeof(T); ++i) {
            *out <<= 8;
            // Must cast to uint8_t to avoid clobbering by sign extension.
            *out |= static_cast<uint8_t>(buf[i]);
        }
    }
    
    template<typename T>
    inline void WriteBigEndian(char buf[], T val) {
        for (size_t i = 0; i < sizeof(T); ++i) {
            buf[sizeof(T)-i-1] = static_cast<char>(val & 0xFF);
            val >>= 8;
        }
    }
    
    template <>
    inline void ReadBigEndian<uint8_t>(const char buf[], uint8_t* out) {
        *out = buf[0];
    }
    
    template <>
    inline void WriteBigEndian<uint8_t>(char buf[], uint8_t val) {
        buf[0] = static_cast<char>(val);
    }
}

#endif
