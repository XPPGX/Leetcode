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
    struct ListNode* tmp_ptr;
    struct ListNode* l1 = (struct ListNode*)malloc(sizeof(struct ListNode*));
    if(_val_1 == 0){
        ptr = l1;
        ptr->val = 0;
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
            tmp_ptr = ptr;
            ptr = ptr->next;
        }
        free(ptr);
        tmp_ptr->next = NULL;
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
            tmp_ptr = ptr;
            ptr = ptr->next;
        }
        free(ptr);
        tmp_ptr->next = NULL;
    }


    printf("reverse l1 = ");
    for(ptr = l1; ptr != NULL; ptr = ptr->next){
        printf("%d", ptr->val);
    }
    printf("\n");

    printf("reverse l2 = ");
    for(ptr = l2; ptr != NULL; ptr = ptr->next){
        printf("%d", ptr->val);
    }
    printf("\n\n");
    

    //assign two heads of lists to the input pointer
    *_l1 = l1;
    *_l2 = l2;
}

struct ListNode* addTwoNumbers(struct ListNode* l1, struct ListNode* l2){
    struct ListNode* ptr1;
    struct ListNode* ptr2;
    struct ListNode* pre_current = NULL;
    struct ListNode* current;
    int carry = 0;
    int sum = 0;
    int lock = 1;
    ptr1 = l1;
    ptr2 = l2;
    current = ptr1;
    for(; ptr1 || ptr2; ){
        sum = carry;
        if(ptr1){
            printf("ptr1->val = %d\t", ptr1->val);
            sum = sum + ptr1->val;
            ptr1 = ptr1->next;
        }
        if(ptr2){
            printf("ptr2->val = %d\t", ptr2->val);
            sum = sum + ptr2->val;
            ptr2 = ptr2->next;
        }
        carry = sum / 10;
        printf("carry = %d\tsum = %d\n", carry, sum % 10);
        current->val = sum % 10;

        pre_current = current;
        current = current->next;

        if(lock == 1){
            if(!ptr1 && ptr2){
                lock = 0;
                printf("\n\n");
                printf("ptr1 = %p, ptr2 = %p\n", ptr1, ptr2);
                pre_current->next = ptr2;
                current = ptr2;
            }
        }
    }
    if(carry > 0){
        pre_current->next = (struct ListNode*)malloc(sizeof(struct ListNode));
        pre_current = pre_current->next;
        pre_current->val = carry;
        pre_current->next = NULL;
    }

    return l1;
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
    struct ListNode* ans = addTwoNumbers(l1, l2);
    printf("\n\n");
    printf("ans = ");
    for(; ans != NULL; ans = ans->next){
        printf("%d", ans->val);
    }
    printf("\n");
}