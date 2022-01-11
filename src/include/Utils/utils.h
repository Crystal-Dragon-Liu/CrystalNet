#ifndef UTILS_H
#define UTILS_H
#include <string>


namespace UtilFunc
{
    
    extern void stripUselessChr(std::string& str);
    extern void copyCharArray(char* des, const std::string& line);
    extern void printCharArray(char* data);
    extern void printConfigSection(void* data);
    extern void printkyp(void* data);
    extern void freeKyp(void* data);
    extern void freeConfigSection(void* data);
    extern void freeNothing(void* data);

    /* some much more general tools */
    extern void file_error(char* filename);
    extern void errorOccur(const char* msg);
}

#endif