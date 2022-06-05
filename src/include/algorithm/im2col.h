#ifndef IM2COL_H
#define IM2COL_H

#include <vector>
//From Berkeley Vision's Caffe!
//https://github.com/BVLC/caffe/blob/master/LICENSE
extern void im2ColCPU(std::vector<float>* batchData, int channels, int height, int width, int kernelSize, int stride, int pad, std::vector<float>* dataCol);

/*
 * @brief this method are implemented for the method im2col
 * @parameter 	img_width: 		the width of image without padding considerd.
 * 				img_height:		the height of image without padding considered.
 * 				img_channels:	channels of image, such as 3
 * 				index_row:		the index of row we need to read on a image in which paddings has been added.
 * 				index_col:		the index of column we need to read on a image in which paddings has been added.
 *              index_channel:  the index of channel
 * 				padding:		the length we expand on image with zero.
 */
extern float getPixelIm2col(std::vector<float>* img_data, int img_width, int img_height, int img_channels, int index_row, int index_col, int index_c, int padding);

 
#endif
