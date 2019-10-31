#ifndef ast_hpp
#define ast_hpp

#include<fstream>
#include<string>
#include<vector>
#include<iostream>

#include "ast_node.hpp"
#include "ast_declaration.hpp"
#include "ast_expression.hpp"
#include "ast_statement.hpp"
#include "ast_context.hpp"

extern const Node* parseAST();


#endif
