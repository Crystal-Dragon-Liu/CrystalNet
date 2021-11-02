#ifndef MATRIX_H
#define MATRIX_H
#include "include/Utils/common.h"
using namespace MyAllocFunc;
typedef NetSimpleAlloc<float, MyNetAlloc> dataItemAllocator; // define a allocator.
typedef NetSimpleAlloc<float*, MyNetAlloc> data2DAllocator;
typedef NetSimpleAlloc<float,  MyNetCAlloc> dataCitemAllocator;
typedef NetSimpleAlloc<float*, MyNetCAlloc> dataC2DAllocator;


/*
        @brief Matrix provide a wrapper for 2d array.
*/
class  Matrix{
public:
        /*      @brief create a new Matrix without rawdata. */
        Matrix();
        /*
                @brief  create a new Matrix with size(rows, cols)
                @param  rows: row counts.
                @param  cols: column counts.
        */
        Matrix(size_t rows, size_t cols);
        virtual ~Matrix();
        float** getData(){ return rawdata_; }
        size_t  getRowCount() const {return row_count_;}
        size_t  getColCount() const {return col_count_;}
        Matrix& operator=(Matrix& data);
private:
        /* maybe copy constructor should be forbidden. */
        Matrix(const Matrix& matrix){}
        size_t  row_count_;
        size_t  col_count_;
        float** rawdata_;
};


/*
    @brief  the method under MatrixFunc namespace provide the general operation,
            to process matrix data.
*/
namespace MatrixFunc{
        /*
                @brief  create a new Matrix with size (rows, cols).
                        no data would be stored in the matrix built by this method.
                @param  rows: row counts.
                @param  cols: column counts.
        */
        extern float**  make2DArray(size_t rows, size_t cols);
        
        //TODO create a brand new Matrix obj with same data from matrix expected.
        extern Matrix*  copyMatrix(Matrix* matrix);

        /*      @brief  free the raw data which is usually belong to Matrix obj.
                @param  rawData: the data with type float**.
                @param  row_count: the row count of data.       
        */
        extern void     freeRawData(float** rawData, size_t row_count);
      
}

#endif 