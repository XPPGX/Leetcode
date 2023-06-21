#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#define _DEBUG_

#ifndef _DEBUG_
#define _RELEASE_
#endif

struct Node{
    int val;
    int index;
    struct Node* next;
};

inline int hash_function(int val){
    return val % 10; 
}

void insert_key(int _val, int _index, struct Node** _hashtable){
    //insert = O(1)
    int hash_value = hash_function(_val);
    struct Node* hash_value_head;
    hash_value_head = _hashtable[hash_value];
    //printf("hash value [%d]:\n", hash_value);
    struct Node* hash_value_new = (struct Node*)malloc(sizeof(struct Node));
    hash_value_new->val = _val;
    hash_value_new->index = _index;
    hash_value_new->next = hash_value_head;
    _hashtable[hash_value] = hash_value_new;
}

int query_key(int _val, int _num1_index, struct Node** _hashtable){
    //query = O(L), where L is the length of nodes in the slot that we need.
    int hash_value = hash_function(_val);
    struct Node* hash_aim_ptr;
    for(hash_aim_ptr = _hashtable[hash_value]; hash_aim_ptr->next != NULL; hash_aim_ptr = hash_aim_ptr->next){
        if(hash_aim_ptr->val == _val && hash_aim_ptr->index != _num1_index){
            #ifndef _RELEASE_
            printf("Find %d in hash table\n", _val);
            #endif
            return 1;
        }
    }
    return 0;
}

int* two_sum(int* nums, int numsSize, int target, int* returnSize){
    struct Node* hashtable[10];
    for(int i = 0 ; i < 10 ; i ++){
        hashtable[i] = (struct Node*)malloc(sizeof(struct Node));
        hashtable[i]->val = 0; // this value will never use.
        hashtable[i]->next = NULL;
    }

    //find the minimum value, it will be offset during hashing
    int min_value = nums[0];
    for(int i = 0 ; i < numsSize ; i ++){
        if(min_value > nums[i]){
            min_value = nums[i];
        }
    }
    #ifndef _RELEASE_
    printf("min_value = %d\n", min_value);
    #endif

    //create hash_table = O(N) by using min_vlaue as offset
    for(int i = 0 ; i < numsSize ; i ++){
        insert_key(nums[i] - min_value, i, hashtable);
    }
    
    #ifdef _RELEASE_
    int num1 = -1;
    int num2 = -1;
    int num1_index = -1;
    int num2_index = -1;
    int flag = 0; // if flag == 1 means that anwser found, if flag == 0 means that anwser not found.
    for(int i = 0 ; i < numsSize ; i ++){
        num1 = nums[i];
        num2 = target - num1;
        num1_index = i;

        if((num2 - min_value) < 0){
            continue;
        }

        if(query_key(num2 - min_value, num1_index, hashtable)){
            flag = 1;
            printf("Found (%d, %d) is the answer pair\n", num1, num2);
            break;
        }
    }
    for(int i = 0 ; i < numsSize ; i ++){
        if(nums[i] == num2 && flag == 1){
            num2_index = i;
        }
    }

    if(num1_index == -1 || num2_index == -1){
        printf("Pair not found\n");
        *returnSize = 0;
        return NULL;
    }
    else{
        *returnSize = 2;
        int* answer = (int*)malloc(sizeof(int) * 2);
        answer[0] = num1_index;
        answer[1] = num2_index;
        return answer;
    }
    #endif

    #ifdef _DEBUG_
    struct Node* ptr;
    for(int i = 0 ; i < 10 ; i ++){
        printf("show hashtable[%d] : ", i);
        for(ptr = hashtable[i]; ptr->next != NULL; ptr = ptr->next){
            printf("%d, ", ptr->val);
        }
        printf("\n");
    }
    free(ptr);
    return NULL;
    #endif
}

int main(int argc, char* argv[]){
    //先掃一遍arr，找出max, min，然後用min當offset，把負的值都改成正的，再建立hashtable
    //原本arr中的值不會變，只有在建hashtable的時候val才會搭配offset進行進行hash。
    int numsSize = 5;
    int* nums = (int*)malloc(sizeof(int) * numsSize);
    nums[0] = -1;
    nums[1] = -2;
    nums[2] = -3;
    nums[3] = -4;
    nums[4] = -5;
    int target = atoi(argv[1]);
    int returnSize = -1;
    int* temp = two_sum(nums, numsSize, target, &returnSize);
    #ifdef _RELEASE_
    if(temp != NULL){
        printf("(%d, %d) is one answer\n", temp[0], temp[1]);
    }
    else{
        printf("No answer\n");
    }
    #endif
}