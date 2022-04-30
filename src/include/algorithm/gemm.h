#ifndef CRYSTALNET_GEMM_H
#define CRYSTALNET_GEMM_H

namespace GEMM_ALGO{
    /*
        @brief      GEneral Matrix to Matrix Multiplication;
                    the size of Matrix A could be expressed as [M, K];
                    the size of Matrix B could be expressed as [K, N];
                    the size of Matrix C could be expressed as [M, N];
                    lda: the column counts of Matrix A or the row counts of transposed Matrix A;
                    ldb: the column counts of Matrix B or the row counts of transposed Matrix B;
                    ldc: the column counts of Matrix C;
        @parameters transposeA: transpose Matrix A or not
                    transposeB; transpose Matrix B or not
    */
    extern void gemm(bool transposeA, bool transposeB, int M, int N, int K,
                float ALPHA, 
                float* A, int lda, float* B, int ldb,
                float BETA, 
                float*C, int ldc);

    extern void gemm_cpu(bool transposeA, bool transposeB, int M, int N, int K,
                float ALPHA, 
                float* A, int lda, float* B, int ldb,
                float BETA, 
                float*C, int ldc);
                
    extern void gemm_nn(int M, int N, int K,
                float ALPHA, 
                float* A, int lda, float* B, int ldb,
                float BETA, 
                float*C, int ldc);
}

#endif