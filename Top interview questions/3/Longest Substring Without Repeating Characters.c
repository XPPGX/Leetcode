#include <stdio.h>
#include <stdlib.h>


int lengthOfLongestSubstring(char* s){
    printf("Input string is : %s\n", s);
    int sliding_window_head = 0;
    int sliding_window_tail = 0;
    int length = 0; //longest substring without repeating characters
    int sliding_window_length = 0;
    for(sliding_window_head = 0, sliding_window_tail = 0; s[sliding_window_tail] != '\0'; sliding_window_tail ++){
        printf("head = %d, tail = %d, s[%d] = %c\n", sliding_window_head, sliding_window_tail, sliding_window_tail, s[sliding_window_tail]);
        sliding_window_length = sliding_window_tail - sliding_window_head;
        for(int temp_index = sliding_window_head; temp_index < sliding_window_tail; temp_index ++){
            printf("checking...\n");
            if(s[temp_index] == s[sliding_window_tail]){
                printf("(%d, %d) repeat\n\n", temp_index, sliding_window_tail);
                sliding_window_head++;
                sliding_window_tail = sliding_window_head - 1;
                break;
            }
        }
        if(sliding_window_length > length){
            length = sliding_window_length;
        }
        printf("\n");
    }
    printf("tail = %d, head = %d\n", sliding_window_tail, sliding_window_head);
    sliding_window_length = sliding_window_tail - sliding_window_head;
    if(sliding_window_length > length){
        length = sliding_window_length;
    }
    printf("length = %d\n", length);
    if(sliding_window_head == 0 && sliding_window_tail == 0){
        printf("no string\n");
        length = 0;
    }
    if(sliding_window_head == 0 && sliding_window_tail == 1){
        printf("only one char\n");
        length = 1;
    }
    return length;
}


int main(){
    // char* s = "au";
    // char* s = "abcabcbb";
    // char* s = "pwwkew";
    // char* s = "bbbbb";
    // char* s = "";
    // char* s = " "; 
    char* s = "bapriujimqwspslgvlcsaqbdbgwtbseettwdnfnbyjvpdjxyuzqxstatbzpctthoofremgfkrbcvkzvgbofthgojh";
    int length = lengthOfLongestSubstring(s);
}