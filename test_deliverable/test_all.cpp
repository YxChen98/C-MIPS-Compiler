#include<stdlib.h>
#include<stdio.h>
#include<iostream>

int main(){

    std::cout<<"Testing FOR_N:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/for_n_driver.c -o test_deliverable/mips_out/for_n_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/for_n.c -o test_deliverable/mips_out/for_n.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/for_n.s test_deliverable/mips_out/for_n_driver.s -o test_deliverable/mips_out/for_n");
    system("qemu-mips test_deliverable/mips_out/for_n");
    system("echo $?");


    std::cout<<"Testing IF_F:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/if_f_driver.c -o test_deliverable/mips_out/if_f_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/if_f.c -o test_deliverable/mips_out/if_f.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/if_f.s test_deliverable/mips_out/if_f_driver.s -o test_deliverable/mips_out/if_f");
    system("qemu-mips test_deliverable/mips_out/if_f");
    system("echo $?");


    std::cout<<"Testing IF_T:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/if_t_driver.c -o test_deliverable/mips_out/if_t_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/if_t.c -o test_deliverable/mips_out/if_t.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/if_t.s test_deliverable/mips_out/if_t_driver.s -o test_deliverable/mips_out/if_t");
    system("qemu-mips test_deliverable/mips_out/if_t");
    system("echo $?");


    std::cout<<"Testing IF_ELSE_F:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/if_else_f_driver.c -o test_deliverable/mips_out/if_else_f_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/if_else_f.c -o test_deliverable/mips_out/if_else_f.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/if_else_f.s test_deliverable/mips_out/if_else_f_driver.s -o test_deliverable/mips_out/if_else_f");
    system("qemu-mips test_deliverable/mips_out/if_else_f");
    system("echo $?");
  

    std::cout<<"Testing IF_ELSE_T:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/if_else_t_driver.c -o test_deliverable/mips_out/if_else_t_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/if_else_t.c -o test_deliverable/mips_out/if_else_t.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/if_else_t.s test_deliverable/mips_out/if_else_t_driver.s -o test_deliverable/mips_out/if_else_t");
    system("qemu-mips test_deliverable/mips_out/if_else_t");
    system("echo $?");

    std::cout<<"Testing RETURN:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/return_driver.c -o test_deliverable/mips_out/return_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/return.c -o test_deliverable/mips_out/return.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/return.s test_deliverable/mips_out/return_driver.s -o test_deliverable/mips_out/return");
    system("qemu-mips test_deliverable/mips_out/return");
    system("echo $?");


    std::cout<<"Testing ASSIGNMENT:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/assignment_driver.c -o test_deliverable/mips_out/assignment_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/assignment.c -o test_deliverable/mips_out/assignment.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/assignment.s test_deliverable/mips_out/assignment_driver.s -o test_deliverable/mips_out/assignment");
    system("qemu-mips test_deliverable/mips_out/assignment");
    system("echo $?");


    std::cout<<"Testing MAIN:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/main_driver.c -o test_deliverable/mips_out/main_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/main.c -o test_deliverable/mips_out/main.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/main.s test_deliverable/mips_out/main_driver.s -o test_deliverable/mips_out/main");
    system("qemu-mips test_deliverable/mips_out/main");
    system("echo $?");


    std::cout<<"Testing GLOBAl:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/global_driver.c -o test_deliverable/mips_out/global_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/global.c -o test_deliverable/mips_out/global.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/global.s test_deliverable/mips_out/global_driver.s -o test_deliverable/mips_out/global");
    system("qemu-mips test_deliverable/mips_out/global");
    system("echo $?");


    std::cout<<"Testing LOCAL_ARRAY:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/local_array_driver.c -o test_deliverable/mips_out/local_array_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/local_array.c -o test_deliverable/mips_out/local_array.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/local_array.s test_deliverable/mips_out/local_array_driver.s -o test_deliverable/mips_out/loca_array");
    system("qemu-mips test_deliverable/mips_out/local_array");
    system("echo $?");
    

    std::cout<<"Testing SELF_RECURSION:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/self_recursion_driver.c -o test_deliverable/mips_out/self_recursion_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/self_recursion.c -o test_deliverable/mips_out/self_recursion.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/self_recursion.s test_deliverable/mips_out/self_recursion_driver.s -o test_deliverable/mips_out/self_recursion");
    system("qemu-mips test_deliverable/mips_out/self_recursion");
    system("echo $?");


    std::cout<<"Testing ATOF:"<<std::endl;
    system("mips-linux-gnu-gcc -c -S test_deliverable/test_cases/atof_driver.c -o test_deliverable/mips_out/atof_driver.s");
    system("./bin/c_compiler --S test_deliverable/test_cases/atof.c -o test_deliverable/mips_out/atof.s");
    system("mips-linux-gnu-gcc -static test_deliverable/mips_out/atof.s test_deliverable/mips_out/atof_driver.s -o test_deliverable/mips_out/atof");
    system("qemu-mips test_deliverable/mips_out/atof");
    system("echo $?");


    return 0;
}
