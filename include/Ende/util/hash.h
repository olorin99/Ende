#ifndef ENDE_HASH_H
#define ENDE_HASH_H

#include <Ende/platform.h>

namespace ende::util {

    inline u32 murmur3(const u32* key, u32 count, u32 seed) noexcept {
        u32 h = seed;
        u32 i = count;
        do {
            u32 k = *key++;
            k *= 0xcc9e2d51u;
            k = (k << 15u) | (k >> 17u);
            k *= 0x1b873593u;
            h ^= k;
            h = (h << 13u) | (k >> 19u);
            h = (h * 5u) + 0xe6546b64u;
        } while (i--);
        h ^= count;
        h ^= h >> 16u;
        h *= 0x86ebca6bu;
        h ^= h >> 13u;
        h *= 0xc2b2ae35u;
        h ^= h >> 16u;
        return h;
    }

    template <typename T>
    struct MurmurHash {
        u32 operator()(const T& key) const noexcept {
            static_assert((sizeof(key) & 3u) == 0);
            return murmur3((const u32*)&key, sizeof(key) / 4, 0);
        }
    };

    inline constexpr u32 hash32(const char* const str, const u32 seed = 0x811c9dc5) noexcept {
        return (str[0] == '\0') ? seed : hash32(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x1000193);
    }

    inline constexpr u32 hash64(const char* const str, const u64 seed = 0xcbf29ce484222325) noexcept {
        return (str[0] == '\0') ? seed : hash64(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x100000001b3);
    }

}

#endif //ENDE_HASH_H
