/**
 * @brief The code is for solving the problem of garland.
 * The website about the problem : https://codeforces.com/problemset/problem/767/C
 * @author XPPGX
 * @date 2023/06/29
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node{
    int parent;
    int label;
    int light;
    struct Node* left_child;
    struct Node* right_child;
};

void Get_Row_Data(char* _row, int* _node_parent, int* _node_light){
    char* token;
    
    token = strtok(_row, " ");
    *_node_parent = atoi(token);
    printf("parent = %d, ", *_node_parent);

    token = strtok(NULL, " ");
    *_node_light = atoi(token);
    printf("node_light = %d\n", *_node_light);
}

struct Node* Build_Tree(char* _dataset, struct Node** _nodes, int* _total_light, int* _threshold){
    FILE* fptr;
    fptr = fopen(_dataset, "r");
    if(fptr == NULL){
        printf("Error: Open file\n");
        exit(1);
    }
    char row[256];
    fgets(row, 256, fptr);
    printf("%s\n", row);
    int nodes_num = atoi(row);
    printf("nodes_num = %d\n", nodes_num);
    
    //construct nodes info
    struct Node* nodes = (struct Node*)malloc(sizeof(struct Node) * (nodes_num + 1));
    *_nodes = nodes; //把array傳出去。
    int node_parent = 0;
    int node_light = 0;
    for(int node_label = 1 ; node_label <= nodes_num ; node_label++){ //O(N)
        fgets(row, 256, fptr);
        Get_Row_Data(row, &node_parent, &node_light);
        nodes[node_label].parent = node_parent;
        nodes[node_label].label = node_label;
        nodes[node_label].light = node_light;
        *_total_light = *_total_light + node_light;
        nodes[node_label].left_child = NULL;
        nodes[node_label].right_child = NULL;
    }

    //check if it can be devided into 3 partitions
    if(*_total_light % 3 != 0){
        printf("the garland cannot be devided into 3 partitions\n");
        exit(1);
    }
    else{
        *_threshold = *_total_light / 3;
    }

    //connect parents and childs
    struct Node* root;
    int current_parent = -1;
    printf("\n");
    for(int node_label = 1 ; node_label <= nodes_num ; node_label++){ //O(N)
        printf("current_node = %d, ", node_label);
        current_parent = nodes[node_label].parent;
        if(current_parent == 0){
            root = &nodes[node_label];
            continue;
        }
        if(nodes[current_parent].left_child == NULL){
            nodes[current_parent].left_child = &nodes[node_label];
            printf("current_parent = %d, left(%d) = %d\n", current_parent, current_parent, nodes[node_label].label);
            continue;
        }
        if(nodes[current_parent].right_child == NULL){
            nodes[current_parent].right_child = &nodes[node_label];
            printf("current_parent = %d, right(%d) = %d\n", current_parent, current_parent, nodes[node_label].label);
            continue;
        }
    }
    fclose(fptr);
    return root;
}

int Trace_Up_Operation(struct Node* _node, int _current_total_light, int _threshold, int* _cut_node1, int* _cut_node2){
    if(_current_total_light < _threshold){
        printf("current_node = %d, should keep trace up\n", _node->label);
        return _current_total_light;
    }
    else if(_current_total_light > _threshold){
        /**
         * 如果要切的話，在_node的child就會切了，會到這裡是因為_node的child也都無法當成獨立的partition。
         * _node的child都無法當成獨立的partition，而累積light到_node這裡又會>threshold，
         * 造成child必須要切(因為_node.light > threshold)，而child又不能切(因為child不足以當成獨立的partition)。
         * 所以沒有任何切法滿足這個樹。
         * */
        printf("current_node = %d, current_total_light = %d\n", _node->label, _current_total_light);
        printf("ans = -1\n");
        exit(1); //exit the program
    }
    else{ //current_total_light == threshold
        printf("!!!!! cut node %d, current_total_light = %d\n\n\n", _node->label, _current_total_light);
        if(*_cut_node1 == -1){
            *_cut_node1 = _node->label;
        }
        else{
            if(*_cut_node2 == -1){
                *_cut_node2 = _node->label;
            }
        }
        return 0;
    }
}


int Find_Nodes_Should_Be_Cut(struct Node* _root, int _threshold, int* _cut_node1, int* _cut_node2){
    printf("current_node = %d, light(%d) = %d\n", _root->label, _root->label, _root->light);
    int current_total_light = 0;
    if(_root->left_child != NULL && _root->right_child != NULL){
        current_total_light = _root->light + Find_Nodes_Should_Be_Cut(_root->left_child, _threshold, _cut_node1, _cut_node2) + Find_Nodes_Should_Be_Cut(_root->right_child, _threshold, _cut_node1, _cut_node2);
        printf("current_node = %d, total_light = %d\n", _root->label, current_total_light);
        return Trace_Up_Operation(_root, current_total_light, _threshold, _cut_node1, _cut_node2);
        //return current_total_light;
    }
    else if(_root->left_child != NULL && _root->right_child == NULL){
        current_total_light = _root->light + Find_Nodes_Should_Be_Cut(_root->left_child, _threshold, _cut_node1, _cut_node2);
        printf("current_node = %d, total_light = %d\n", _root->label, current_total_light);
        return Trace_Up_Operation(_root, current_total_light, _threshold, _cut_node1, _cut_node2);
        //return current_total_light;
    }
    else if(_root->left_child == NULL && _root->right_child != NULL){
        current_total_light = _root->light + Find_Nodes_Should_Be_Cut(_root->right_child, _threshold, _cut_node1, _cut_node2);
        printf("current_node = %d, total_light = %d\n", _root->label, current_total_light);
        return Trace_Up_Operation(_root, current_total_light, _threshold, _cut_node1, _cut_node2);
        //return current_total_light;
    }
    else{
        current_total_light = _root->light;
        printf("current_node = %d, total_light = %d\n", _root->label, current_total_light);
        return Trace_Up_Operation(_root, current_total_light, _threshold, _cut_node1, _cut_node2);
        //return current_total_light;
    }
}

/**
 * @brief Get the lights of subtree which is rooted at _node
 * @param _node any node in the tree
 * @return The total lights of any subtree
*/
int Light_Of_Tree(struct Node* _node){
    if(_node->left_child != NULL && _node->right_child != NULL){
        return _node->light + Light_Of_Tree(_node->left_child) + Light_Of_Tree(_node->right_child);
    }
    else if(_node->left_child != NULL && _node->right_child == NULL){
        return _node->light + Light_Of_Tree(_node->left_child);
    }
    else if(_node->left_child == NULL && _node->right_child != NULL){
        return _node->light + Light_Of_Tree(_node->right_child);
    }
    else{
        return _node->light;
    }
}

void check_ans(int _sum_light_of_node1, int _sum_light_of_node2, int _threshold){
    if(_sum_light_of_node1 == _threshold && _sum_light_of_node2 == _threshold){
        printf("anwser is correct\n");
    }
}

void Post_Order_Traverse_Print_Label(struct Node* _root){
    if(_root == NULL){
        printf("NULL, ");
        return;
    }
    Post_Order_Traverse_Print_Label(_root->left_child);
    Post_Order_Traverse_Print_Label(_root->right_child);
    printf("%d, ", _root->label);
}

int main(int argc, char* argv[]){
    char* dataset = argv[1];
    printf("dataset = %s\n", dataset);
    int total_light = 0;
    int threshold = 0;
    struct Node* nodes_arr;
    struct Node* root = Build_Tree(dataset, &nodes_arr, &total_light, &threshold);
    int cut_node1 = -1;
    int cut_node2 = -1;
    printf("=============\n");
    printf("root = %d, addr(root) = %p, root->left_child = %p, root->right_child = %p\n", root->label, root, root->left_child, root->right_child);
    printf("left = %d, right = %d\n", root->left_child->label, root->right_child->label);
    printf("root = %d: \n", root->label);
    printf("\troot->parent = %d\n", root->parent);
    printf("\troot->light = %d\n", root->light);
    Post_Order_Traverse_Print_Label(root);
    printf("\n");
    printf("\n\n=======================\n");
    printf("==Checking...\n");
    int lights = Find_Nodes_Should_Be_Cut(root, threshold, &cut_node1, &cut_node2);
    printf("lights = %d\n", lights);

    printf("======anwser======\n");
    printf("cut_node1 = %d, cut_node2 = %d\n", cut_node1, cut_node2);
    int cummulative_light_node1 = Light_Of_Tree(&nodes_arr[cut_node1]);
    int cummulative_light_node2 = Light_Of_Tree(&nodes_arr[cut_node2]);
    printf("cummulative_light(%d) = %d, cummulative_light(%d) = %d\n", cut_node1, cummulative_light_node1, cut_node2, cummulative_light_node2);
    check_ans(cummulative_light_node1, cummulative_light_node2, threshold);
}