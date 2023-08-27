#include "core/CDS/details/hash.h"

#include "string.h"


u64 hash_unsigned(u64 key, u32 max){
    const u64 c1 = 0x87c37b91114253d5ULL;
    const u64 c2 = 0x4cf5ad432745937fULL;
    const u64 seed = 11274528463;  // You can change the seed value if desired

    u64 hashValue = seed ^ key;

    hashValue ^= hashValue >> 33;
    hashValue *= c1;
    hashValue ^= hashValue >> 33;
    hashValue *= c2;
    hashValue ^= hashValue >> 33;

    return hashValue % max;
}



u64 hash_signed(s64 key, u32 max){
    return hash_unsigned((u64)key, max);
}


u64 hash_f64(f64 key, u32 max){
    const u64 c1 = 0xff51afd7ed558ccdULL;
    const u64 c2 = 0xc4ceb9fe1a85ec53ULL;
    const u64 seed = 0; // You can change the seed value if desired

    u64 hashValue = 0;

    // Convert the key to a 64-bit unsigned integer
    u64 keyAsUInt;
    memcpy(&keyAsUInt, &key, sizeof(u64));

    const u64* data = &keyAsUInt;
    const u32 numBlocks = sizeof(u64) / 8;

    // MurmurHash3 mixing algorithm
    for (u32 i = 0; i < numBlocks; ++i) {
        u64 k = data[i];
        k *= c1;
        k ^= k >> 31;
        k *= c2;
        hashValue ^= k;
        hashValue ^= hashValue >> 33;
        hashValue *= 0xff51afd7ed558ccdULL;
        hashValue ^= hashValue >> 33;
        hashValue *= 0xc4ceb9fe1a85ec53ULL;
        hashValue ^= hashValue >> 33;
    }

    hashValue %= max;
    return hashValue;
}

u64 hash_f32(f32 key, u32 max){
    const u64 c1 = 0xff51afd7ed558ccdULL;
    const u64 c2 = 0xc4ceb9fe1a85ec53ULL;
    const u64 seed = 0; // You can change the seed value if desired

    u64 hashValue = 0;

    // Convert the key to a 32-bit unsigned integer
    u32 keyAsUInt;
    memcpy(&keyAsUInt, &key, sizeof(u32));

    const u32* data = &keyAsUInt;
    const u32 numBlocks = sizeof(u32) / 4;

    // MurmurHash3 mixing algorithm
    for (u32 i = 0; i < numBlocks; ++i) {
        u32 k = data[i];
        k *= c1;
        k ^= k >> 24;
        k *= c2;
        hashValue ^= k;
        hashValue ^= hashValue >> 33;
        hashValue *= 0xff51afd7ed558ccdULL;
        hashValue ^= hashValue >> 33;
        hashValue *= 0xc4ceb9fe1a85ec53ULL;
        hashValue ^= hashValue >> 33;
    }

    hashValue %= max;
    return hashValue;
}

/// @brief used hash algorithm : FNV-1a 
/// @param key 
/// @param max 
/// @return 
u64 hash_string(const char* key, u32 max){
    const u64 FNV_PRIME = 1099511628211ULL;
    const u64 FNV_OFFSET_BASIS = 14695981039346656037ULL;
    const u64 ROTATION_CONSTANT = 47; // Experiment with different constants

    u64 hashValue = FNV_OFFSET_BASIS;
    // FNV-1a hash with mixing
    for (u32 i = 0; key[i] != '\0'; i++) {
        hashValue ^= (u64)key[i];
        hashValue *= FNV_PRIME;
        hashValue ^= (hashValue >> ROTATION_CONSTANT);
    }

    hashValue %= (u64)max;
    return hashValue;
}
