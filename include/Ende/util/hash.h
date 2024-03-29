#ifndef ENDE_HASH_H
#define ENDE_HASH_H

#include <Ende/platform.h>

namespace ende::util {

    constexpr inline auto combineHash(u32 first, u32 second) -> u32 {
        return first ^= second + 0x9e3779b9 + (first<<6) + (first>>2);
    }

    constexpr inline auto combineHash(u64 first, u64 second) -> u64 {
        return first ^= second + 0x9e3779b97f4a7c15 + (first<<12) + (first>>4);
    }

    inline auto murmur3(const u32* key, u32 count, u32 seed) noexcept -> u32 {
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
        } while (--i);
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
        auto operator()(const T& key) const noexcept -> u32 {
            static_assert((sizeof(key) & 3u) == 0);
            return murmur3((const u32*)&key, sizeof(key) / 4, 0);
        }
    };

    inline constexpr auto hash32(const char* const str, const u32 seed = 0x811c9dc5) noexcept -> u32 {
        return (str[0] == '\0') ? seed : hash32(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x1000193);
    }

    inline constexpr auto hash64(const char* const str, const u64 seed = 0xcbf29ce484222325) noexcept -> u32 {
        return (str[0] == '\0') ? seed : hash64(&str[1], (seed ^ static_cast<u32>(str[1])) * 0x100000001b3);
    }

}

#endif //ENDE_HASH_H
