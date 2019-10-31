#ifndef ast_compile_hpp
#define ast_compile_hpp

#include <string>
#include <vector>
#include <iostream>
#include <map>

#include "ast.hpp"


/*                  $0     : fixed 0
                    $2-3   : Return values
                    $4-7   : Argument registers
                    $8-15  : Temporary registers
                    $16-23 : Saved Registers
                    $sp    : $29, stack pointer
                    $fp    : $30, frame pointer
                    $31    : return address   

                    Other registers currently not used

                    ==========Assuming input is well-formed C code===============             
*/
 

static int ArgNo=0;                         //number of args in function
static int DecNo=0;                         //number of declared variable in function
static int SavedNo=0;                       //number of volatile registers
static int LocalTempNo=0;                   //number of temp local data
static int RetNo=2;                         //return address+old frame pointer

static int GlobalNo=0;                      //number of global variables(negative count)
static std::vector<std::string> globVec;    //a vector containing global variable names, indexed by (-GlobalNo-1)

//All above static int counts will increase by 1 on corresponding instantiation

static int PadNo(){         //pad stack frame size to be multiple of 8

    return (DecNo+ArgNo+SavedNo+RetNo+LocalTempNo) % 2 == 0 ? 0 :1;  
    
}

static int fSize(){        //frame size
    if(DecNo+ArgNo+SavedNo+RetNo+LocalTempNo == 1){ return 2; }
    return DecNo+ArgNo+SavedNo+RetNo+LocalTempNo+PadNo();
}     

 
//where is the variable in the stack?
//increment on declaration
static int curLevel=0;      

//bind declared variable name with its position in the stack 
static std::map<std::string, int> bindings;     
static std::pair<std::string, int> makeBind(std::string name, int stackNo){
    return std::make_pair(name, stackNo);
}

//bind each temp variable with a unique name
static int makeNameUnique=0;
static std::string makeName(std::string name_in){
    makeNameUnique++;
    return name_in+std::to_string(makeNameUnique);
}



#endif
