#include "include/algorithm/gemm.h"

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
    void gemm(bool transposeA, bool transposeB, int M, int N, int K,
                float ALPHA, 
                std::vector<float>* A, int lda, std::vector<float>* B, int ldb,
                float BETA, 
                std::vector<float>* C, int ldc){
                gemm_cpu(transposeA, transposeB, M, N, K, ALPHA, A, lda, B, ldb, BETA, C, ldc);
    }

    void gemm_cpu(bool transposeA, bool transposeB, int M, int N, int K,
                float ALPHA, 
                std::vector<float>* A, int lda, std::vector<float>* B, int ldb,
                float BETA, 
                std::vector<float>* C, int ldc){
                int i, j;
                for(i = 0; i < M; i++){
                    for(j=0; j < N; j++){
                        (*C)[i*ldc + j]*= BETA;
                    }
                }
        if(!transposeA && !transposeB){
            gemm_nn(M, N, K, ALPHA, A, lda, B, ldb, BETA, C, ldc);
        }
        //TODO gemm_tn, gemm_tt, gemm_nt
    }
                
    void gemm_nn(int M, int N, int K,
                float ALPHA, 
                std::vector<float>* A, int lda, std::vector<float>* B, int ldb,
                float BETA, 
                std::vector<float>* C, int ldc){
        int i, j, k;
        for(i = 0; i < M; i++){
            for(k = 0; k < K; k++){
                float A_part = ALPHA * (*A)[lda* i + k];
                for(j = 0; j < N; j++){
                    (*C)[i*ldc + j] += A_part* (*B)[ldb*k+j];
                }
            }
        }
    }
}