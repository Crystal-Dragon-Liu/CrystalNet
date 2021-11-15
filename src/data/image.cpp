#include "include/data/image.h"

extern "C"{
#define STB_IMAGE_IMPLEMENTATION
#include "include/image_process/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "include/image_process/stb_image_write.h"
#include <iostream>
}

void Image::setData(float* data, size_t w, size_t h, size_t c){
    if(this->data_){
        // remove data_. maybe this operation is so fxxkin dangerous.
        float* data_temp = this->data_;
        ImageFunc::freeRawData(data_temp);
        // dataCitemAllocator::deallocate(data_temp);
    }
    this->data_ = data;
    this->w_    = w;
    this->h_    = h;
    this->c_    = c;
}

Image::Image(size_t w, size_t h, size_t c):w_(w), h_(h), c_(c), data_(nullptr){
    float* data = ImageFunc::makeRawData(h_*w_*c_);
    // float* data = dataCitemAllocator::allocate(h_*w_*c_, sizeof(float));
    data_ = data;
}

Image::~Image(){
    if(data_){
        ImageFunc::freeRawData(data_);
        // dataCitemAllocator::deallocate(data_);
        data_ = nullptr;
    }
}

Image& Image::operator=(Image& image){
    // delete raw data.
    if(this == &image) return *this;
    float* rawDataTemp = data_;
    size_t dataLen = image.getHeight()*image.getWidth()* image.getChannels();
    data_ = ImageFunc::makeRawData(dataLen);
    // data_ = dataCitemAllocator::allocate(dataLen, sizeof(float));
    if(data_){
        float* imageData = image.getData();
        memcpy(data_, imageData, dataLen);
    }
    if(rawDataTemp)
        ImageFunc::freeRawData(rawDataTemp);
    h_ = image.getHeight();
    w_ = image.getWidth();
    c_ = image.getChannels();
    return *this;
}


namespace ImageFunc{

    Image* loadImage(const char* fileName, size_t w, size_t h, size_t channel){return nullptr;}

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
     
    //TODO free the space of Image.
    void   freeImage(Image* image){
        delete image;
    }

    //TODO create cache for image.
    Image* makeImage(size_t w, size_t h, size_t c){
        Image* image = new Image(w, h, c); // with nullptr to image data.
        return image;
    }

    Image* resizeImage(Image* og_data, size_t w, size_t h)
    {
        return nullptr;
    }

    void   freeRawData(float* data){
        dataCitemAllocator::deallocate(data);
    }
    
    float*  makeRawData(size_t len){
        if(len == 0) return nullptr;
        auto data = dataCitemAllocator::allocate(len, sizeof(float));
        return data;
    }

}


