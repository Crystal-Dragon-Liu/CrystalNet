#include "include/data/matrix.h"

namespace MatrixFunc{

    float** make2DArray(size_t rows, size_t cols){
        float** rawData = dataC2DAllocator::allocate(rows, sizeof(float*));
        for(size_t i = 0; i < rows; i++){
            rawData[i] = dataCitemAllocator::allocate(cols, sizeof(float));}
        return rawData;
    }

    Matrix* copyMatrix(Matrix* matrix){return nullptr;}

}


Matrix::Matrix(): row_count_(0), col_count_(0), rawdata_(nullptr){}

Matrix::~Matrix(){
    if(rawdata_){
        for(size_t i = 0; i < row_count_; i++){dataItemAllocator::deallocate(rawdata_[i]);}
        data2DAllocator::deallocate(rawdata_);
    }
}

Matrix::Matrix(size_t rows, size_t cols){
    row_count_      =   rows;
    col_count_      =   cols;
    float** rawData =   MatrixFunc::make2DArray(row_count_, col_count_);
    this->rawdata_  =   rawData;   
}






