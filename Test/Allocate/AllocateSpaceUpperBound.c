#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    printf("HI\n");
    long edgeNum = atol(argv[1]);
    printf("edgeNum = %ld\n", edgeNum);
    int* arr = (int*)malloc(sizeof(int) * edgeNum * 2);
}