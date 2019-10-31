%option noyywrap

%{
// Avoid error "error: `fileno' was not declared in this scope"
extern "C" int fileno(FILE *stream);

#include "parser.tab.hpp"
%}

%%

"~"             { return T_BNOT; }
"!"             { return T_LNOT; }
"?"             { return T_QUESTION; }
":"             { return T_COLON; }
";"             { return T_SEMICOLON; }
","             { return T_COMMA; }
"+"             { return T_PLUS; }
"-"             { return T_MINUS; }
"*"             { return T_TIMES; }
"/"             { return T_DIVIDE; }
"%"             { return T_MOD; }
"="             { return T_ASSIGN; }
"*="            { return T_TIMES_ASSIGN; }
"/="            { return T_DIVIDE_ASSIGN; }
"%="            { return T_MOD_ASSIGN; }
"+="            { return T_PLUS_ASSIGN; }
"-="            { return T_MINUS_ASSIGN; }
"<<="           { return T_LSHIFT_ASSIGN; }
">>="           { return T_RSHIFT_ASSIGN; }
"&="            { return T_AND_ASSIGN; }
"|="            { return T_OR_ASSIGN; }
"^="            { return T_XOR_ASSIGN; }
">"             { return T_GT; }
">="            { return T_GE; }
"<"             { return T_LT; }
"<="            { return T_LE; }
"=="            { return T_EQ; }
"!="            { return T_NE; }
"&"             { return T_BAND; }
"|"             { return T_BOR; }
"^"             { return T_XOR; }
"&&"            { return T_LAND; }
"||"            { return T_LOR; }
"{"             { return T_LCURLY; }
"}"             { return T_RCURLY; }
"("             { return T_LBRACKET; }
")"             { return T_RBRACKET; }
"++"            { return T_INC; }
"--"            { return T_DEC; }
"<<"            { return T_LSHIFT; }
">>"            { return T_RSHIFT; }

"else"          { return S_ELSE; }
"if"            { return S_IF; }
"do"            { return S_DO; }
"while"         { return S_WHILE; }
"for"           { return S_FOR; }
"return"        { return S_RETURN; }
"int"           { return S_INT; }
"void"          { return S_VOID; }





[-]?[0-9]+      { yylval.number=strtod(yytext, 0); return T_NUMBER; }
[a-zA-Z]+        { yylval.string=new std::string(yytext); return IDENTIFIER; }
\"(\\.|[^\\"])*\" { yylval.string=new std::string(yytext); return STRING_LITERAL; }
[ \t\r\n]+		{;}


.               { fprintf(stderr, "Invalid token\n"); exit(1); }
%%

void yyerror (char const *s)
{
  fprintf (stderr, "Parse error : %s\n", s);
  exit(1);
}
