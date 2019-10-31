#ifndef ast_expression_hpp
#define ast_expression_hpp

#include <string>
#include <fstream>
#include "ast_context.hpp"


class Expression : public Node
{

};

class Identifier : public Expression
{
private:
    std::string id;
public:
    Identifier(const std::string &_id)
        : id(_id)
    {}

    virtual std::string expressionType() const override
    { return "Variable"; }

    virtual std::string getId() const override
    { return id; }

    virtual void translate(std::ostream &dst) const override
    {
      dst<<id;
    }
 
    virtual void compile(std::ostream &dst) const override
    {
        dst<<id;
    }
};

class Number : public Expression
{
private:
    int value;
public:
    Number(int _value)
        : value(_value)
    {}

    virtual std::string expressionType() const override
    { return "Number"; }

    virtual int getValue() const override
    { return value; }

    virtual void translate(std::ostream &dst) const override
    {
      dst<<value;
    }

    virtual void compile(std::ostream& dst) const override
    {
      dst<<value;
    }
};

class BinaryOperator : public Expression
{
protected:
    NodePtr left;
    NodePtr right;

    BinaryOperator(NodePtr _left, NodePtr _right)
        : left(_left)
        , right(_right)
    {SavedNo++;}
public:
    virtual const char *getOpcode() const =0;

    virtual NodePtr getLeft() const override
    { return left; }

    virtual NodePtr getRight() const override
    { return right; }

    virtual std::string expressionType() const override {return "Binary";};

    virtual void translate(std::ostream &dst) const override
    {
        left->translate(dst);
        dst<<getOpcode();
        right->translate(dst);
    }

    virtual void compile(std::ostream& dst)const override
    {

///////////////////////////////plus operation////////////////////////////////////////////////

        if(std::string(getOpcode())=="+"){

            //adding with two variables
            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        addu    $2,$3,$2"<<std::endl;
            }

            //adding with one variable and one NON-ZERO constant
            //constant on the RIGHT!
            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }

                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        addiu   $2,$2,"<<right->getValue()<<std::endl;
            }
                
        }

/////////////////////////////////minus operation//////////////////////////////////////////

        else if(std::string(getOpcode())=="-"){
            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        subu    $2,$3,$2"<<std::endl;
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        addiu   $2,$2,-"<<right->getValue()<<std::endl;
            }
        }

///////////////////////////////////mult operation////////////////////////////////////////////

        else if(std::string(getOpcode())=="*"){
            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        mult    $3,$2"<<std::endl;
                dst<<"        mflo    $2"<<std::endl;
            }

            //by our default, when multiplying with a constant, 
            //save it to a temp register and do the same thing
            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){
                dst<<"        li      $8,"<<right->getValue()<<std::endl;

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $3,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        mult    $3,$8"<<std::endl;
                dst<<"        mflo    $2"<<std::endl;
            }
        }

/////////////////////////////////div operation////////////////////////////////////////////////

        else if(std::string(getOpcode())=="/"){
            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        div     $3,$2"<<std::endl;
                dst<<"        mflo    $2"<<std::endl;
            }
             
            //handle in the same way as mult
            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){
                dst<<"        li      $8,"<<right->getValue()<<std::endl;

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $3,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        div    $3,$8"<<std::endl;
                dst<<"        mflo    $2"<<std::endl;
            }
        }

///////////////////////////////////////mod operation//////////////////////////////////////////////

        else if(std::string(getOpcode())=="%"){

            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        div     $3,$2"<<std::endl;
                dst<<"        mfhi    $2"<<std::endl; 
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){
                dst<<"        li      $8,"<<right->getValue()<<std::endl;

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $3,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        div    $3,$8"<<std::endl;
                dst<<"        mfhi    $2"<<std::endl;
            }
        }

/////////////////////////////////bitwise or operation/////////////////////////////////////////
        else if(std::string(getOpcode())=="|"){

            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        or      $2,$3,$2"<<std::endl;
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        ori     $2,$2,"<<right->getValue()<<std::endl;
            }

        }

////////////////////////////////bitwise and operation/////////////////////////////////////////////

        else if(std::string(getOpcode())=="&"){

            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        and     $2,$3,$2"<<std::endl;
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        andi    $2,$2,"<<right->getValue()<<std::endl;
            }

        }

////////////////////////////bitwise xor operation////////////////////////////////////////////

        else if(std::string(getOpcode())=="^"){

            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        xor     $2,$3,$2"<<std::endl;
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        xori    $2,$2,"<<right->getValue()<<std::endl;
            }
        }

/////////////////////////////////////less than operation//////////////////////////////////////

        else if(std::string(getOpcode())=="<"){

           if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                    dst<<"                  #compare if less than"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)";
                    dst<<"                       #compare if less than"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }                

                dst<<"        nop"<<std::endl;
                dst<<"        slt     $2,$3,$2"<<std::endl;
                dst<<"        andi    $2,$2,0x00ff";
                dst<<"                     #end compare if less than"<<std::endl;
           }

           else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                    dst<<"                   #compare if less than"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)";
                    dst<<"                        #compare if less than"<<std::endl;
                }

                dst<<"        nop"<<std::endl;
                dst<<"        slti    $2,$2,"<<right->getValue()<<std::endl;
                dst<<"        andi    $2,$2,0x00ff";
                dst<<"                     #end compare if less than"<<std::endl;
           }

        }

////////////////////////////////////greater than operation/////////////////////////////////////

        else if(std::string(getOpcode())==">"){

           if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

                //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                    dst<<"                   #compare if greater than"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)";
                    dst<<"                        #compare if greater than"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }        

               dst<<"        nop"<<std::endl;
               dst<<"        slt     $2,$2,$3"<<std::endl;
               dst<<"        andi    $2,$2,0x00ff";
               dst<<"                     #end compare if greater than"<<std::endl;
           }

           else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){
              //left variable is global
              if(bindings[left->getId()] < 0){
                  dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                  dst<<"                   #compare if greater than"<<std::endl;
                  dst<<"        nop"<<std::endl;
                  dst<<"        lw      $2,0($2)"<<std::endl;
              }
              //left variable is local
              else{
                  dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)";
                  dst<<"                        #compare if greater than"<<std::endl;
              }

              dst<<"        nop"<<std::endl;
              dst<<"        slti    $2,$2,"<<right->getValue()<<std::endl;
              dst<<"        andi    $2,$2,0x00ff";
              dst<<"                     #end compare if greater than"<<std::endl;
           }

        }

//////////////////////////////////equal operation/////////////////////////////////////////////
        else if(std::string(getOpcode())=="=="){
            
            if(left->expressionType()=="Variable" && right->expressionType()=="Variable"){

               //left variable is global
                if(bindings[left->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                    dst<<"                  #compare if equal to"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                }
                //left variable is local
                else{
                    dst<<"        lw      $3,"<<4*(1+bindings[left->getId()])<<"($fp)";
                    dst<<"                       #compare if equal to"<<std::endl;
                }

                //right variable is global
                if(bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                }
                //right variable is local
                else{
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                }

               dst<<"        nop"<<std::endl;
               dst<<"        xor     $2,$3,$2"<<std::endl;
               dst<<"        sltiu   $2,$2,1"<<std::endl;
               dst<<"        andi    $2,$2,0x00ff";
               dst<<"                     #end compare if equal to"<<std::endl;
            }

            else if(left->expressionType()=="Variable" && right->expressionType()=="Number"){
               //left variable is global
               if(bindings[left->getId()] < 0){
                  dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)";
                  dst<<"                   #compare if equal to"<<std::endl;
                  dst<<"        nop"<<std::endl;
                  dst<<"        lw      $2,0($2)"<<std::endl;
               }
               //left variable is local
               else{
                  dst<<"        lw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)";
                  dst<<"                        #compare if equal to"<<std::endl;
               }

               dst<<"        nop"<<std::endl;
               dst<<"        xori    $2,$2,"<<right->getValue()<<std::endl;
               dst<<"        sltiu   $2,$2,1"<<std::endl;
               dst<<"        andi    $2,$2,0x00ff";
               dst<<"                     #end compare if equal to"<<std::endl;
            }
        }

////////////////////////////logical or operation///////////////////////////////////
        else if(std::string(getOpcode())=="||"){

            left->compile(dst);
            //save left result to temp register
            dst<<"        move    $9,$2";
            dst<<"                            #save left to temp"<<std::endl;

            right->compile(dst);
            //save right result to temp register
            dst<<"        move    $10,$2";
            dst<<"                           #save left to temp"<<std::endl;

            dst<<"        or      $2,$9,$10"<<std::endl;

        }

//////////////////////////logical and operation////////////////////////////////////
        else if(std::string(getOpcode())=="&&"){
        
            left->compile(dst);
            dst<<"        move    $9,$2";
            dst<<"                            #save left to temp"<<std::endl;

            right->compile(dst);
            dst<<"        move    $10,$2";
            dst<<"                           #save left to temp"<<std::endl;

            dst<<"        and     $2,$9,$10"<<std::endl;
        }

//////////////////////////////assignment operation///////////////////////////////
        //this assignment is only used when variables are declared!!
        //assignment on declaration used in another class in declaration.hpp!!
        else if(std::string(getOpcode())=="="){

            //assign with zero 
            //a=0
            if(right->getValue()==0){

                //assign global variable with 0
                if(bindings[left->getId()]<0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw     $0,0($2)"<<std::endl;
                }
                //assign local variable with 0
                else{
                    dst<<"        sw      $0,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }
            }

            //assign variable with a binary expression
            //a=b+c
            else if(right->expressionType()=="Binary"){
                right->compile(dst);
                //left is global
                if(bindings[left->getId()]<0){
                    dst<<"        move    $3,$2"<<std::endl;
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw      $3,0($2)"<<std::endl;
                }
                //left is local
                else{
                    dst<<"        sw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }
            }
  
            //assign with a variable
            //a=b
            else if(right->expressionType()=="Variable"){
                //both local
                if(bindings[left->getId()] > 0 &&
                   bindings[right->getId()]>0){
                    dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //left global, right local 
                else if(bindings[left->getId()] < 0 &&
                   bindings[right->getId()] > 0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        lw      $3,"<<4*(1+bindings[right->getId()])<<"($fp)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw      $3,0($2)"<<std::endl;
                }

                //left local, right global 
                else if(bindings[left->getId()] > 0 &&
                   bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $2,0($2)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }

                //both global
                else if(bindings[left->getId()] < 0 &&
                   bindings[right->getId()] < 0){
                    dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        lw      $3,0($2)"<<std::endl;
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        nop"<<std::endl;
                    dst<<"        sw      $3,0($2)"<<std::endl;
                }
            }

            //assign with non-zero constants
            else if(right->getValue()!=0){
                if(bindings[left->getId()]<0){
                    dst<<"        lw      $2,"<<"%got("<<left->getId()<<")($28)"<<std::endl;
                    dst<<"        li      $3,"<<right->getValue()<<std::endl;
                    dst<<"        sw      $3,0($2)"<<std::endl;
                }
                else{
                    dst<<"        li      $2,"<<right->getValue()<<std::endl;
                    dst<<"        sw      $2,"<<4*(1+bindings[left->getId()])<<"($fp)"<<std::endl;
                }
            }
        }

    }
};

class ArgumentListExpression : public Expression
{
protected:
    NodePtr left;
    NodePtr right;


public:
    ArgumentListExpression(NodePtr _left, NodePtr _right)
        : left(_left)
        , right(_right)
    {}

    virtual void translate(std::ostream &dst) const override
    {
        left->translate(dst);
        dst<<",";
        right->translate(dst);
    }

    virtual void compile(std::ostream& dst)const override
    {
        left->compile(dst);
        right->compile(dst);
    }
};

class IncPostfixOperator : public Expression
{
protected:
    NodePtr right;

public:
     IncPostfixOperator(NodePtr _right)
        : right(_right)
    {}

    virtual void translate(std::ostream &dst) const override
    {        
        right->translate(dst);
	dst<<"++";
    }
};


class DecPostfixOperator : public Expression
{
protected:
    NodePtr right;

public:
    DecPostfixOperator(NodePtr _right)
        : right(_right)
    {}
    virtual void translate(std::ostream &dst) const override
    {        
        right->translate(dst);
	dst<<"--";
    }
};

class NumberList : public Expression
{
private:
    NodePtr left;
    int right;
public:
    NumberList(NodePtr _left,int _right)
        : left(_left),right(_right)
    {}

    virtual void translate(std::ostream &dst) const override
    {
      left->translate(dst);
      dst<<",";
      dst<<right;
    }
};


class UnaryOperator : public Expression
{
protected:
    NodePtr left;

    UnaryOperator(NodePtr _left)
        : left(_left)
    {}
public:
    virtual const char *getOpcode() const =0;

    NodePtr getLeft() const
    { return left; }

    virtual void translate(std::ostream &dst) const override
    {
        dst<<getOpcode();
        left->translate(dst);
    }
};

class IncOperator : public UnaryOperator
{
protected:
  virtual const char*getOpcode() const override
  { return "++"; }
public:
  IncOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class DecOperator : public UnaryOperator
{
protected:
  virtual const char* getOpcode() const override
  { return "--"; }
public:
  DecOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class PositiveOperator : public UnaryOperator
{
protected:
  virtual const char* getOpcode() const override
  { return "+"; }
public:
  PositiveOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class NegativeOperator : public UnaryOperator
{
protected:
  virtual const char* getOpcode() const override
  { return "-"; }
public:
  NegativeOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class BitwiseNotOperator : public UnaryOperator
{
protected:
  virtual const char* getOpcode() const override
  { return "~"; }
public:
  BitwiseNotOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class LogicalNotOperator : public UnaryOperator
{
protected:
  virtual const char* getOpcode() const override
  { return "!"; }
public:
  LogicalNotOperator(NodePtr _left) : UnaryOperator(_left) {}

};

class TimesOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "*"; }
public:
    TimesOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class DivideOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "/"; }
public:
    DivideOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class ModOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "%"; }
public:
    ModOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class AddOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "+"; }
public:
    AddOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class MinusOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "-"; }
public:
    MinusOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LshiftOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "<<"; }
public:
    LshiftOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class RshiftOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return ">>"; }
public:
    RshiftOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LessthanOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "<"; }
public:
    LessthanOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class GreaterthanOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return ">"; }
public:
   GreaterthanOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class GreaterequalOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return ">="; }
public:
    GreaterequalOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LessequalOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "<="; }
public:
    LessequalOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class EqualOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "=="; }
public:
    EqualOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class NotequalOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "!="; }
public:
    NotequalOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class BandOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "&"; }
public:
    BandOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class XorOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "^"; }
public:
    XorOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class BorOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "|"; }
public:
    BorOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LandOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "&&"; }
public:
    LandOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LorOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "||"; }
public:
    LorOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class AssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "="; }
public:
    AssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class TimesAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "*="; }
public:
    TimesAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class DivideAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "/="; }
public:
    DivideAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class ModAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "%="; }
public:
    ModAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class PlusAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "+="; }
public:
    PlusAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class MinusAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "-="; }
public:
    MinusAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class LshiftAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "<<="; }
public:
    LshiftAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class RshiftAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return ">>="; }
public:
    RshiftAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class AndAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "&="; }
public:
    AndAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class XorAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "^="; }
public:
    XorAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

class OrAssignOperator : public BinaryOperator
{
protected:
    virtual const char *getOpcode() const override
    { return "|="; }
public:
   OrAssignOperator(NodePtr _left, NodePtr _right) : BinaryOperator(_left, _right) {}

};

#endif
