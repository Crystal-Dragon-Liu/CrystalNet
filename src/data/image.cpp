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

/*
    @brief get pixel with width, height, channel expected.
           w stands for the nums of columns.
           h means the nums of rows.
*/
float  Image::getPixel(size_t w, size_t h, size_t c) const{
    assert(w < w_ && h < h_ && c < c_);
    return data_[c*h_*w_ + h*w_ + w];
}

void   Image::setPixel(float val, size_t w, size_t h, size_t c) {
	assert(w < w_ && h < h_ && c < c_);
	data_[c*h_*w_ + h * w_ + w] = val;
}

void   Image::addPixel(float val, size_t w, size_t h, size_t c) {
	assert(w < w_ && h < h_ && c < c_);
	data_[c*h_*w_ + h * w_ + w] = data_[c*h_*w_ + h * w_ + w] + val;
}


namespace ImageFunc{

    void   printShape(Image* data){
        if(!data) std::cout << "<invalid image data...>" << std::endl;
        std::cout << "image loaded with size [" << data->getWidth() << ", " << data->getHeight() << ", " << data->getChannels() << "]" << std::endl;
    }

    Image* loadImage(const char* fileName, size_t w, size_t h, size_t channel){
        Image* out = _loadImage(fileName, channel);
        if((h && w) && (out->getHeight() != h && out->getWidth() != w)){
            // TODO resize the image.
            Image* data = resizeImage(out, w, h);
            // TODO delete the data stored in out.
            freeImage(out);
            out = data;
        }
        return out; //TODO return the result but not out.
    }

    Image* _loadImage(const char* fileName, size_t channels)
    {
        int w, h ,c; /// width, height, channel.
        unsigned char *data = stbi_load(fileName, &w, &h, &c, channels);
        if(!data){
            // print error info without any consideration about thread safe.
            std::cerr << "Cannot load image" << fileName << stbi_failure_reason() << std::endl;
            exit(0);   
        }
        Image* new_image = ImageFunc::makeImage(w, h, c);
        printShape(new_image);
        auto new_image_data = new_image->getData();
        // switch the position of pixel.
        for(size_t k = 0; k < c; k++){
            for(size_t j = 0; j < h; j++){
                for(size_t i = 0; i < w; i++){
                    size_t dst_index = i + w*j + w*h*k;
                    size_t src_index = k + c*i + c*w*j;
                    new_image_data[dst_index] = static_cast<float>(data[src_index])/255.;
                    // new_image.data[dst_index] = (float)data[src_index]/255.;
                }
            }
        }
        free(data);
        return new_image;
    }
     
    void   freeImage(Image* image){
        delete image;
    }

    Image* makeImage(size_t w, size_t h, size_t c){
        Image* image = new Image(w, h, c); // with nullptr to image data.
        return image;
    }

    Image* resizeImage(Image* og_data, size_t w, size_t h){
               size_t c_o = og_data->getChannels();
        size_t h_o = og_data->getHeight();
        size_t w_o = og_data->getWidth();
        Image* resized = makeImage(w, h, c_o);
        Image* temp = makeImage(w, h_o, c_o);
		// calculate the scale of width and height without considering the boundary of image,
		// in that the edge of image should be reserved.
        float w_scale = static_cast<float>(w_o - 1) / (w-1);
        float h_scale = static_cast<float>(h_o - 1) / (h-1);
        for(size_t k = 0; k < c_o; k++){
            for(size_t r = 0; r < h_o; r++){
                for(size_t c = 0; c < w; c++){
                    // TODO calculate the resized data.
                    float val = 0;
                    if(c == w - 1 || w_o == 1){
                        val = og_data->getPixel(w_o - 1, r, k); // assignment the right edge of pixels for new image.
                    }
					else {
						float sx = c * w_scale;
						int ix = static_cast<int>(sx);
						float dx = sx - ix;
						val = (1 - dx) * og_data->getPixel(ix, r, k) + dx * og_data->getPixel(ix+1, r, k);
					}
					temp->setPixel(val, c, r, k);
                }
            }
        }
		
		// step 2, interpolation for y axis.
		for (size_t k = 0; k < c_o; ++k) {
			for (size_t r = 0; r < h; ++r) {
				float sy = r * h_scale;
				int iy = static_cast<int>(sy);
				float dy = sy - iy;
				for (size_t c = 0; c < w; ++c) {
					float val = (1 - dy)* temp->getPixel(c, iy, k);
					resized->setPixel(val, c, r, k);
				}
				if (r == h - 1 || h_o == 1) continue;
				for (size_t c = 0; c < w; ++c) {
					float val = dy * temp->getPixel(c, iy + 1, k);
					resized->addPixel(val, c, r, k);
				}
			}
		}
		ImageFunc::freeImage(temp);
		return resized; //! dont forget to delete it.
    }

    void   freeRawData(float* data){
        DataCitemAllocator::deallocate(data);
    }
    
    float*  makeRawData(size_t len){
        if(len == 0) return nullptr;
        auto data = DataCitemAllocator::allocate(len, sizeof(float));
        return data;
    }
}


