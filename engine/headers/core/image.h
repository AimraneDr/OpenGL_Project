#ifndef IMAGE_H
#define IMAGE_H
#define STB_IMAGE_IMPLEMENTATION

#include "defines.h"
#include "engine_types.h"
/// @brief create and alloc the memory needed for img
/// @param width pass 0 if the width stored in the out_img 
/// @param height pass 0 if the height stored in the out_img
/// @param out_img ref to the img holder
/// @return 
bool image_create(s32 width,s32 height, Img* out_img);
bool image_destroy(Img* img);
bool imgae_load(const char* file, Img* out_img);

/// @brief Fill the raw property with data
/// @param img handler to image
/// @return 
bool image_raw_data(Img* img);

#endif /* IMAGE_H */