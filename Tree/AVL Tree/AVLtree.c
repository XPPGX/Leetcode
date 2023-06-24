#include <stdio.h>
#include <stdlib.h>

struct Node{
    int primary_key;
    int depth;
    struct Node* left_child;
    struct Node* right_child;
    struct Node* parent;
};

struct Node* create_node(struct Node* _node, int data){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->primary_key = data;
    new_node->depth = 0;
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    new_node->parent = _node;
    return new_node;
}

int max(int a, int b){
    return (a > b) ? a : b;
}

void update_depth(struct Node* _node){
    if((_node->left_child) && (_node->right_child)){ //both child exist
        _node->depth = max(_node->left_child->depth, _node->right_child->depth) + 1;
        printf("node = %d : depth(l) = %d, depth(r) = %d, depth(%d) = %d\n", _node->primary_key, _node->left_child->depth, _node->right_child->depth, _node->primary_key, _node->depth);
    }
    else if(!(_node->left_child) && (_node->right_child)){//left not exist, right exist
        _node->depth = _node->right_child->depth + 1;
        printf("node = %d : depth(l) = -1, depth(r) = %d, depth(%d) = %d\n", _node->primary_key, _node->right_child->depth, _node->primary_key, _node->depth);
    }
    else if((_node->left_child) && !(_node->right_child)){//left exist, right not exist
        _node->depth = _node->left_child->depth + 1;
        printf("node = %d : depth(l) = %d, depth(r) = -1, depth(%d) = %d\n", _node->primary_key, _node->left_child->depth, _node->primary_key, _node->depth);
    }
    else{//both child not exist
        _node->depth = 0;
        printf("node = %d, no child, depth(%d) = %d\n", _node->primary_key, _node->primary_key, _node->depth);
    }
}

void trace_backward_update_depth(struct Node* _node){
    //O(log n)
    if(_node->parent == NULL){
        return;
    }

    struct Node* parent_ptr;
    for(parent_ptr = _node->parent ; parent_ptr != NULL ; parent_ptr = parent_ptr->parent){
        update_depth(parent_ptr);
    }
}

int get_balance_factor(struct Node* _node){
    int left_depth = 0;
    int right_depth = 0;
    if(_node->left_child){
        left_depth = _node->left_child->depth + 1;
    }
    else{
        left_depth = 0;
    }
    
    if(_node->right_child){
        right_depth = _node->right_child->depth + 1;
    }
    else{
        right_depth = 0;
    }
    return left_depth - right_depth;
}

void LL_operation(struct Node** _root, struct Node* _node){
    struct Node* left_child = _node->left_child; 

    //rewire
    _node->left_child = left_child->right_child;
    if(left_child->right_child != NULL){
        left_child->right_child->parent = _node;
    }
    left_child->right_child = _node;

    if(_node->parent){
        if(_node->parent->left_child == _node){ //_node is _node->parent's left_child
            _node->parent->left_child = left_child;
        }
        else{ //_node is _node->parent's right_child
            _node->parent->right_child = left_child;
        }
    }
    left_child->parent = _node->parent;
    _node->parent = left_child;

    //update depth
    update_depth(_node);
    trace_backward_update_depth(_node);

    //check if the upward node is the root or not
    if(left_child->parent == NULL){// the pointer is root
        printf("after rotation, %d is new root\n", left_child->primary_key);
        *_root = left_child;
    }
}

void RR_operation(struct Node** _root, struct Node* _node){
    struct Node* right_child = _node->right_child;
    //rewire
    _node->right_child = right_child->left_child;
    if(right_child->left_child != NULL){
        right_child->left_child->parent = _node;
    }
    right_child->left_child = _node;

    if(_node->parent){
        if(_node->parent->left_child == _node){ //_node is _node->parent's left_child
            _node->parent->left_child = right_child;
        }
        else{ //_node is _node->parent's right_child
            _node->parent->right_child = right_child;
        }
    }
    right_child->parent = _node->parent;
    _node->parent = right_child;
    
    //update depth
    update_depth(_node);
    trace_backward_update_depth(_node);

    //check if the upward node is the root or not
    if(right_child->parent == NULL){// the pointer is new root
        printf("after rotation, %d is new root\n", right_child->primary_key);
        *_root = right_child;
    }
}

void LR_operation(struct Node** _root, struct Node* _node){
    printf("after RR\n");
    RR_operation(_root, _node->left_child);
    printf("after LL\n");
    LL_operation(_root, _node);
}

void RL_operation(struct Node** _root, struct Node* _node){
    printf("after LL\n");
    LL_operation(_root, _node->right_child);
    printf("after RR\n");
    RR_operation(_root, _node);
}

void trace_backward_to_balance_predecessor_node(struct Node** _root, struct Node* _node){
    if(_node->parent == NULL){
        return;
    }

    struct Node* parent_ptr;
    for(parent_ptr = _node ; parent_ptr != NULL ; parent_ptr = parent_ptr->parent){
        if(get_balance_factor(parent_ptr) > 1){ // L type
            printf("node %d has imbalanced subtrees\n", parent_ptr->primary_key);
            if(get_balance_factor(parent_ptr->left_child) == 1){ // L type
                printf("LL type do LL operation\n");
                LL_operation(_root, parent_ptr);
            }
            else{
                printf("LR type do LR operation\n");
                LR_operation(_root, parent_ptr);
            }
            break;
        }
        else if(get_balance_factor(parent_ptr) < -1){ // R type
            printf("node %d has imbalanced subtrees\n", parent_ptr->primary_key);
            if(get_balance_factor(parent_ptr->right_child) == -1){ // R type
                printf("RR type do RR operation\n");
                RR_operation(_root, parent_ptr);
            }
            else{ // L type
                printf("RL type do RL operation\n");
                RL_operation(_root, parent_ptr);
            }
            break;
        }
    }
}

void insert(struct Node** _root, int _data){
    int balance_factor = 0;
    if(!(*_root)){
        printf("root is NULL\n");
        *_root = create_node(NULL, _data);
    }
    else{
        struct Node* ptr = *_root;
        printf("root = %d, depth = %d\n", ptr->primary_key, ptr->depth);
        while(1){
            if(_data > ptr->primary_key){
                if(ptr->right_child == NULL){
                    printf("ptr->r is NULL\n");
                    ptr->right_child = create_node(ptr, _data);
                    printf("after insertion %d\n", _data);
                    ptr = ptr->right_child; // go to leaf which is right child.
                    break;
                }
                else{
                    printf("r = %d, depth = %d\n", ptr->right_child->primary_key, ptr->right_child->depth);
                    ptr = ptr->right_child;
                }
            }
            else if(_data < ptr->primary_key){
                if(ptr->left_child == NULL){
                    printf("ptr->l is NULL\n");
                    ptr->left_child = create_node(ptr, _data);
                    printf("after insertion %d\n", _data);
                    ptr = ptr->left_child; //go to leaf which is left child.
                    break;
                }
                else{
                    printf("l = %d, depth = %d\n", ptr->left_child->primary_key, ptr->left_child->depth);
                    ptr = ptr->left_child;
                }
            }
            else{
                printf("Same key exist\n");
                break;
            }
        }
        trace_backward_update_depth(ptr);
        trace_backward_to_balance_predecessor_node(_root, ptr);
    }
}

struct Node* BuildAVLTree(int* _input_data, int _input_size){
    struct Node* root = NULL;
    for(int i = 0 ; i < _input_size ; i ++){
        printf("input_data[%d] = %d : \n", i, _input_data[i]);
        insert(&root, _input_data[i]);
        printf("\n");
    }
    return root;
}

struct Node* search(struct Node* _root, int _data){
    struct Node* ptr;
    for(ptr = _root ; ptr != NULL ;){
        if(ptr->primary_key == _data){
            printf("found %d, and depth(%d) = %d\n", ptr->primary_key, ptr->primary_key, ptr->depth);
            return ptr;
        }
        else if(ptr->primary_key < _data){
            ptr = ptr->right_child;
        }
        else{
            ptr = ptr->left_child;
        }
    }
    printf("node %d not found\n", _data);
    return NULL;
}

void delete(struct Node** _root, int _data){
    struct Node* aim_ptr = search(*_root, _data);
    if(aim_ptr == NULL){
        return;
    }
    struct Node* aim_ptr_parent = aim_ptr->parent;
    if((aim_ptr->left_child == NULL) && (aim_ptr->right_child == NULL)){ //aim_ptr is leaf
        if(aim_ptr_parent != NULL){ //aim_ptr is not root
            if(aim_ptr_parent->left_child == aim_ptr){
                aim_ptr_parent->left_child = NULL;
            }
            else{
                aim_ptr_parent->right_child = NULL;
            }
            free(aim_ptr);
            update_depth(aim_ptr_parent);
            trace_backward_update_depth(aim_ptr_parent);
            trace_backward_to_balance_predecessor_node(_root, aim_ptr_parent);
        }
        else{ //aim_ptr is root.
            free(aim_ptr);
            *_root = NULL;
        }
    }
    else if((aim_ptr->left_child) && (aim_ptr->right_child == NULL)){ //aim_ptr has left_child but no right_child
        if(aim_ptr_parent != NULL){ //aim_ptr is not root
            if(aim_ptr_parent->left_child == aim_ptr){
                aim_ptr_parent->left_child = aim_ptr->left_child;
            }
            else{
                aim_ptr_parent->right_child = aim_ptr->left_child;
            }
            aim_ptr->left_child->parent = aim_ptr_parent;
            free(aim_ptr);
            update_depth(aim_ptr_parent);
            trace_backward_update_depth(aim_ptr_parent);
            trace_backward_to_balance_predecessor_node(_root, aim_ptr_parent);
        }
        else{ //aim_ptr is root
            aim_ptr->left_child->parent = NULL;
            *_root = aim_ptr->left_child;
            free(aim_ptr);
        }
    }
    else if((aim_ptr->left_child == NULL) && (aim_ptr->right_child)){ //aim_ptr has right_child but no left_child
        if(aim_ptr_parent != NULL){ //aim_ptr is not root
            if(aim_ptr_parent->left_child == aim_ptr){
                aim_ptr_parent->left_child = aim_ptr->right_child;
            }
            else{
                aim_ptr_parent->right_child = aim_ptr->right_child;
            }
            aim_ptr->right_child->parent = aim_ptr_parent;
            free(aim_ptr);
            update_depth(aim_ptr_parent);
            trace_backward_update_depth(aim_ptr_parent);
            trace_backward_to_balance_predecessor_node(_root, aim_ptr_parent);
        }
        else{ //aim_ptr is root
            aim_ptr->right_child->parent = NULL;
            *_root = aim_ptr->right_child;
            free(aim_ptr);
        }
    }
    else{ //aim_ptr has both left_child and right_child
        //find max_key in left subtree
        for(struct Node* left_subtree_max_ptr = aim_ptr->left_child ; left_subtree_max_ptr != NULL ; left_subtree_max_ptr = left_subtree_max_ptr->right_child){
            
        }

        if(aim_ptr_parent != NULL){ //aim_ptr is not root
            
        }
        else{ //aim_ptr is root
        
        }
    }
}

void inorder_traverse(struct Node* _node){
    if(_node == NULL){
        printf("NULL, ");
        return;
    }
    printf("%d, ", _node->primary_key);
    inorder_traverse(_node->left_child);
    inorder_traverse(_node->right_child);
}

int main(){
    int input_data[10] = {10, 20, 40, 32, 39, 49};
    struct Node* root = BuildAVLTree(input_data, 6);
    inorder_traverse(root);
    printf("\n");
    struct Node* temp = search(root, 49);
    // int single_input = 0;
    // while(1){
    //     printf("insert a key : ");
    //     scanf("%d", &single_input);
    //     if(single_input == -1){
    //         break;
    //     }
    //     insert(&root, single_input);
    //     inorder_traverse(root);
    //     printf("\n");
    // }
    
}

