%code requires{
  #include "../include/ast.hpp"

  #include <cassert>
  #include <string>

  extern const Node* g_root; // A way of getting the AST out

  //! This is to fix problems when generating C++
  // We are declaring the functions provided by Flex, so
  // that Bison generated code can call them.
  int yylex(void);
  void yyerror(const char *);
}

// Represents the value associated with any kind of
// AST node.
%union{
  const Node* node;
  double number;
  std::string* string;
}

%token IDENTIFIER STRING_LITERAL T_NUMBER
%token T_PLUS T_MINUS T_TIMES T_DIVIDE T_MOD T_GT T_LT T_BAND T_BOR T_XOR T_BNOT T_LNOT T_QUESTION
%token T_COLON T_SEMICOLON T_COMMA T_LCURLY T_RCURLY T_LBRACKET T_RBRACKET
%token T_INC T_DEC T_LSHIFT T_RSHIFT T_LE T_GE T_EQ T_NE T_LAND T_LOR
%token T_ASSIGN T_TIMES_ASSIGN T_DIVIDE_ASSIGN T_MOD_ASSIGN T_PLUS_ASSIGN T_MINUS_ASSIGN T_LSHIFT_ASSIGN T_RSHIFT_ASSIGN T_AND_ASSIGN T_XOR_ASSIGN T_OR_ASSIGN
%token S_INT S_IF S_ELSE S_WHILE S_FOR S_RETURN S_DO S_VOID


%type <node> PRIMARY_EXPRESSION POSTFIX_EXPRESSION ARGUMENT_EXPRESSION_LIST UNARY_EXPRESSION
             MULTIPLICATIVE_EXPRESSION ADDITIVE_EXPRESSION SHIFT_EXPRESSION RELATIONAL_EXPRESSION
             EQUALITY_EXPRESSION BAND_EXPRESSION XOR_EXPRESSION BOR_EXPRESSION LAND_EXPRESSION LOR_EXPRESSION
             CONDITIONAL_EXPRESSION ASSIGNMENT_EXPRESSION EXPRESSION
             TRANSLATION_UNIT EXTERNAL_DECLARATION FUNCTION_DEFINITION
	     DECLARATION DECLARATOR
             PARAMETER_LIST PARAMETER_DECLARATION IDENTIFIER_DEC_LIST NUMBER_LIST
             STATEMENT COMPOUND_STATEMENT EXPRESSION_STATEMENT SELECTION_STATEMENT ITERATION_STATEMENT JUMP_STATEMENT
             DECLARATION_LIST STATEMENT_LIST ASSIGNMENT_LIST

%type <number> T_NUMBER

%type <string> IDENTIFIER  STRING_LITERAL

%start START

%%

START : TRANSLATION_UNIT { g_root = $1; }
      ;

TRANSLATION_UNIT : EXTERNAL_DECLARATION { $$ = $1; }
                 | TRANSLATION_UNIT EXTERNAL_DECLARATION { $$ = new TranslationUnit($1,$2); }
                 ;

EXTERNAL_DECLARATION : FUNCTION_DEFINITION { $$ = new FunctionDef($1); }
                     | DECLARATION { $$ = $1; }
                     ;

//------Declaration

DECLARATION : S_INT IDENTIFIER_DEC_LIST T_SEMICOLON { $$ = new IntDecList($2); }//int a; int a,b,c;
            | S_INT DECLARATOR T_ASSIGN ASSIGNMENT_LIST  T_SEMICOLON { $$ = new AssignParameterDeclaraiton($2,$4); }//int a=3;
            ;

IDENTIFIER_DEC_LIST : IDENTIFIER { $$ = new IdentifierDec(*$1); }
                | IDENTIFIER_DEC_LIST T_COMMA IDENTIFIER { $$ = new IdentifierDecList($1,*$3); }
                ;

ASSIGNMENT_LIST : ASSIGNMENT_EXPRESSION { $$ = $1; }
                | ASSIGNMENT_LIST T_COMMA ASSIGNMENT_EXPRESSION { $$ = new AssignDecList($1, $3); }
                ;

DECLARATOR : IDENTIFIER { $$ = new Identifier(*$1); }
           | DECLARATOR T_LBRACKET PARAMETER_LIST T_RBRACKET { $$ = new FuncName($1, $3); }
           | DECLARATOR T_LBRACKET T_RBRACKET { $$ = new EmptyFunctionCall($1); }
           | DECLARATOR T_LBRACKET IDENTIFIER_DEC_LIST T_RBRACKET { $$ = new FunctionCall($1, $3); }
	   | DECLARATOR T_LBRACKET NUMBER_LIST T_RBRACKET { $$ = new FunctionCall($1, $3); }
           ;

NUMBER_LIST : T_NUMBER { $$ = new Number($1); }
	    | NUMBER_LIST T_COMMA T_NUMBER { $$ = new NumberList($1,$3); }
            ;

PARAMETER_LIST : PARAMETER_DECLARATION { $$ = $1; }
               | PARAMETER_LIST T_COMMA PARAMETER_DECLARATION { $$ = new ParaDecList($1, $3); }
               ;

PARAMETER_DECLARATION : S_INT IDENTIFIER { $$ = new ParaDec(*$2); }
                      ;
DECLARATION_LIST : DECLARATION { $$ = $1;  }
                 | DECLARATION_LIST DECLARATION { $$ = new DeclarationList($1,$2); } /* int a; int b;*/
                 ;
//-----------------------------Function and statement

FUNCTION_DEFINITION : S_INT DECLARATOR COMPOUND_STATEMENT { $$ = new IntFunctionDec($2,$3); }
                    | S_VOID DECLARATOR COMPOUND_STATEMENT { $$ = new VoidFunctionDec($2, $3); }
                    | S_INT IDENTIFIER T_SEMICOLON { $$ = new GlobalVar(*$2); }
                    | S_INT IDENTIFIER T_ASSIGN ASSIGNMENT_EXPRESSION T_SEMICOLON { $$ = new GlobalAssignVar(*$2, $4); }
                    ;

STATEMENT : COMPOUND_STATEMENT { $$ =$1; }
          | EXPRESSION_STATEMENT { $$ =$1; }
          | SELECTION_STATEMENT { $$ =$1; }
          | ITERATION_STATEMENT { $$ =$1; }
          | JUMP_STATEMENT { $$ =$1; }
          ;

COMPOUND_STATEMENT : T_LCURLY T_RCURLY { $$ = new EmptyCompoundStatement(); }
                   | T_LCURLY STATEMENT_LIST T_RCURLY { $$ = new UnaryCompoundStatement($2); }
                   | T_LCURLY DECLARATION_LIST T_RCURLY { $$ = new UnaryCompoundStatement($2); }
                   | T_LCURLY DECLARATION_LIST STATEMENT_LIST T_RCURLY { $$ = new BinaryCompoundStatement($2,$3); }
                   ;

STATEMENT_LIST : STATEMENT { $$ = $1; }
               | STATEMENT_LIST STATEMENT { $$ = new StatementList($1,$2); }
               ;

EXPRESSION_STATEMENT : T_SEMICOLON { $$ = new EmptyExpressionStatement(); }
                     | EXPRESSION T_SEMICOLON { $$ = new UnaryExpressionStatement($1); }
                     ;

SELECTION_STATEMENT : S_IF T_LBRACKET EXPRESSION T_RBRACKET STATEMENT { $$ = new IfStatement($3,$5); }
                    | S_IF T_LBRACKET EXPRESSION T_RBRACKET STATEMENT S_ELSE STATEMENT { $$ = new IfElseStatement($3,$5,$7); }
                    ;

ITERATION_STATEMENT : S_WHILE T_LBRACKET EXPRESSION T_RBRACKET STATEMENT { $$ = new WhileStatement($3,$5); }
                    | S_FOR T_LBRACKET EXPRESSION_STATEMENT EXPRESSION_STATEMENT EXPRESSION T_RBRACKET STATEMENT// { $$ = new ForStatement($3,$4,$5,$7); }
                    ;

JUMP_STATEMENT : S_RETURN T_SEMICOLON { $$ = new EmptyReturnStatement(); }
               | S_RETURN EXPRESSION T_SEMICOLON { $$ = new UnaryReturnStatement($2); }
               ;

//---------------

EXPRESSION : ASSIGNMENT_EXPRESSION { $$ =$1; }
           | EXPRESSION T_COMMA ASSIGNMENT_EXPRESSION { $$ = new ArgumentListExpression($1,$3); }
           ;

ASSIGNMENT_EXPRESSION : CONDITIONAL_EXPRESSION { $$ = $1; }
                      | UNARY_EXPRESSION T_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new AssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_TIMES_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new TimesAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_DIVIDE_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new DivideAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_MOD_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new ModAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_PLUS_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new PlusAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_MINUS_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new MinusAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_LSHIFT_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new LshiftAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_RSHIFT_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new RshiftAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_AND_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new AndAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_XOR_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new XorAssignOperator($1, $3); }
                      | UNARY_EXPRESSION T_OR_ASSIGN ASSIGNMENT_EXPRESSION { $$ = new OrAssignOperator($1, $3); }
                      ;

CONDITIONAL_EXPRESSION : LOR_EXPRESSION { $$ = $1; }
                       | LOR_EXPRESSION T_QUESTION EXPRESSION T_COLON CONDITIONAL_EXPRESSION// { $$ = QuestionOperator($1,$3,$5); }
                       ;

LOR_EXPRESSION : LAND_EXPRESSION { $$ = $1; }
               | LOR_EXPRESSION T_LOR LAND_EXPRESSION { $$ = new LorOperator($1, $3); }
               ;

LAND_EXPRESSION : BOR_EXPRESSION { $$ = $1; }
                | LAND_EXPRESSION T_LAND BOR_EXPRESSION { $$ = new LandOperator($1, $3); }
                ;

BOR_EXPRESSION : XOR_EXPRESSION { $$ = $1; }
               | BOR_EXPRESSION T_BOR XOR_EXPRESSION { $$ = new BorOperator($1, $3); }
               ;

XOR_EXPRESSION : BAND_EXPRESSION { $$ = $1; }
               | XOR_EXPRESSION T_XOR BAND_EXPRESSION { $$ = new XorOperator($1, $3); }
               ;

BAND_EXPRESSION : EQUALITY_EXPRESSION { $$ = $1; }
               | BAND_EXPRESSION T_BAND EQUALITY_EXPRESSION { $$ = new BandOperator($1,$3); }
               ;

EQUALITY_EXPRESSION : RELATIONAL_EXPRESSION { $$ = $1; }
                    | EQUALITY_EXPRESSION T_EQ RELATIONAL_EXPRESSION { $$ = new EqualOperator($1,$3); }
                    | EQUALITY_EXPRESSION T_NE RELATIONAL_EXPRESSION { $$ = new NotequalOperator($1,$3); }
                    ;

RELATIONAL_EXPRESSION : SHIFT_EXPRESSION { $$ = $1; }
                      | RELATIONAL_EXPRESSION T_LT SHIFT_EXPRESSION { $$ = new LessthanOperator($1,$3); }
                      | RELATIONAL_EXPRESSION T_GT SHIFT_EXPRESSION { $$ = new GreaterthanOperator($1,$3); }
                      | RELATIONAL_EXPRESSION T_GE SHIFT_EXPRESSION { $$ = new GreaterequalOperator($1,$3); }
                      | RELATIONAL_EXPRESSION T_LE SHIFT_EXPRESSION { $$ = new LessequalOperator($1,$3); }
                      ;

SHIFT_EXPRESSION : ADDITIVE_EXPRESSION { $$ = $1; }
                 | SHIFT_EXPRESSION T_LSHIFT ADDITIVE_EXPRESSION { $$ = new LshiftOperator($1,$3); }
                 | SHIFT_EXPRESSION T_RSHIFT ADDITIVE_EXPRESSION { $$ = new RshiftOperator($1,$3); }
                 ;

ADDITIVE_EXPRESSION : MULTIPLICATIVE_EXPRESSION { $$ = $1; }
                    | ADDITIVE_EXPRESSION T_PLUS MULTIPLICATIVE_EXPRESSION { $$ = new AddOperator($1,$3); }
                    | ADDITIVE_EXPRESSION T_MINUS MULTIPLICATIVE_EXPRESSION { $$ = new MinusOperator($1,$3); }
                    ;

MULTIPLICATIVE_EXPRESSION : UNARY_EXPRESSION { $$ = $1; }
                          | MULTIPLICATIVE_EXPRESSION T_TIMES UNARY_EXPRESSION { $$ = new TimesOperator($1,$3); }
                          | MULTIPLICATIVE_EXPRESSION T_DIVIDE UNARY_EXPRESSION { $$ = new DivideOperator($1,$3); }
                          | MULTIPLICATIVE_EXPRESSION T_MOD UNARY_EXPRESSION { $$ = new ModOperator($1,$3); }
                          ;

UNARY_EXPRESSION : POSTFIX_EXPRESSION { $$ = $1; }
                 | T_INC UNARY_EXPRESSION { $$ = new IncOperator($2); }
                 | T_DEC UNARY_EXPRESSION { $$ = new DecOperator($2); }
                 | T_PLUS UNARY_EXPRESSION { $$ = new PositiveOperator($2); }
                 | T_MINUS UNARY_EXPRESSION { $$ = new NegativeOperator($2); }
                 | T_BNOT UNARY_EXPRESSION { $$ = new BitwiseNotOperator($2); }
                 | T_LNOT UNARY_EXPRESSION { $$ = new LogicalNotOperator($2); }
                 ;

POSTFIX_EXPRESSION : PRIMARY_EXPRESSION { $$ = $1; }
                   | POSTFIX_EXPRESSION T_LBRACKET T_RBRACKET { $$ = new EmptyFunctionCall($1); }
                   | POSTFIX_EXPRESSION T_LBRACKET ARGUMENT_EXPRESSION_LIST T_RBRACKET { $$ = new FunctionCall($1,$3); }
                   | POSTFIX_EXPRESSION T_INC { $$ = new IncPostfixOperator($1); }
                   | POSTFIX_EXPRESSION T_DEC { $$ = new DecPostfixOperator($1);}
                   ;

PRIMARY_EXPRESSION : IDENTIFIER { $$ = new Identifier(*$1); }
                   | T_NUMBER   { $$ = new Number($1); }
                   | STRING_LITERAL { $$ = new Identifier(*$1); }
                   | T_LBRACKET EXPRESSION T_RBRACKET { $$ = $2; }
                   ;

ARGUMENT_EXPRESSION_LIST : ASSIGNMENT_EXPRESSION { $$ = $1; }
                         | ARGUMENT_EXPRESSION_LIST T_COMMA ASSIGNMENT_EXPRESSION { $$ = new ArgumentListExpression($1,$3); }
                         ;


%%

const Node* g_root;
const Node* parseAST()
{
  g_root=0;
  yyparse();
  return g_root;
}
