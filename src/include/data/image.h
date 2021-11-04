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


#endif