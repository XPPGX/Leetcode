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
    struct ListNode* pre_ptr1;
    struct ListNode* pre_ptr2;
    int carry = 0;
    for(ptr1 = l1, ptr2 = l2; ptr1->next != NULL, ptr2->next != NULL; ptr1 = ptr1->next, ptr2 = ptr2->next){
        printf("ptr1->val = %d, ptr2->val = %d, ", ptr1->val, ptr2->val);
        ptr1->val = ptr1->val + ptr2->val + carry;
        if(ptr1->val >= 10){
            carry = ptr1->val / 10;
            ptr1->val = ptr1->val % 10;
            printf("carry = %d, ", carry);
        }
        else{
            carry = 0;
        }
        printf("digit_sum = %d\n", ptr1->val);
        pre_ptr1 = ptr1;
        pre_ptr2 = ptr2;
        printf("pre_ptr1 = %p, ptr1->next = %p\n", pre_ptr1, ptr1->next);
        printf("pre_ptr2 = %p, ptr2->next = %p\n", pre_ptr2, ptr2->next);
    }
    printf("after one is null : ptr1 = %p, ptr2 = %p\n\n", ptr1, ptr2);

    if(ptr1 == NULL){
        printf("ptr1 is NULL\n");
        printf("pre_ptr1 = %p, pre_ptr1->next = %p, ptr2 = %p\n", pre_ptr1, pre_ptr1->next, ptr2);
        pre_ptr1->next = ptr2;
        printf("after chained, pre_ptr1->next = %p, ptr2 = %p\n", pre_ptr1->next, ptr2);
        printf("\n");
        for(; ptr2 != NULL; ptr2 = ptr2->next){
            printf("ptr2->val = %d, carry = %d\n", ptr2->val, carry);
            ptr2->val = ptr2->val + carry;
            if(ptr2->val >= 10){
                carry = ptr2->val / 10;
                ptr2->val = ptr2->val % 10;
            }
        }
        if(carry > 0){
            printf("carry still > 0\n");
            ptr2 = (struct ListNode*)malloc(sizeof(struct ListNode*));
            ptr2->val = carry;
            ptr2->next = NULL;
        }
    }
    else if(ptr2 == NULL){
        printf("ptr2 is NULL\n");
        printf("pre_ptr1 = %p, pre_ptr1->next = %p\n", pre_ptr1, pre_ptr1->next);
        for(; ptr1 != NULL; ptr1 = ptr1->next){
            ptr1->val = ptr1->val + carry;
            if(ptr1->val >= 10){
                carry = ptr1->val / 10;
                ptr1->val = ptr1->val % 10;
            }
        }
        if(carry > 0){
            ptr1 = (struct ListNode*)malloc(sizeof(struct ListNode*));
            ptr1->val = carry;
            ptr1->next = NULL;
        }
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