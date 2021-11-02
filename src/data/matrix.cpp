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

    void freeRawData(float** rawData, size_t row_count){
    if(rawData){
        for(size_t i = 0; i < row_count; i++){dataItemAllocator::deallocate(rawData[i]);}
        data2DAllocator::deallocate(rawData);}
    }
    
    void printMatrix(const Matrix& matrix){
        // awesome function.....!
        auto data = matrix.getData();
        std::cout << "Matrix: [" << matrix.getRowCount() << ", " << matrix.getColCount() << "]" << std::endl;
        printf(" __");
        for(size_t j = 0; j < 16 *matrix.getColCount()-1; ++j) printf(" ");
        printf("__ \n");
        printf("|  ");
        for(size_t j = 0; j < 16*matrix.getColCount()-1; ++j) printf(" ");
        printf("  |\n");
        for(size_t i = 0; i < matrix.getRowCount(); ++i){
            printf("|  ");
            for(size_t j = 0; j < matrix.getColCount(); ++j){
                printf("%15.7f ", data[i][j]);
                }
            printf(" |\n");
        }
        printf("|__");
        for(size_t j = 0; j < 16*matrix.getColCount()-1; ++j) printf(" ");
        printf("__|\n");
    }

    void     resizeMatrix(Matrix* m, size_t size){
        if(m->getRowCount() == size) return;
        auto val    =   m->getData();
        // TODO replace the code below with NetSimpleAlloc::deallocate()...
        val = dataC2DAllocator::reallocate(val, size*sizeof(float*));
        return;
    }

}



/* implement all the member function of class Matrix. */

Matrix::Matrix(): row_count_(0), col_count_(0), data_(nullptr){}
Matrix::~Matrix(){MatrixFunc::freeRawData(data_, row_count_);}

Matrix::Matrix(size_t rows, size_t cols){
    row_count_      =   rows;
    col_count_      =   cols;
    float** rawData =   MatrixFunc::make2DArray(row_count_, col_count_);
    this->data_  =   rawData;   
}

Matrix& Matrix::operator=(Matrix& data){
    // delete raw data.
    if(this == &data) return *this;
    float** rawdataTemp = data_;
    data_ = MatrixFunc::make2DArray(data.row_count_, data.col_count_);
    // copy data.
    float** otherData = data.getData();
    for(size_t i = 0; i < data.row_count_; i++){
        memcpy(data_[i], otherData[i], data.getColCount()*sizeof(float));}
    MatrixFunc::freeRawData(rawdataTemp, row_count_);
    return *this;
}







