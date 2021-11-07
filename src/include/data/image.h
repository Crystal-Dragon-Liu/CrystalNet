#ifndef IMAGE_H
#define IMAGE_H

/*
    @brief maintain a Image class contains the width, height, channels and data.
*/
class Image{
public:
    Image():width_(0), height_(0), data_(nullptr){}
    //TODO implement a correct process.
    ~Image(){}
private:
    size_t width_;
    size_t height_;
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

    /*
        @brief resize image input, and create a new image with size (w, h)
    */
    extern Image* resizeImage(Image* og_data, size_t w, size_t h);

}

#endif