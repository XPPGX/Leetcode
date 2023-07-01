#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    char person;
    int age;
    int married;
    struct Node* sibling;
    struct Node* parent;
    struct Node* child;
};

void get_row_data(char* _row, char* _person, int* _age, int* _married, char* temp_parent){
    char* token;
    token = strtok(_row, ", ");
    *_person = token[0];
    printf("person : %c, ", *_person);
    
    token = strtok(NULL, ", ");
    *_age = atoi(token);
    printf("age = %d, ", *_age);

    token = strtok(NULL, ", ");
    *_married = atoi(token);
    printf("married = %d, ", *_married);

    token = strtok(NULL, ", ");
    *temp_parent = token[0];
    printf("temp_parent = %c", *temp_parent);

    printf("\n");
}

struct Node* create_node(struct Node* parent, char _person, int _age, int _married){
    struct Node* new_node = (struct Node*)malloc(sizeof(struct Node));
    new_node->person = _person;
    new_node->age = _age;
    new_node->married = _married;
    new_node->sibling = NULL;
    new_node->child = NULL;
    new_node->parent = parent;
    return new_node;
}

struct Node* inorder_query(struct Node* _root, char _person){
    if(_root == NULL){
        printf("NULL, ");
        return NULL;
    }
    else{
        printf("%c, ", _root->person);
        if(_root->person == _person){
            printf("find\n");
            return _root;
        }
        else{
            inorder_query(_root->child, _person);
            inorder_query(_root->sibling, _person);
        }
        
    }
}

struct Node* BuildTree(char* s){
    FILE* fptr;
    fptr = fopen(s, "r");
    if(fptr == NULL){
        printf("Error : Open file\n");
        exit(1);
    }
    char row[255];
    fgets(row, 255, fptr); //remove the row of metadata

    char person;
    int age;
    int married;
    char temp_parent;

    struct Node* root;
    fgets(row, 255, fptr);
    get_row_data(row, &person, &age, &married, &temp_parent);
    root = create_node(NULL, person, age, married);
    printf("root : person : %c, age = %d, married = %d\n\n", root->person, root->age, root->married);
    struct Node* ptr;
    while(fgets(row, 255, fptr) != NULL){
        get_row_data(row, &person, &age, &married, &temp_parent);
        printf("query(%c): ", temp_parent);
        ptr = inorder_query(root, temp_parent);
        printf("\n");
        if(ptr->child == NULL){
            ptr->child = create_node(ptr, person, age, married);
        }
        else{
            while(ptr->sibling != NULL){
                ptr = ptr->sibling;
            }
            ptr->sibling = create_node(ptr, person, age, married);
        }
        printf("\n");
    }
    fclose(fptr);
    return NULL;
}

int main(int argc, char* argv[]){
    char *s = argv[1];
    printf("dataset path = %s\n", s);
    BuildTree(s);
}