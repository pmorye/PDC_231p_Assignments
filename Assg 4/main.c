/**
(assuming that you are using the gcc compiler)
Compile me with:  gcc main.c mat-mul.c -o my_executable
Run me with:      ./my_executable t s
Where t is the number of threads, and s the size of the matrices.
**/
#include <stdio.h>
#include <stdlib.h>
#include "mat-mult.h"

static long rand_seed=13;

/* from K&R: produces a random number.*/
int rand(){
    rand_seed = rand_seed * 1103515245 +12345;
    return (int)(rand_seed / 65536) % 32768;
}

/*an array filler, you pass the pointer and 
  how many elements to fill*/
void fill_matrices(int* p, int count){
    for(int i=0;i<count;++i){
	*(p+i)=rand();
    }
}

/*pretty printer for the matrices*/
void print_mat(int* p,int off, int size){
    for(int i=off;i<off+size*size;++i){
	printf("%7d ",*(p+i));
	if((i+1)%size==0)
	   printf("\n"); 
    }
    printf("\n");
}

int main(int argc, char **argv){
    if(argc<3){//less than 2 arguments in command line
	printf("USAGE: ./main.bin threads size\n");
	exit(1);
    }
    int threads=atoi(argv[1]); //get the first argument
    int s=atoi(argv[2]);       //get the second one
    int* mat_ptr;
    
    mat_ptr = (int*) calloc(s*s*3, sizeof(int)); //allocating mem
    fill_matrices(mat_ptr, s*s*2);               //initialize A and B
    print_mat(mat_ptr, 0, s);                    //print A
    print_mat(mat_ptr, s*s, s);                  //print B
    multiply(mat_ptr, threads, s);               //multiply
    print_mat(mat_ptr, s*s*2, s);                //print C
    free(mat_ptr);
    return 0;
}
