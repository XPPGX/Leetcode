#include <stdio.h>
#include <stdlib.h>
#include <set>
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <string.h>
using namespace std;

//#define _DEBUG__COMMON_
//#define _DEBUG_FAIL_DATASET_
#define _DEBUG_SUCC_DATASET_
struct Node{
    int light;
    int parent;
    int remain_child;
    int left_child;
    int right_child;
};

vector<int> get_randoms_sequence_from_vector(int _nodes_num){
    vector<int> random_sequence;
    for(int i = 1 ; i < _nodes_num + 1 ; i ++){
        random_sequence.push_back(i);
        #ifdef _DEBUG__COMMON_
        printf("vector[%d] = %d\n", i-1, random_sequence[i-1]);
        #endif
    }
    auto seed = std :: chrono :: system_clock :: now().time_since_epoch().count();
    shuffle(random_sequence.begin(), random_sequence.end(), default_random_engine(seed));
    #ifdef _DEBUG__COMMON_
    printf("random sequence = {");
    for(const auto &node : random_sequence){
        printf("%d, ", node);
    }
    printf("}\n");
    #endif
    return random_sequence;
}

/**
 * Part 1. 創點 => OK.
 * Part 2. 連接parent與child => OK.
 * Part 3. 隨機選擇一個點z(非root).
 * Part 4. 讓以z為root的tree Tz 的light總和>(total_light / 3)，並且assign給Tz的每個node一個light.
 * Part 5. 剩下的點隨便賦予light.
 * Part 6. 輸出dataset.txt.
*/
void create_fail_dataset(int _nodes_num, int _total_light, int _max_single_light, const char* _choose_parent_or_leaf, const char* _dataset_path){
    #pragma region part1
    struct Node* nodes_info = (struct Node*)malloc(sizeof(struct Node) * (_nodes_num + 1));
    for(int i = 1 ; i < _nodes_num + 1 ; i ++){
        nodes_info[i].light = -1;
        nodes_info[i].remain_child = 2;
        nodes_info[i].left_child = -1;
        nodes_info[i].right_child = -1;
    }
    vector<int> random_sequence = get_randoms_sequence_from_vector(_nodes_num);
    #pragma endregion

    #pragma region part2
    //contain the leaf nodes that can connect 1 or 2 child.
    //If there is no space to connect child, the node will be remove.
    set<int> accept_child_nodes;
    set<int> :: iterator it;
    set<int> parent_nodes;
    set<int> all_nodes;
    set<int> leaf_nodes;
    int root = random_sequence[0];
    printf("root = %d\n", root);
    accept_child_nodes.insert(root);
    nodes_info[root].parent = 0;
    for(int i = 1, node = 0, choosed_parent = 0 ; i < random_sequence.size() ; i ++){
        node = random_sequence[i];
        it = accept_child_nodes.begin();
        advance(it, rand() % accept_child_nodes.size());
        choosed_parent = *it;
        if(nodes_info[choosed_parent].remain_child == 2){
            nodes_info[node].parent = choosed_parent;
            nodes_info[choosed_parent].remain_child--;
            nodes_info[choosed_parent].left_child = node;
        }
        else if(nodes_info[choosed_parent].remain_child == 1){
            nodes_info[node].parent = choosed_parent;
            nodes_info[choosed_parent].remain_child--;
            accept_child_nodes.erase(choosed_parent);
            nodes_info[choosed_parent].right_child = node;
        }
        if(choosed_parent != root){
            parent_nodes.insert(choosed_parent);
        }
        accept_child_nodes.insert(node);
        all_nodes.insert(node);
        #ifdef _DEBUG_FAIL_DATASET_
        cout << "node = " << node << ", parent = " << choosed_parent << endl;
        #endif
    }
    set_difference(all_nodes.begin(), all_nodes.end(), parent_nodes.begin(), parent_nodes.end(), inserter(leaf_nodes, leaf_nodes.begin())); //差集
    #ifdef _DEBUG_FAIL_DATASET_
    printf("\n");
    printf("parent nodes : {");
    for(const auto &node : parent_nodes){
        printf("%d, ", node);
    }
    printf("}\n");
    printf("leaf nodes : {");
    for(const auto &node : leaf_nodes){
        printf("%d, ", node);
    }
    printf("}\n");
    #endif
    #pragma endregion

    #pragma region part3
    int choosed_fail_node = 0;
    if(strcmp(_choose_parent_or_leaf, "parent") == 0){ // _choose_parent_or_leaf == "parent", choose one parent as fail node
        it = parent_nodes.begin();
        advance(it, rand() % parent_nodes.size());
        choosed_fail_node = *it;
        printf("!!!choose %d as fail root from parent_nodes\n", choosed_fail_node);
    }
    else if(strcmp(_choose_parent_or_leaf, "leaf") == 0){ // _choose_parent_or_leaf == "leaf", choose one leaf as fail node
        it = leaf_nodes.begin();
        advance(it, rand() % leaf_nodes.size());
        choosed_fail_node = *it;
        printf("!!!choose %d as fail root from leaf_nodes\n", choosed_fail_node);
    }
    else{
        cout << "choose the forth argument to select \"parent\" or \"leaf\"\n";
        exit(1);
    }
    #pragma endregion
    
    #pragma region part4
    int threshold = _total_light / 3;
    srand(time(NULL));
    int fail_tree_light = threshold + rand() % 10 + 1;
    _total_light = _total_light - fail_tree_light; //更新_total_light
    printf("fail_tree_light = %d\n\n", fail_tree_light);

    set<int> fail_tree;
    set<int> remain_tree;
    for(int i = 1 ; i < _nodes_num + 1 ; i ++){
        remain_tree.insert(i);
    }
    fail_tree.insert(choosed_fail_node);
    remain_tree.erase(choosed_fail_node);

    int fail_node;
    int fail_light;
    while(!fail_tree.empty()){
        it = fail_tree.begin();
        advance(it, rand() % fail_tree.size());
        fail_node = *it;
        if(nodes_info[fail_node].left_child != -1){
            fail_tree.insert(nodes_info[fail_node].left_child);
            remain_tree.erase(nodes_info[fail_node].left_child);
        }
        if(nodes_info[fail_node].right_child != -1){
            fail_tree.insert(nodes_info[fail_node].right_child);
            remain_tree.erase(nodes_info[fail_node].right_child);
        }
        if(fail_tree.size() == 1 && nodes_info[fail_node].left_child == -1 && nodes_info[fail_node].right_child == -1){
            nodes_info[fail_node].light = fail_tree_light;
            break;
        }

        nodes_info[fail_node].light = (rand() % fail_tree_light) % _max_single_light;
        fail_tree_light = fail_tree_light - nodes_info[fail_node].light;
        fail_tree.erase(fail_node);
    }
    #ifdef _DEBUG_FAIL_DATASET_
    printf("------------------------\n");
    printf("SHOW NODES INFO OF FAIL PART......\n");
    for(int i = 1 ; i < _nodes_num + 1; i ++){
        printf("nodes_info[%d].light = %d, parent = %d\n", i, nodes_info[i].light, nodes_info[i].parent);
    }
    #endif
    #pragma endregion

    #pragma region part5
    int remain_node;
    #ifdef _DEBUG_FAIL_DATASET_
    printf("remain_tree : {");
    for(const auto &node : remain_tree){
        printf("%d, ", node);
    }
    printf("}\n");
    #endif
    while(remain_tree.size() != 1){
        it = remain_tree.begin();
        advance(it, rand() % remain_tree.size());
        remain_node = *it;
        nodes_info[remain_node].light = (rand() % _total_light) % _max_single_light;
        #ifdef _DEBUG_FAIL_DATASET_
        printf("remain_node = %d, light = %d, total_light = %d\n", remain_node, nodes_info[remain_node].light, _total_light);
        #endif
        _total_light = _total_light - nodes_info[remain_node].light;
        remain_tree.erase(remain_node);
    }
    it = remain_tree.begin();
    advance(it, rand() % remain_tree.size());
    remain_node = *it;
    nodes_info[remain_node].light = _total_light;
    #ifdef _DEBUG_FAIL_DATASET_
    printf("remain_node = %d, light = %d\n", remain_node, nodes_info[remain_node].light);

    
    printf("------------------------\n");
    printf("SHOW ALL NODE INFO......\n");
    for(int i = 1 ; i < _nodes_num + 1; i ++){
        printf("nodes_info[%d].light = %d, parent = %d\n", i, nodes_info[i].light, nodes_info[i].parent);
    }
    #endif
    #pragma endregion

    #pragma region part6
    ofstream writer;
    writer.open(_dataset_path);
    if(!writer.is_open()){
        printf("Error : writer can't open file\n");
        exit(1);
    }
    for(int i = 0 ; i < _nodes_num + 1; i++){
        if(i == 0){
            writer << _nodes_num;
        }
        else{
            writer << endl;
            writer << nodes_info[i].parent << " " << nodes_info[i].light;
        }
    }
    #pragma endregion
}

/**
 * Part 1. 創點
 * Part 2. 連接parent與child
 * Part 3. 隨機選擇一個點a(非root)
 * Part 4. 讓以a為root的tree Ta 的light總和=(total_light/3)，並且assign給Ta的每個node一個light
 * Part 5. 排除所有Ta中的點，剩下的點再選一個點b(非root)
 * Part 6. 讓以b為root的tree Tb 的light總和=(total_light/3)，並且assign給Tb的每個node一個light
 * Part 7. assign剩下的node一個light
*/
void create_succ_dataset(int _nodes_num, int _total_light, int _max_single_light, const char* _dataset_path){
    #pragma region part1
    struct Node* nodes_info = (struct Node*)malloc(sizeof(struct Node) * (_nodes_num + 1));
    for(int i = 1 ; i < _nodes_num + 1 ; i ++){
        nodes_info[i].light = -1;
        nodes_info[i].remain_child = 2;
        nodes_info[i].left_child = -1;
        nodes_info[i].right_child = -1;
    }
    vector<int> random_sequence = get_randoms_sequence_from_vector(_nodes_num);
    #pragma endregion

    #pragma region part2
    
    #pragma endregion
}

int main(int argc, char* argv[]){
    char* mode = argv[1];
    int nodes_num = atoi(argv[2]);
    int total_light = atoi(argv[3]);
    int max_single_light = atoi(argv[4]);
    if(strcmp(mode, "fail") == 0){
        char* choose_parent_or_leaf = argv[5];
        char* dataset_path = argv[6];
        create_fail_dataset(nodes_num, total_light, max_single_light, choose_parent_or_leaf, dataset_path);
    }
    else if(strcmp(mode, "success") == 0){
        char* dataset_path = argv[5];

    }
}