#ifndef CDS_DETAILS_HASH_H
#define CDS_DETAILS_HASH_H

#include "defines.h"
u64 hash_u64(u64 input);
u64 hash_unsigned(u64 key, u32 max);

u64 hash_signed(s64 key, u32 max);

u64 hash_f64(f64 key, u32 max);
u64 hash_f32(f32 key, u32 max);

u64 hash_string(const char* key, u32 max);

#endif /* CDS_DETAILS_HASH_H */