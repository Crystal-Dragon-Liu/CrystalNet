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
        float** getData() const { return data_; }
        size_t  getRowCount() const {return row_count_;}
        size_t  getColCount() const {return col_count_;}
        void    setRowCount(size_t row_count) {row_count_ = row_count;}
        void    setColCount(size_t col_count) {col_count_ = col_count;}
        Matrix& operator=(Matrix& data);
        // Shallow-copy constructor may be enough.
        Matrix(const Matrix& matrix){
                row_count_ = matrix.getRowCount();
                col_count_ = matrix.getColCount();
                data_      = matrix.getData();
        }
        // maybe the data is dangerous, but i don care. :)
private:
        /* maybe copy constructor should be forbidden. */
        float** data_;
        size_t  row_count_;
        size_t  col_count_;
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
        

        /*      @brief  free the raw data which is usually belong to Matrix obj.
                @param  rawData: the data with type float**.
                @param  row_count: the row count of data.       
        */
        extern void     freeRawData(float** rawData, size_t row_count);

        /*
                @brief  print Matrix data with its size which is awesome function.
        */
        extern void     printMatrix(const Matrix& matrix);

        /*
                @brief  resize the row count of matrix.
                @param  size: the new size of row.
                @param  m:    matrix data.
        */
        extern void     resizeMatrix(Matrix* m, size_t size);

        /*      @brief  implement a scale on each element of Matrix.
                @param  m is the matrix.
                @param  scale is the factor implemented on each element.
        */
        extern void     scaleMatrix(Matrix*m , float scale = 1.0);

        /*
                @brief  provide a method add the each of element of srcMatrix to dstMatrix.
                @param  srcMatrix source matrix.
                @param  dstMatrix target matrix which would be  changed.
        */
        extern void     matrixAddMatrix(Matrix* srcMatrix, Matrix* dstMatrix);

        /*
                @brief  delete a column from matrix.
                @param  matrix: the matrix which would be modified.
                @param  column: the index of column.the column of data with this index 
                        would be removed logically.
        */
        extern float*   popColumn(Matrix* matrix, size_t column);
        //TODO hold_out_matrix
        extern Matrix   holdOutMatrix(Matrix* matrix, size_t m);

        //TODO csv_to_matrix
        //TODO matrix_to_csv
        //TODO matrix_topk_accuracy

      
}

#endif 