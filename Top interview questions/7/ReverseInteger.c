#include <stdio.h>
#include <stdlib.h>

int reverse(int x){
    // printf("max int = %d\n",__INT_MAX__);
    int maxVal[10] = {2,1,4,7,4,8,3,6,4,7};
    int* arr = (int*)calloc(10, sizeof(int));
    int index = 0;
    int returnVal = 0;
    int weight = 1;
    if(x >= 0){
        while(x > 0){
            arr[index] = x % 10;
            x = x / 10;
            index = index + 1;
        }
        // printf("arr = ");
        // for(int i = 0 ; i < 10 ; i ++){
        //     printf("%d", arr[i]);
        // }
        // printf("\n");

        //check overflow
        int overflowFlag = 2; //1 == overflow, 2 == keepgoing, 3 == will not overflow
        if(arr[9] != 0){
            for(int i = 0 ; i < 10 ; i ++){
                if(arr[i] > maxVal[i]){
                    // printf("overflow\n");
                    overflowFlag = 1;
                    break;
                }
                else if(arr[i] < maxVal[i]){
                    // printf("will not overflow\n");
                    overflowFlag = 3;
                    break;
                }
            }
        }
        // printf("overflowFlag = %d\n", overflowFlag);
        if(overflowFlag == 1){
            return 0;
        }
        int startFlag = 0;
        for(int i = 9 ; i >= 0 ; i --){
            if(arr[i] > 0 && startFlag == 0){
                startFlag = 1;
            }
            if(startFlag == 1){
                returnVal = returnVal + arr[i] * weight;
                if(i > 0){
                    weight = weight * 10;
                }
            }
        }
        // printf("returnVal = %d\n", returnVal);
        return returnVal;
    }
    else{
        if(x + __INT_MAX__ < 0){
            return 0;
        }
        int positive = 0 - x;
        int reversePositive = reverse(positive);
        returnVal = -reversePositive;
        // printf("reversePositive = %d\n", returnVal);
        return returnVal;
    }
}

int main(int argc, char* argv[]){
    int x = atoi(argv[1]);
    int y = reverse(x);
    printf("y = %d\n", y);
}