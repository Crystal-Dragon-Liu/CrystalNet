#ifndef MATRIX_H
#define MATRIX_H
#include "include/Utils/common.h"
using namespace MyAllocFunc;
typedef NetSimpleAlloc<float, MyNetAlloc> dataItemAllocator; // define a allocator.
typedef NetSimpleAlloc<float*, MyNetAlloc> data2DAllocator;
typedef NetSimpleAlloc<float,  MyNetCAlloc> dataCitemAllocator;
typedef NetSimpleAlloc<float*, MyNetCAlloc> dataC2DAllocator;
class  Matrix{
public:
        Matrix();
        Matrix(size_t rows, size_t cols);
        virtual ~Matrix();
        float** getData();
private:
        size_t  row_count_;
        size_t  col_count_;
        float** rawdata_;
};

namespace MatrixFunc{
        extern Matrix*    makeMatrix(size_t rows, size_t cols);
}

    // extern Matrix   makeMatrix(size_t rows, size_t cols);

#endif 