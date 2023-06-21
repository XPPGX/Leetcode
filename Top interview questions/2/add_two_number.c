#include <stdio.h>
#include <stdlib.h>

//#define _DEBUG_

#ifndef _DEBUG_
#define _RELEASE_
#endif

struct ListNode{
    int val;
    struct ListNode *next;
};

void CreateTwoListNode(int _val_1, int _val_2, struct ListNode** _l1, struct ListNode** _l2){
    int tail_digit = -1;
    struct ListNode* ptr;
    
    struct ListNode* l1 = (struct ListNode*)malloc(sizeof(struct ListNode*));
    if(_val_1 == 0){
        ptr = l1;
        ptr->val = 0;
        ptr->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        ptr = ptr->next;
        ptr->val = -1;
        ptr->next = NULL;
    }
    else{
        for(ptr = l1; _val_1 != 0 ; _val_1 = _val_1 / 10){
            tail_digit = _val_1 % 10;
            #ifdef _DEBUG_
            printf("tail digit = %d\n", tail_digit);
            #endif
            ptr->val = tail_digit;
            ptr->next = (struct ListNode*)malloc(sizeof(struct ListNode*));
            ptr = ptr->next;
        }
        ptr->val = -1;
        ptr->next = NULL;
    }

    struct ListNode* l2 = (struct ListNode*)malloc(sizeof(struct ListNode*));
    if(_val_2 == 0){
        ptr = l2;
        ptr->val = 0;
        ptr->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        ptr = ptr->next;
        ptr->val = -1;
        ptr->next = NULL;
    }
    else{
        for(ptr = l2; _val_2 != 0 ; _val_2 = _val_2 / 10){
            tail_digit = _val_2 % 10;
            #ifdef _DEBUG_
            printf("tail digit = %d\n", tail_digit);
            #endif
            ptr->val = tail_digit;
            ptr->next = (struct ListNode*)malloc(sizeof(struct ListNode*));
            ptr = ptr->next;
        }
        ptr->val = -1;
        ptr->next = NULL;
    }


    printf("reverse l1 = ");
    for(ptr = l1; ptr->next != NULL; ptr = ptr->next){
        printf("%d", ptr->val);
    }
    printf("\n");

    printf("reverse l2 = ");
    for(ptr = l2; ptr->next != NULL; ptr = ptr->next){
        printf("%d", ptr->val);
    }
    printf("\n");
    

    //assign two heads of lists to the input pointer
    *_l1 = l1;
    *_l2 = l2;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    
    return NULL;
}

int main(int argc, char* argv[]){
    int num1 = atoi(argv[1]);
    int num2 = atoi(argv[2]);
    printf("num1 = %d\n", num1);
    printf("num2 = %d\n", num2);
    struct ListNode* l1;
    struct ListNode* l2;
    CreateTwoListNode(num1, num2, &l1, &l2);
    //printf("%d\n", l1->val);
    //printf("%d\n", l2->val);
    
}