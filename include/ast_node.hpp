#ifndef ast_node_hpp
#define ast_node_hpp

#include <string>
#include <iostream>
#include <vector>
#include "ast_context.hpp"

static int scopes=0;
class Node;

typedef const Node *NodePtr;

class Node
{
public:
    virtual ~Node()
    {}

    virtual void translate(std::ostream &dst) const {};
    virtual void compile(std::ostream &dst) const {};

    //tag funcitons, not using bool, as bool can only take 2 values
    //so there will be a 50% error
    virtual std::string getId() const{};
    virtual std::string expressionType() const{};
    virtual std::string functionType() const{};

    virtual int getValue() const{};
    virtual NodePtr getLeft() const{};
    virtual NodePtr getRight() const{};


};


#endif
