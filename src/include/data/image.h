#ifndef IMAGE_H
#define IMAGE_H
#include "include/Utils/common.h"
/*
    @brief maintain a Image class contains the width, height, channels and data.
*/
class Image{
public:
    Image():w_(0), h_(0), c_(0), data_(nullptr){}

    Image(size_t w, size_t h, size_t c);

    //TODO implement a correct process. no idea about the ref_count.
    ~Image();
    /*
        @brief  set data for empty Image obj.
                this method should check whether the data_ is empty or not.
                if it's not set to empty, the data stored in the Image obj 
                should be removed.
    */
    void setData(float* data, size_t w, size_t h, size_t c);
    
    //TODO implement Image& Image::operator=(Image& data)
    

    size_t getWidth() const {return w_;}
    size_t getHeight() const {return h_;}
    size_t getChannels() const {return c_;}
    float* getData() const { return data_; }
    /*
        @brief  deep copy.
    */
    Image& operator=(Image& data);

private:
    /*
        @brief swallow copy which may be not good enough. 
    */
    Image(const Image& image){
                w_ = image.getWidth();
                h_ = image.getHeight();
                c_ = image.getChannels();
                data_ = image.getData();
    }
    size_t w_;
    size_t h_;
    size_t c_;
    float* data_;
};

namespace ImageFunc{

    /*
        @brief  load a image from filename expected.
        @param filename:    file path.
        @param w:           width of image.
        @param h:           height of image.
        @param channel:     channel of 
    */
    extern Image* loadImage(const char* fileName, size_t w, size_t h, size_t channel);

    /*
        @brief  load a image by calling the func from stb_image lib.
    */
    extern Image* _loadImage(const char* fileName, size_t channels);

    /*
        @brief  free image data.
    */
    extern void   freeImage(Image* data);

    extern void   freeRawData(float*);

    extern float* makeRawData(size_t len);

    /*
        @brief create cache for Image.
    */
    extern Image* makeImage(size_t w, size_t h, size_t c);

    /*
        @brief resize image input, and create a new image with size (w, h)
    */
    extern Image* resizeImage(Image* og_data, size_t w, size_t h);

}

#endif