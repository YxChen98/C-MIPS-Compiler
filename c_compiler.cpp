#include "include/ast.hpp"

#include<iostream>
#include<fstream>
#include<vector>
#include<stdlib.h>
#include<stdio.h>

void translate(){   

    const Node* ast=parseAST();
    ast->translate(std::cout);

    std::cout<<std::endl;
    std::cout<<"# Boilerplate"<<std::endl;
    std::cout<<"if __name__ == "<<'"'<<"__main__"<<'"'<<":"<<std::endl;
    std::cout<<"    import sys"<<std::endl;
    std::cout<<"    ret=main()"<<std::endl;
    std::cout<<"    sys.exit(ret)"<<std::endl;
    
}

void compile(char* inFile){

    const NodePtr ast=parseAST();
  
    std::cout<<"        .file   1 "<<'"'<<inFile<<'"'<<std::endl;
    std::cout<<"        .section .mdebug.abi32"<<std::endl;
    std::cout<<"        .previous"<<std::endl;
    std::cout<<"        .nan    legacy"<<std::endl;
    std::cout<<"        .module fp=xx"<<std::endl;
    std::cout<<"        .module nooddspreg"<<std::endl;
    std::cout<<"        .abicalls"<<std::endl;

    ast->compile(std::cout);
}

int main(int argc, char* argv[]){
    
    //bin/c_compiler --translate [source-file.c] -o [dest-file.py]
    if(argc!=5){
        fprintf(stderr, "not well formatted command input\n");
        exit(1);
    }

    //convert fstream to stdiostream
    freopen(argv[2], "r", stdin);
    freopen(argv[4], "w", stdout);

    std::string command;
    command=argv[1];

    if(command=="--S"){
        compile( argv[2]);
    }

    else if(command=="--translate"){
        translate();
    }

    else{
        std::cout<<"no command found"<<std::endl;
        exit(0);
    }

    fclose(stdout);

    return 0;
}


