#include "include/data/image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

namespace ImageFunc{

    Image* loadImage(const char* fileName, size_t w, size_t h, size_t channel){return nullptr;}
    
    Image* _loadImage(const char* fileName, size_t channels){return nullptr;}

    void   freeImage(Image* data){}

    Image* resizeImage(Image* og_data, size_t w, size_t h){return nullptr;}

}

