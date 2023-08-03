#include <stdio.h>
#include <stdlib.h>

typedef enum{
    N,
    Y
}bBool;

int myAtoi(char* s){
    int intMax[10] = {2,1,4,7,4,8,3,6,4,7};
    bBool pos_neg = Y; //default is positive
    bBool startFlag = N;
    bBool leadingZero = N;
    bBool overflowSus = N;
    int* ansArr = (int*)calloc(11, sizeof(int));
    int digitCount = 0;
    for(int i = 0 ; s[i] != '\0' ; i ++){
        if(startFlag == N){
            if(s[i] == ' '){
                continue;
            }
            else if(s[i] == '-'){
                printf("neg signed\n");
                startFlag = Y;
                pos_neg = N;
                continue;
            }
            else if(s[i] == '+'){
                printf("pos signed\n");
                startFlag = Y;
                pos_neg = Y;
                continue;
            }
            else if('0' <= s[i] && s[i] <= '9'){
                if(s[i] == '0'){
                    printf("leadingZero\n");
                    leadingZero = Y;
                }
                printf("s[%d] = %c\n", i, s[i]);
                startFlag = Y;
            }
            else{              
                return 0;
            }
        }        

        if(startFlag == Y){
            if(s[i] < '0' || '9' < s[i]){
                printf("s[%d] = %c is not digit, out of for loop\n", i, s[i]);
                break;
            }
            else{
                if(digitCount == 10){ //overflow
                    if(pos_neg == Y){
                        return __INT_MAX__;
                    }
                    else{
                        return -__INT_MAX__ - 1;
                    }
                }
                if((leadingZero == Y && s[i] == '0' ) || (ansArr[0] == 0 && s[i] == '0')){
                    printf("s[%d] = %c, pass\n", i, s[i]);
                    continue;
                }
                leadingZero = N;
                ansArr[digitCount] = s[i] - 0x30;
                digitCount ++;
                printf("digitCount = %d\n", digitCount);
            }
        }
    }

    printf("ansArr = ");
    for(int i = 0 ; i < 11 ; i ++){
        printf("%d", ansArr[i]);
    }
    printf("\n");

    //overflow clamp
    if(digitCount == 9){
        if(pos_neg == Y){
            for(int i = 0 ; i <= digitCount ; i ++){
                if(ansArr[i] > intMax[i]){
                    //is overflow
                    return __INT_MAX__;
                }
                else if(ansArr[i] < intMax[i]){
                    //no overflow
                    break;
                }
            }
        }
        else{
            overflowSus = Y;
            for(int i = 0 ; i <= digitCount - 1 ; i ++){
                if(ansArr[i] > intMax[i]){
                    return -__INT_MAX__ - 1;
                }
                else if(ansArr[i] == intMax[i]){
                    overflowSus = Y;
                }
                else if(ansArr[i] < intMax[i]){
                    break;
                }
            }
            if(overflowSus == Y && ansArr[digitCount] >= 8){
                return -__INT_MAX__ - 1;
            }
        }
    }

    int ans = 0;
    for(int i = 0 ; i < digitCount ; i ++){
        if(ans > __INT_MAX__ / 10){
            if(pos_neg == Y){
                return __INT_MAX__;
            }
            else{
                return -__INT_MAX__ - 1;
            }
        }
        if(ans == __INT_MAX__ / 10 && ansArr[i] >= 8){
            if(pos_neg == Y){
                return __INT_MAX__;
            }
            else{
                return -__INT_MAX__ - 1;
            }
        }
        ans = ans * 10 + ansArr[i];
    }

    if(pos_neg == N){
        printf("negative\n");
        ans = 0 - ans;
    }
    printf("return val = %d\n", ans);
    return ans;
}

int main(int argc, char* argv[]){
    // char* input = "4193 with words"; //ans = -123
    // char* input = "words and 987";
    // char* input = "   -42";
    // char* input = "-91283472332";
    // char* input = "-2147483648";
    // char* input = "2147483648";
    // char* input = "+-12";
    // char* input = "  0000000000012345678";
    // char* input = "00000-42a1234";
    // char* input = "-2147483649";
    char* input = "2147483800";
    // char* input = "21474836460";
    // char* input = "-000000000000001";
    // char* input = "010";
    // char* input = "2147483646";
    printf("input = %s\n", input);
    int ans = myAtoi(input);
    printf("ans = %d\n", ans);
}