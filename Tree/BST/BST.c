#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    int studentID;
    int score;
    char* name;
    struct Node* left_child;
    struct Node* right_child;
};

void get_row_data(char* _row, char** _name, int* _studentID, int* _score){
    char* token;
    token = strtok(_row, ", ");
    *_name = token;
    printf("name: %s\n", *_name);
    
    token = strtok(NULL, ", ");
    *_studentID = atoi(token);
    printf("studentID = %d\n", *_studentID);

    token = strtok(NULL, ", ");
    *_score = atoi(token);
    printf("score = %d\n", *_score);
}

struct Node* create_node(char* _name, int _studentID, int _score){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->left_child = NULL;
    new_node->right_child = NULL;
    new_node->name = _name;
    new_node->studentID = _studentID;
    new_node->score = _score;
    return new_node;
}

void insert_data(struct Node** root, char* _name, int _studentID, int _score){
    if(*root == NULL){
        *root = create_node(_name, _studentID, _score);
    }
    else{
        struct Node* ptr = *root;
        while(1){
            if(_studentID > ptr->studentID){
                if(ptr->right_child != NULL){
                    ptr = ptr->right_child;
                }
                else{
                    ptr->right_child = create_node(_name, _studentID, _score);
                    break;
                }
            }
            else if(_studentID < ptr->studentID){
                if(ptr->left_child != NULL){
                    ptr = ptr->left_child;
                }
                else{
                    ptr->left_child = create_node(_name, _studentID, _score);
                    break;
                }
            }
            else if(_studentID == ptr->studentID){
                printf("input a same data\n");
                break;
            }
        }
    }
}

void inorder_traversal(struct Node* _root){
    if(_root == NULL){
        printf("NULL, ");
    }
    else{
        printf("%d, ", _root->studentID);
        inorder_traversal(_root->left_child);
        inorder_traversal(_root->right_child);
    }
}

struct Node* BuildBST(char* dataset_path){
    FILE *fptr = fopen(dataset_path, "r");
    if(fptr == NULL){
        printf("Error: Open file\n");
        exit(1);
    }
    char row[256];
    fgets(row, 256, fptr);
    printf("%s\n", row);

    struct Node* root = NULL;
    char* name;
    int studentID;
    int score;
    while(fgets(row, 256, fptr) != NULL){
        get_row_data(row, &name, &studentID, &score);
        printf("\n");
        insert_data(&root, name, studentID, score);
    }
    return root;
}

int main(int argc, char* argv[]){
    char* dataset_path = argv[1];
    printf("dataset_path = %s\n", dataset_path);
    struct Node* root = BuildBST(dataset_path);
    inorder_traversal(root);
    printf("\n");
}