#include "include/data/matrix.h"
Matrix::Matrix(): row_count_(0), col_count_(0), rawdata_(nullptr){}
float** Matrix::getData(){return rawdata_;}

Matrix::~Matrix(){
    if(rawdata_){
        for(size_t i = 0; i < row_count_; i++){
            dataItemAllocator::deallocate(rawdata_[i]);
        }
        data2DAllocator::deallocate(rawdata_);
    }
}
namespace MatrixFunc{
float**  make2DArray(size_t rows, size_t cols){
    float** rawData = dataC2DAllocator::allocate(rows, sizeof(float*));
    for(size_t i = 0; i < rows; i++){
        rawData[i] = dataCitemAllocator::allocate(cols, sizeof(float));}
    return rawData;
}
}


