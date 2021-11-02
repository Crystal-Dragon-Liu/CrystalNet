#include "include/data/matrix.h"
#include "memory.h"

/*
    @brief  the method under MatrixFunc namespace provide the general operation,
            to process matrix data.
*/
namespace MatrixFunc{

    float** make2DArray(size_t rows, size_t cols){
        if(rows == 0 || cols == 0) return nullptr;
        float** rawData = dataC2DAllocator::allocate(rows, sizeof(float*));
        for(size_t i = 0; i < rows; i++){
            rawData[i] = dataCitemAllocator::allocate(cols, sizeof(float));}
        return rawData;
    }

    Matrix* copyMatrix(Matrix* matrix){return nullptr;}

    void freeRawData(float** rawData, size_t row_count){
    if(rawData){
        for(size_t i = 0; i < row_count; i++){dataItemAllocator::deallocate(rawData[i]);}
        data2DAllocator::deallocate(rawData);}
    }

}


Matrix::Matrix(): row_count_(0), col_count_(0), rawdata_(nullptr){}
Matrix::~Matrix(){MatrixFunc::freeRawData(rawdata_, row_count_);}

Matrix::Matrix(size_t rows, size_t cols){
    row_count_      =   rows;
    col_count_      =   cols;
    float** rawData =   MatrixFunc::make2DArray(row_count_, col_count_);
    this->rawdata_  =   rawData;   
}

Matrix& Matrix::operator=(Matrix& data){
    // delete raw data.
    if(this == &data) return *this;
    float** rawdataTemp = rawdata_;
    rawdata_ = MatrixFunc::make2DArray(data.row_count_, data.col_count_);
    // copy data.
    float** otherData = data.getData();
    for(size_t i = 0; i < data.row_count_;){
        memcpy(rawdata_[i], otherData[i], data.getColCount()*sizeof(float));}
    MatrixFunc::freeRawData(rawdataTemp, row_count_);
    return *this;
}







