#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
namespace MyAllocFunc{

    #define __DETECT_BAD_ALLOC std::cerr << "failed to allocate space!" << std::endl; exit(1)
    /*
        @brief implement a so fxxking simple alloc without any problem
                involving out of memory. 
    */
    //TODO memory problem should be fixed.
    class MyNetAlloc{
       public:
            static void*    allocate(size_t n){
                void* result = malloc(n);
                if(!result) {__DETECT_BAD_ALLOC;}
                return result;}
            static void     deallocate(void* p, size_t){free(p);}
    };

    /*
        @brief implement a simple Alloc to wrap the NetAlloc we defined.
    */
    template<class _Tp, class NetAlloc>
    class NetSimpleAlloc
    {
    public:
        static _Tp* allocate(size_t n){
            return 0 == n ? nullptr:reinterpret_cast<_Tp*>(NetAlloc::allocate(n * sizeof(_Tp)));
            }
        static _Tp* allocate(void){
            return reinterpret_cast<_Tp*>(NetAlloc::allocate(sizeof(_Tp)));}
        static void deallocate(_Tp* p, size_t n){
            if(0 != n) NetAlloc::deallocate(p, n * sizeof(_Tp));}
        static void deallocate(_Tp*p){
            NetAlloc::deallocate(p, sizeof(_Tp));}
    };
}


// /// @brief delete the 2d array with type _Tp. make sure rowCount is matched to data.
// template<class _Tp>
// void    delete2DArray(int rowCount, _Tp** data){
//     for(int i = 0; i < rowCount; i++){
//         delete[] data[i];
//     }
//     delete[] data;
// }





#endif