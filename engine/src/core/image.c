#include "core/image.h"

#include "core/logger.h"
#include "core/CDS/strings.h"

#include <stdlib.h>
#include <stb_image.h>


bool image_create(s32 width,s32 height, Img* out_img){
    if(width!=0)
        out_img->resolutionX = width;
    if(height!=0)
        out_img->resolutionY = height;
    out_img->pixels = (Pixel*)malloc(sizeof(Pixel) * out_img->resolutionX * out_img->resolutionY);
    if(out_img->pixels == NULL){
        ERR("failed to allocate memory for img!");
        return false;
    }
    return true;
}
bool image_destroy(Img* img){
    free(img->pixels);
    free(img->raw);
    img->pixels = NULL;
    img->raw = NULL;
    img->resolutionX  = img->resolutionY = 0;
    return true;
}
bool imgae_load(const char* file, Img* out_img){
    
    s32 channels;
    stbi_set_flip_vertically_on_load(true);
    u8* image_h = stbi_load(file,&out_img->resolutionX,&out_img->resolutionY,&channels,0);
    if(image_h == NULL){
        ERR("Failed to load img : %s | error : %s",file,stbi_failure_reason());
        return false;
    }
    u32 buffer_size = out_img->resolutionX * out_img->resolutionY * channels;

    if(!image_create(0,0,out_img)){
        ERR("failed to create img!");
        stbi_image_free(image_h);
        return false;
    }
    for (u32 i = 0; i < out_img->resolutionX * out_img->resolutionY; i++) {
        out_img->pixels[i].r = image_h[i * channels];
        out_img->pixels[i].g = image_h[i * channels + 1];
        out_img->pixels[i].b = image_h[i * channels + 2];
        out_img->pixels[i].a = channels == 4 ? image_h[i * channels + 3] : 255;
    }
    image_raw_data(out_img);

    stbi_image_free(image_h);
    return true;
}

bool image_raw_data(Img* img){
    img->raw = (u8*)malloc(img->resolutionX*img->resolutionY*4*sizeof(u8));
    for (u32 i = 0,j = 0; i < img->resolutionX*img->resolutionY; i++)
    {
        img->raw[j++] = img->pixels[i].r;
        img->raw[j++] = img->pixels[i].g;
        img->raw[j++] = img->pixels[i].b;
        img->raw[j++] = img->pixels[i].a;
    }
    return true;
}