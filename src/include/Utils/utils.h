#ifndef UTILS_H
#define UTILS_H
#include <string>
#include <iostream>
#include <exception>
#define TWO_PI 6.2831853071795864769252866

#define LOG(info) log__(__FILE__, __func__, __LINE__, info)

inline void log__(std::string file_name, std::string func_name, int line, const char* info) {
    std::string out = file_name + " " + func_name + ", line " + std::to_string(line) + ": ";
    std::cout << out << info << std::endl;
}

inline void print__() { std::cout << std::endl; };

/**
 * TODO change the output to a thread safe method
 * do not use cout to output with this method in concurrency environment
 * This is thread unsafety
 */
template<typename T, typename... Types>
void print__(const T& firstArg, const Types&... args) {
    std::cout << firstArg << " ";
    print__(args...);
}

#define PRINT(...) print__(__VA_ARGS__)

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
    extern int charToInt(const char* data);
    extern float charToFloat(const char* data);
    extern char* constCharToChar(const char* data);
    // From http://en.wikipedia.org/wiki/Box%E2%80%93Muller_transform
    extern float randNormal();
    // Generate random numbers evenly distributed in (min, Max) interval
    extern float randUniform(float min, float max);

}

/*
    customized exception class.
*/
class NetworkException: public std::exception{
public:
    NetworkException(): message_("Error."){}
    NetworkException(const char* msg): message_(msg){message_ = std::string("Error: ") + message_;}
    ~NetworkException() throw () {}
    virtual const char* what() const throw() {
        return message_.c_str();
    }
private:
    std::string message_;
};

#endif