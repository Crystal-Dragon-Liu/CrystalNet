#ifndef COMMON_H
#define COMMON_H
#include <stdlib.h>
#include <iostream>

#define __LOG_MESSAGE__(s) std::cout << "==========" << #s << "==========" << std::endl;
#define __LOG_MESSAGE_X__(s) std::cout << "==========" << s << "==========" << std::endl;
struct KeyValuePair
{
    char* key_;
    char* value_;
    int used_;
};


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
            static void*    allocate(size_t num, size_t itemSize){
                void* result = allocate(num*itemSize);
                return result;
            }
            static void     deallocate(void* p, size_t){free(p);}
            static void*    reallocate(void* p, size_t new_size){
                void* result = realloc(p, new_size);
                if(!result) {__DETECT_BAD_ALLOC;}
                return result;
            }
    };

    class MyNetCAlloc{
        public:
            static void*    allocate(size_t num, size_t itemSize){
                void* result = calloc(num, itemSize);
                if(!result) {__DETECT_BAD_ALLOC;}
                return result;
            }
            static void     deallocate(void* p, size_t){free(p);}
            static void*    reallocate(void* p, size_t new_size){
                void* result = realloc(p, new_size);
                if(!result) {__DETECT_BAD_ALLOC;}
                return result;
            }
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
        static _Tp* allocate(size_t num, size_t itemSize){
            return reinterpret_cast<_Tp*>(NetAlloc::allocate(num, itemSize));}
        static void deallocate(_Tp* p, size_t n){
            if(0 != n) NetAlloc::deallocate(p, n * sizeof(_Tp));}
        static void deallocate(_Tp*p){
            NetAlloc::deallocate(p, sizeof(_Tp));}
        static _Tp* reallocate(_Tp* p, size_t new_size){
            return reinterpret_cast<_Tp*>(NetAlloc::reallocate(p, new_size));}
    };
}

using namespace MyAllocFunc;
typedef NetSimpleAlloc<float, MyNetAlloc> dataItemAllocator; // define a allocator.
typedef NetSimpleAlloc<float*, MyNetAlloc> data2DAllocator;
typedef NetSimpleAlloc<float,  MyNetCAlloc> dataCitemAllocator;
typedef NetSimpleAlloc<float*, MyNetCAlloc> dataC2DAllocator;

// /// @brief delete the 2d array with type _Tp. make sure rowCount is matched to data.
// template<class _Tp>
// void    delete2DArray(int rowCount, _Tp** data){
//     for(int i = 0; i < rowCount; i++){
//         delete[] data[i];
//     }
//     delete[] data;
// }





#endif