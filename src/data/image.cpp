#include "include/data/image.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include<iostream>
#include "include/Utils/common.h"
void Image::setData(float* data, size_t w, size_t h, size_t c){
    if(this->data_){
        // remove data_. maybe this operation is so fxxkin dangerous.
        float* data_temp = this->data_;
        dataCitemAllocator::deallocate(data_temp);
    }
    this->data_ = data;
    this->w_    = w;
    this->h_    = h;
    this->c_    = c;
}

Image::Image(size_t w, size_t h, size_t c):w_(w), h_(h), c_(c), data_(nullptr){
    float* data = dataCitemAllocator::allocate(h_*w_*c_, sizeof(float));
}

Image::~Image(){if(data_){
            dataCitemAllocator::deallocate(data_);
            data_ = nullptr;
        }
}


namespace ImageFunc{

    Image* loadImage(const char* fileName, size_t w, size_t h, size_t channel){return nullptr;}
    
    //TODO free the space of Image.
    void   freeImage(Image* image){delete image;}

    //TODO create cache for image.
    Image* makeImage(size_t w, size_t h, size_t c){
        Image* image = new Image(w, h, c); // with nullptr to image data.
        return image;
    }

    Image* resizeImage(Image* og_data, size_t w, size_t h){return nullptr;}

    Image* _loadImage(const char* fileName, size_t channels)
    {
        int w, h ,c; /// width, height, channel.
        unsigned char *data = stbi_load(fileName, &w, &h, &c, channels);
        if(!data){
            // print error info without any consideration about thread safe.
            std::cerr << "Cannot load image" << fileName << stbi_failure_reason() << std::endl;
            exit(0);   
        }
        free(data);
        return nullptr;
    }

}

