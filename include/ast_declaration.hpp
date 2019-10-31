#ifndef ast_declaration_hpp
#define ast_declaration_hpp

#include "ast_node.hpp"
#include "ast_expression.hpp"
#include "ast_context.hpp"

class TranslationUnit : public Node
{
protected:
  NodePtr left;
  NodePtr right;
public:
  TranslationUnit(NodePtr _left,NodePtr _right) : left(_left),right(_right){}
  NodePtr getLeft() const
  { return left; }

  NodePtr getRight() const
  { return right; }

  virtual void translate(std::ostream &dst) const override
  {
    left->translate(dst);
    right->translate(dst);
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
    right->compile(dst);
  }

};

class FunctionDef : public Node
{
protected:
  NodePtr Left;
public:
  FunctionDef(NodePtr _Left) : Left(_Left) {}

  virtual NodePtr getLeft() const override
  { return Left; }

  virtual void translate(std::ostream& dst) const override
  {
    Left->translate(dst);
  }

  virtual void compile(std::ostream& dst) const override
  {
    static int funcCount=0;
    static int globCount=0;
    static int globAssignCount=0;

    if(Left->functionType()=="Function"){
        funcCount++;
        //only emit for the first function definition
        if(funcCount == 1){
            dst<<"        .text"<<std::endl;
        }

        dst<<"        .align  2"<<std::endl;
        dst<<"        .globl  "<<Left->getLeft()->getLeft()->getId()<<std::endl;
        dst<<"        .set    nomips16"<<std::endl;
        dst<<"        .set    nomicromips"<<std::endl;
        dst<<"        .ent    "<<Left->getLeft()->getLeft()->getId()<<std::endl;
        dst<<"        .type   "<<Left->getLeft()->getLeft()->getId()<<", @function"<<std::endl;
        dst<<std::endl;

        //label function
        Left->getLeft()->compile(dst);
        dst<<":";
        dst<<std::endl;

        //llvm settings
        dst<<"        .frame  $fp,"<<4*(fSize())<<",$31"<<std::endl;
        dst<<"        .mask   0x40000000,-4"<<std::endl;
        dst<<"        .fmask  0x00000000,0"<<std::endl;
        dst<<"        .set    noreorder"<<std::endl;
        if(GlobalNo<0){
            dst<<"        .cpload $25"<<std::endl;
        }
        dst<<"        .set    nomacro"<<std::endl;

        Left->compile(dst);

        //llvm settings
        dst<<"        .set    macro"<<std::endl;
        dst<<"        .set    reorder"<<std::endl;
        dst<<"        .end    ";
        Left->getLeft()->compile(dst);
        dst<<std::endl;
        dst<<"        .size   ";
        Left->getLeft()->compile(dst);
        dst<<", .-";
        Left->getLeft()->compile(dst);
        dst<<std::endl;
    }

    else if(Left->functionType() == "Global"){
        globCount++;

        dst<<"        .globl  "<<Left->getId()<<std::endl;
 
        //only emit on first global variable
        if(globCount==1){
            dst<<"        .section        .bss,"<<'"'<<"aw"<<'"'<<",@nobits"<<std::endl;
        }

        dst<<"        .align  2"<<std::endl;
        dst<<"        .type   "<<Left->getId()<<", @object"<<std::endl;
        dst<<"        .size   "<<Left->getId()<<", 4"<<std::endl;
        dst<<std::endl;

        dst<<Left->getId()<<":"<<std::endl;
        dst<<"        #not explicitly assigned global variable"<<std::endl;
        dst<<"        #default 0"<<std::endl;
        dst<<"        .space  4"<<std::endl;
    }

    else if(Left->functionType() == "GlobalAssign"){
        globAssignCount++;

        dst<<"        .globl  "<<Left->getId()<<std::endl;

        //only emit on first global assign variable
        if(globAssignCount==1){
            dst<<"        .data"<<std::endl;
        }

        dst<<"        .align  2"<<std::endl;
        dst<<"        .type   "<<Left->getId()<<", @object"<<std::endl;
        dst<<"        .size   "<<Left->getId()<<", 4"<<std::endl;
        dst<<std::endl;


        dst<<Left->getId()<<":"<<std::endl;
        dst<<"        #explicitly assigned global variable"<<std::endl;;
        dst<<"        .word   "<<Left->getRight()->getValue()<<std::endl;
    }
  }
};

class AssignParameterDeclaraiton : public Node
{
private:
  NodePtr VarName;
  NodePtr Value;
public:
  AssignParameterDeclaraiton(NodePtr _VarName,NodePtr _Value) : VarName(_VarName),Value(_Value)
  { DecNo++; }

  virtual void translate(std::ostream &dst) const override
  {
    VarName->translate(dst);
    dst<<"=";
    Value->translate(dst);
  }

  //this class is used for assignment on declaration
  //for assigning already declared variable, another class in expression.hpp is used
  virtual void compile(std::ostream& dst) const override
  {
      curLevel++;
      bindings.insert(makeBind(VarName->getId(), curLevel));

      //assign zero
      if(Value->getValue()==0){
          dst<<"        sw      $0,"<<4*(curLevel+1)<<"($fp)";
          dst<<"                       #local declaration"<<std::endl;
      }

      //assign binary expressions
      else if(Value->expressionType()=="Binary"){
          Value->compile(dst);
          dst<<"        sw      $2,"<<4*(curLevel+1)<<"($fp)";
          dst<<"                       #assign declaration"<<std::endl;
      }

      //assign variable
      else if(Value->expressionType()=="Variable"){
          dst<<"        lw      $2,"<<4*(1+bindings[Value->getId()])<<"($fp)"<<std::endl;
          dst<<"        nop"<<std::endl;
          dst<<"        sw      $2,"<<4*(curLevel+1)<<"($fp)";
          dst<<"                       #assign declaration"<<std::endl;
      }

      //assign other constants
      else if(Value->getValue()!=0){
          dst<<"        li      $2,"<<Value->getValue()<<std::endl;
          dst<<"        sw      $2,"<<4*(curLevel+1)<<"($fp)";
          dst<<"                       #assign declaration"<<std::endl;
      }

  }



};

class EmptyFunctionCall : public Expression
{
private:
    NodePtr left;
public:
    EmptyFunctionCall(NodePtr _left)
        : left(_left)
    {}

    virtual NodePtr getLeft() const override
    { return left; }

    virtual void translate(std::ostream &dst) const override
    {
        
        left->translate(dst);
	dst<<"()";
    }

    virtual void compile(std::ostream& dst) const override
    {      
        left->compile(dst);
    }
};

class FunctionCall : public Node
{
private:
  NodePtr FuncName;
  NodePtr ParaName;
public:
  FunctionCall(NodePtr _FuncName,NodePtr _ParaName) : FuncName(_FuncName),ParaName(_ParaName){}
  virtual void translate(std::ostream &dst) const override
  {
    FuncName->translate(dst);
    dst<<"(";
    ParaName->translate(dst);
    dst<<")";
  }

  virtual void compile(std::ostream& dst) const override
  {
    FuncName->compile(dst);
  }

};

class ParaDec : public Node
{
private:
  std::string Val;
public:
  ParaDec(std::string _Val) : Val(_Val){ 
      ArgNo++; 
  }

  virtual void translate(std::ostream &dst) const override
  {
    dst<<Val;
  }

  virtual void compile(std::ostream& dst) const override
  {
    static int count=0;
    bindings.insert(makeBind(Val, fSize()-1+count));
    count++;
  }

};

class ParaDecList : public Node
{
private:
  NodePtr left;
  NodePtr right;
public:
  ParaDecList(NodePtr _left,NodePtr _right) : left(_left),right(_right){}
  virtual void translate(std::ostream &dst) const override
  {
    left->translate(dst);
    dst<<",";
    right->translate(dst);
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
    right->compile(dst);
  }

};

class IntDecList : public Node
{
private:
  NodePtr VarName;
public:
  IntDecList(NodePtr _VarName):VarName(_VarName)
  { DecNo++; }

  virtual void translate(std::ostream &dst) const override
  {
    //for(int i=0;i<scopes;i++){ dst<<"\t"; }
    VarName->translate(dst);
    dst<<std::endl;
  }

  virtual void compile(std::ostream& dst) const override
  {
    VarName->compile(dst);
  }
};

class IdentifierDec :public Node
{
private:
  std::string Val;
public:
  IdentifierDec(const std::string& _Val):Val(_Val) {}
  virtual void translate(std::ostream& dst) const override
  {
    dst<<Val;
    dst<<"=0";
  }

  virtual void compile(std::ostream& dst) const override
  {
    curLevel++;
    bindings.insert(makeBind(Val, curLevel));

    dst<<"        sw      $0,"<<4*(1+curLevel)<<"($fp)";
    dst<<"                       #local declaration"<<std::endl;
  }

};


class IdentifierDecList : public Node
{
private:
    NodePtr id_l;
    std::string id_r;
public:
    IdentifierDecList(NodePtr _id_l,const std::string &_id_r)
        : id_l(_id_l),id_r(_id_r)
    {}

    virtual void translate(std::ostream &dst) const override
    {
      id_l->translate(dst);
      dst<<",";
      dst<<id_r;
      dst<<"=0";
    }

    virtual void compile(std::ostream& dst) const override
    {
      id_l->compile(dst);

      curLevel++;
      bindings.insert(makeBind(id_r, curLevel));

      dst<<"        move    $2,$0"<<std::endl;   //treat non-assignment declaration to be zero
      dst<<"        sw      $2,"<<4*(1+curLevel)<<"($fp)"<<std::endl;

    }
      
};

class AssignDecList : public Node
{
private:
    NodePtr left;
    NodePtr right;
public:
    AssignDecList(NodePtr _left, NodePtr _right) : left(_left), right(_right) {}

    virtual void translate(std::ostream& dst) const override
    {
        left->translate(dst);
        dst<<",";
        right->translate(dst);
    }

    virtual void compile(std::ostream& dst) const override
    {
        left->compile(dst);
        right->compile(dst);
    }
};

class FuncName: public Node
{
private:
    NodePtr name;
    NodePtr param;
public:
    FuncName(NodePtr _name, NodePtr _param) : name(_name), param(_param) {}

    virtual NodePtr getLeft() const override
    { return name; }

    virtual void translate(std::ostream& dst)const override
    {
        name->translate(dst);
        dst<<"(";
        param->translate(dst);
        dst<<")";
    }

    virtual void compile(std::ostream& dst) const override
    {
        name->compile(dst);
        param->compile(dst);
    }
};

class IntFunctionDec : public Node
{
private:
    NodePtr dec;
    NodePtr stm;
public:
    IntFunctionDec(NodePtr _dec, NodePtr _stm) : dec(_dec), stm(_stm) {}
    
    virtual NodePtr getLeft() const override
    { return dec; }

    virtual std::string functionType() const override
    { return "Function"; }

    virtual void translate(std::ostream& dst) const override
    {
        dst<<"def ";
        dec->translate(dst);
        dst<<":";
        dst<<std::endl;

        for(int i=0; i>GlobalNo; i--){
            dst<<"    global "<<globVec[-i]<<std::endl;
        }

        stm->translate(dst);
    }

    virtual void compile(std::ostream &dst) const override
    {
        //set up function
        dst<<std::endl;
        dst<<"        addiu   $sp,$sp,-"<<4*(fSize());
        dst<<"                      #push stack frame"<<std::endl;

        dst<<"        sw      $fp,"<<4*(fSize())-4<<"($sp)";
        dst<<"                      #store previous frame pointer"<<std::endl;

        dst<<"        move    $fp,$sp";
        dst<<"                          #new frame pointer"<<std::endl;

        //save arguments
        for(int i=0; i<ArgNo; i++){
            dst<<"        sw      $"<<4+i<<","<<4*(fSize()+i)<<"($fp)";
            dst<<"                       #argument"<<std::endl;
        }
 
        //return address for int main
        if( dec->getLeft()->getId() == "main"){
            dst<<"        movz    $31,$31,$0";
            dst<<"                       #main function return address"<<std::endl;
        }

        dst<<std::endl;
        dst<<"        #start function body"<<std::endl;

        //do the work
        stm->compile(dst);

        dst<<"        #end function body"<<std::endl;
        dst<<std::endl;

        //on exit
        dst<<"        move    $sp,$fp";
        dst<<"                          #restore base stack pointer"<<std::endl;

        dst<<"        lw      $fp,"<<4*(fSize())-4<<"($sp)";
        dst<<"                      #restore previous frame pointer"<<std::endl;

        dst<<"        addiu   $sp,$sp,"<<4*(fSize());
        dst<<"                       #pop stack frame"<<std::endl;

        dst<<"        j       $31";
        dst<<"                              #return"<<std::endl;
        dst<<"        nop"<<std::endl;
        dst<<std::endl;   

    }
};

class VoidFunctionDec : public Node
{
private:
    NodePtr dec;
    NodePtr stm;
public:
    VoidFunctionDec(NodePtr _dec, NodePtr _stm) : dec(_dec), stm(_stm) {}
    
    virtual std::string functionType() const override
    { return "Function"; }

    virtual NodePtr getLeft() const override
    { return dec; }

    virtual void translate(std::ostream& dst) const override
    {
        dst<<"def ";
        dec->translate(dst);
        dst<<":";
        dst<<std::endl;

        for(int i=0; i>GlobalNo; i--){
            dst<<"    global "<<globVec[-i]<<std::endl;
        }

        stm->translate(dst);
    }

    virtual void compile(std::ostream &dst) const override
    {

        //set up function
        dst<<"        addiu   $sp,$sp,-"<<4*(fSize())<<std::endl;
        dst<<"        sw      $fp,"<<4*(fSize())-4<<"($sp)"<<std::endl;
        dst<<"        move    $fp,$sp"<<std::endl;

        //global count is negative
        if(GlobalNo<0){
            dst<<"        .cprestore      0"<<std::endl;
        }

        //save arguments
        for(int i=0; i<ArgNo; i++){
            dst<<"        sw      $"<<4+i<<","<<4*(fSize()+i)<<"($fp)"<<std::endl;
        }     

        dst<<std::endl;
        dst<<"        #start function body"<<std::endl;

        //do the work
        stm->compile(dst);

        dst<<"        #end function body"<<std::endl;
        dst<<std::endl;

        //on exit
        dst<<"        nop"<<std::endl;
        dst<<"        move    $sp,$fp"<<std::endl;
        dst<<"        lw      $fp,"<<4*(fSize())-4<<"($sp)"<<std::endl;
        dst<<"        addiu   $sp,$sp,"<<4*(fSize())<<std::endl;
        dst<<"        j       $31"<<std::endl;
        dst<<"        nop"<<std::endl;
        dst<<std::endl;
    }
};

class GlobalVar : public Node
{
private:
    std::string name;
public:
    GlobalVar(std::string _name):name(_name) {
        //if a variable binds to a negative value, it is global
        GlobalNo--;
        bindings.insert(makeBind(name, GlobalNo));
        globVec.push_back(name);
    }
 
    virtual std::string functionType() const override
    { return "Global" ;}

    virtual std::string getId() const override
    { return name; }

    virtual void translate(std::ostream& dst) const override
    {
        dst<<name<<"=0"<<std::endl;
    }

    virtual void compile(std::ostream& dst) const override
    { 

    }
};

class GlobalAssignVar : public Node
{
private:
    std::string name;
    NodePtr val;
public:
    GlobalAssignVar(std::string _name, NodePtr _val):name(_name),val(_val) {
        //if a variable binds to a negative value, it is global
        GlobalNo--;
        bindings.insert(makeBind(name, GlobalNo));
    }
 
    virtual std::string functionType() const override
    { return "GlobalAssign" ;}

    virtual NodePtr getRight() const override
    { return val; }

    virtual std::string getId() const override
    { return name; }

    virtual void compile(std::ostream& dst) const override
    { 

    }
};


#endif


