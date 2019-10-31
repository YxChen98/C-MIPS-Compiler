#ifndef ast_statement_hpp
#define ast_statement_hpp


class Statement : public Node
{
public:
  //On entering new scope:
/*  virtual void indentation(std::ostream &dst) const override{
    for(int i=0;i<scopes;i++){ dst<<"\t"; }
  }
*/
};

class DeclarationList : public Node
{
private:
  NodePtr left;
  NodePtr right;
public:
  DeclarationList(NodePtr _left,NodePtr _right) : left(_left),right(_right){}
  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    left->translate(dst);
    dst<<std::endl;
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    right->translate(dst);
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
    right->compile(dst);
  }

};

class EmptyCompoundStatement : public Statement
{
private:
   std::string Lcurly;
   std::string Rcurly;
public:
  EmptyCompoundStatement(): Lcurly("}"), Rcurly("}") {}
  virtual void translate(std::ostream &dst) const override
  {
    scopes++;
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<std::endl;
    scopes--;
  }

  virtual void compile(std::ostream& dst) const override
  {
    dst<<"        nop"<<std::endl;
  }
};


class UnaryCompoundStatement : public Statement
{
private:
  NodePtr left;
public:
  UnaryCompoundStatement(NodePtr _left) : left(_left) {}

  virtual void translate(std::ostream &dst) const override
  {
    scopes++;
    for(int i=0;i<scopes;i++){ dst<<""; }
    left->translate(dst);
    dst<<std::endl;
    scopes--;
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
  }

};

class BinaryCompoundStatement : public Statement
{
private:
  NodePtr left;
  NodePtr right;
public:
  BinaryCompoundStatement(NodePtr _left,NodePtr _right) : left(_left),right(_right) {}

  virtual void translate(std::ostream &dst) const override
  {
    scopes++;
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    left->translate(dst);
    dst<<std::endl;
    //for(int i=0;i<scopes;i++){ dst<<"\t"; }
    right->translate(dst);
    scopes--;
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
    right->compile(dst);
  }
};

class StatementList : public Statement
{
private:
  NodePtr left;
  NodePtr right;
public:
  StatementList(NodePtr _left,NodePtr _right) : left(_left),right(_right) {}

  virtual void translate(std::ostream &dst) const override
  {
    left->translate(dst);
    dst<<std::endl;
    right->translate(dst);
    dst<<std::endl;
    
  }

  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
    right->compile(dst);
  }
};


class EmptyExpressionStatement: public Statement
{
public:
  virtual void translate(std::ostream &dst) const override
  {
    dst<<std::endl;
  }

  virtual void compile(std::ostream& dst)const override
  {
    dst<<"        nop";
    dst<<std::endl;
  }
};

class UnaryExpressionStatement : public Statement
{
private:
  NodePtr left;
public:
  UnaryExpressionStatement(NodePtr _left) : left(_left) {}

  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    left->translate(dst);
    dst<<std::endl;
  }
  
  virtual void compile(std::ostream& dst) const override
  {
    left->compile(dst);
  }
};

class IfStatement : public Statement
{
private:
  NodePtr condition;
  NodePtr stat;
public:
  IfStatement(NodePtr _condition,NodePtr _stat) : condition(_condition),stat(_stat) {}

  NodePtr getCondition() const
  { return condition; }

  NodePtr getStat() const
  { return stat; }

  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"if ";
    condition->translate(dst);
    dst<<" :";
    dst<<std::endl;
    stat->translate(dst);
    scopes--;
  }

  virtual void compile(std::ostream& dst) const override
  {
      //a comparison expression, <, >, ==
      condition->compile(dst);

      //compile comparison expressions, and compare with 0
      //if zero, false, continue with following instructions
      dst<<"        beq     $2,$0,$L3";
      dst<<"                        #equal 0 meaning false"<<std::endl;
      dst<<"        nop"<<std::endl;
      dst<<std::endl;

      dst<<"        #if branch"<<std::endl;
      stat->compile(dst);

      dst<<std::endl;
      dst<<"$L3:    #outside if branch"<<std::endl;

  }
    
};

class IfElseStatement : public Statement
{
private:
  NodePtr condition;
  NodePtr stat1;
  NodePtr stat2;
public:
  IfElseStatement(NodePtr _condition,NodePtr _stat1,NodePtr _stat2) : condition(_condition),stat1(_stat1),stat2(_stat2) {}

  NodePtr getCondition() const
  { return condition; }

  NodePtr getStat1() const
  { return stat1; }

  NodePtr getStat2() const
  { return stat2; }

  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"if ";
    condition->translate(dst);
    dst<<" :";
    stat1->translate(dst);
    scopes--;
    dst<<std::endl;
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"else:";
    stat2->translate(dst);
    scopes--;
  }

  virtual void compile(std::ostream& dst)const override
  {
      //a comparison expression, <, >, ==
      condition->compile(dst);

      //compile comparison expressions, and compare with 0
      //if zero, false, continue with following instructions
      dst<<"        beq     $2,$0,$L2";
      dst<<"                        #equal 0 meaning false"<<std::endl;
      dst<<"        nop"<<std::endl;
      dst<<std::endl;

      dst<<"        #true branch"<<std::endl;
      stat1->compile(dst);

      dst<<"        b       $L4";
      dst<<"                              #continue following code"<<std::endl;
      dst<<"        nop"<<std::endl;

      dst<<std::endl;
      dst<<"$L2:    #false branch"<<std::endl;
      stat2->compile(dst);

      dst<<std::endl;
      dst<<"$L4:    #continue following code"<<std::endl;
  }
};

class WhileStatement : public Statement
{
private:
  NodePtr condition;
  NodePtr stat;
public:
  WhileStatement(NodePtr _condition,NodePtr _stat) : condition(_condition),stat(_stat) {}

  NodePtr getCondition() const
  { return condition; }

  NodePtr getStat() const
  { return stat; }

  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"while ";
    condition->translate(dst);
    dst<<" :";
    stat->translate(dst);
    scopes--;
  }

  virtual void compile(std::ostream& dst) const override
  {
    dst<<"        b       $L2"<<std::endl;
    dst<<"        nop"<<std::endl;
    dst<<std::endl;

    dst<<"$L3:    #operations inside while loop"<<std::endl;
    stat->compile(dst);
    dst<<std::endl;

    dst<<"$L2:    #check condition"<<std::endl;
    condition->compile(dst);
    dst<<"        bne     $2,$0,$L3";
    dst<<"                        #not equal 0 meaning true"<<std::endl;
    dst<<"        nop"<<std::endl;
    dst<<std::endl;

    dst<<"        #continue following code"<<std::endl;
  }
    
};

class EmptyReturnStatement : public Statement
{
public:
  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"return ";
  }

  virtual void compile(std::ostream& dst) const override
  {
    dst<<"        j       $31"<<std::endl;
    dst<<"        nop"<<std::endl;
  }


};

class UnaryReturnStatement : public Statement
{
private:
  NodePtr right;
public:
  UnaryReturnStatement(NodePtr _right) : right(_right) {}

  virtual void translate(std::ostream &dst) const override
  {
    for(int i=0;i<scopes;i++){ dst<<"    "; }
    dst<<"return ";
    right->translate(dst);
  }

  virtual void compile(std::ostream& dst) const override
  {
    //return 0;
    if(right->getValue()==0){
      dst<<"        move    $2,$0";
      dst<<"                            #return value"<<std::endl;
    }

    //return (some variable);
    else if(right->expressionType()=="Variable"){
  
      //global variable
      if(bindings[right->getId()]<0){
          dst<<"        lw      $2,"<<"%got("<<right->getId()<<")($28)"<<std::endl;
          dst<<"        nop"<<std::endl;
          dst<<"        lw      $2,0($2)"<<std::endl;
      }

      //local variable
      else{
          dst<<"        lw      $2,"<<4*(1+bindings[right->getId()])<<"($fp)";
          dst<<"                        #return value"<<std::endl;
      }
      
    }

    //return (binary expression);
    else if(right->expressionType()=="Binary"){
      right->compile(dst);
      dst<<"        #return at $2"<<std::endl;
    }

    //return (some constant);
    else if(right->getValue()!=0){
      dst<<"        li      $2,";
      right->compile(dst);
      dst<<"                             #return value"<<std::endl; 
    }

  }

};

#endif
