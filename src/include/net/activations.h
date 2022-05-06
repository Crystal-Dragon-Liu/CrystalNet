#ifndef ACTIVATION_H
#define ACTIVATION_H
#include <math.h>

#include <string>
enum class ACTIVATION{
    LOGISTIC, 
    RELU,
    RELIE,
    LINEAR,
    RAMP,
    TANH,
    PLSE,
    LEAKY,
    ELU,
    LOGGY,
    STAIR,
    HARDTAN,
    LHTAN
};

namespace ACT_OP{
    /* @brief get the function enum class through funcName.*/
    extern ACTIVATION   getActivation(const char* funcName);
     /* @brief get the function name  through ACTIVATION.*/
    extern std::string  getActivationStr(ACTIVATION act);
    /* @brief return the result of activation through VALUE passed by users and the ACTIVATION enum */
    extern float        activate(float input, ACTIVATION act);

}


#endif