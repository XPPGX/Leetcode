#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 解法：
// 在讀入每個node的時候，就先把light累加在從root到leaf的path上的每個node。
// 只要去搜尋node的累加亮度，就可以知道該不該切這個node。
// cut一個node之後，要再更新路徑上的所有node的cummulative light，才會維持正確的值。


// 結構：
// 可以用array of structure去存node跟亮度

// #define _DEBUG_

struct Node{
    int leftChildID;
    int rightChildID;
    int parentID;
    int light;
    int cummulativeLight;
};

void Get_Row_Data(char* _row, int* _node_parent, int* _node_light){
    char* token;
    
    token = strtok(_row, " ");
    *_node_parent = atoi(token);

    #ifdef _DEBUG_
    printf("parent = %d, ", *_node_parent);
    #endif

    token = strtok(NULL, " ");
    *_node_light = atoi(token);

    #ifdef _DEBUG_
    printf("node_light = %d\n", *_node_light);
    #endif
}

struct Node* buildTree(char* _datasetPath, int* _rootID, int* _totalLight){
    FILE* fptr;
    fptr = fopen(_datasetPath, "r");
    if(fptr == NULL){
        printf("Error: Open file\n");
        exit(1);
    }
    // 取得nodeNum
    char row[256];
    fgets(row, 256, fptr);
    int nodeNum = atoi(row);
    printf("nodeNum = %d\n", nodeNum);

    // 初始化treeArray
    struct Node* treeArray = (struct Node*)malloc(sizeof(struct Node) * (nodeNum + 1)); //nodeID從1開始，所以0號位置是空的。
    for(int nodeID = 1 ; nodeID <= nodeNum ; nodeID ++){
        treeArray[nodeID].parentID = -1;
        treeArray[nodeID].leftChildID = -1;
        treeArray[nodeID].rightChildID = -1;
        treeArray[nodeID].light = 0;
        treeArray[nodeID].cummulativeLight = 0;
    }

    // 讀取dataset以建立treeArray
    int totalLight = 0;
    int nodeParent = 0;
    int nodeLight = 0;
    for(int nodeID = 1 ; nodeID <= nodeNum ; nodeID ++){
        nodeParent = 0;
        nodeLight = 0;
        fgets(row, 256, fptr);
        Get_Row_Data(row, &nodeParent, &nodeLight);
        // 紀錄totalLight，如果totalLight不能夠平分成三等份，則ans回傳-1。
        totalLight += nodeLight;
        // assign值給新node
        treeArray[nodeID].parentID = nodeParent;
        if(nodeParent == 0){
            *_rootID = nodeID;
        }
        treeArray[nodeID].light = nodeLight;
        treeArray[nodeID].cummulativeLight = nodeLight;
        // 讓parent的左子或右子接上新leaf
        if(treeArray[nodeParent].leftChildID == -1){ //該parent還沒有left child
            treeArray[nodeParent].leftChildID = nodeID;
        }
        else if(treeArray[nodeParent].rightChildID == -1){//該parent已有left child，但沒有right child
            treeArray[nodeParent].rightChildID = nodeID;
        }
        // 先向下取得左子與右子的cummulative light，累加於自己。
        if(treeArray[nodeID].leftChildID != -1){ //左子存在，取得左子的cummulative light
            int leftChild = treeArray[nodeID].leftChildID;
            treeArray[nodeID].cummulativeLight += treeArray[leftChild].cummulativeLight;
        }
        if(treeArray[nodeID].rightChildID != -1){ //右子存在，取得右子的cummulative light
            int rightChild = treeArray[nodeID].rightChildID;
            treeArray[nodeID].cummulativeLight += treeArray[rightChild].cummulativeLight;
        }
        // 再向上走到root，並修改path上的每個node的cummulative light
        while(nodeParent > 0){
            treeArray[nodeParent].cummulativeLight += treeArray[nodeID].cummulativeLight;
            nodeParent = treeArray[nodeParent].parentID;
        }
    }
    *_totalLight = totalLight;
    printf("totalLight = %d\n", totalLight);

    #ifdef _DEBUG_
    for(int nodeID = 1 ; nodeID <= nodeNum ; nodeID ++){
        printf("light[%d] = %d, cummulativeLight[%d] = %d\n", nodeID, treeArray[nodeID].light, nodeID, treeArray[nodeID].cummulativeLight);
    }
    #endif

    return treeArray;
}

void PreorderTraverse(struct Node* _treeArray, int _currentNode, int _totalLight, int* _cutNode){
    if(_currentNode == -1){

        #ifdef _DEBUG_
        printf("bottom\n");
        #endif

        return;
    }

    #ifdef _DEBUG_
    printf("node = %d, cummulativeLight[%d] = %d\n", _currentNode, _currentNode, _treeArray[_currentNode].cummulativeLight);
    #endif

    if(_treeArray[_currentNode].cummulativeLight >  (_totalLight / 3)){ //case 1. cummulativeLight > (1/3) * totalLight，則繼續Preorder的traverse
        PreorderTraverse(_treeArray, _treeArray[_currentNode].leftChildID, _totalLight, _cutNode);
        if(*_cutNode > 0){//於左子樹中，找到cutNode
            return;
        }
        PreorderTraverse(_treeArray, _treeArray[_currentNode].rightChildID, _totalLight, _cutNode);
        if(*_cutNode > 0){ //於右子樹中，找到cutNode
            return;
        }

        // 如果左右子樹找完，cutNode還是0，代表不可能切。
        if(*_cutNode == 0){
            return;
        }
    }
    else if(_treeArray[_currentNode].cummulativeLight == (_totalLight / 3)){ //case 2. cummulativeLight == (1/3) * totalLight，則cut該點，並更新所有從該點到root的path上的node的cummulativeLight。
        *_cutNode = _currentNode;
        printf("currentNode = %d, should be cut\n", _currentNode);
        return;
    }
    else{ //case 3. cummulativeLight < (1/3) * totalLight，則返回。因為是Preorder，所以左子如果case3，則往右子traverse。如果左右子都case3，則return -1代表沒有方法可以切。
        return;
    }
}

void afterCutNode(struct Node* _treeArray, int _cutNodeID){
    // cut該點，即把parent的對應child assign成-1
    int parentID = _treeArray[_cutNodeID].parentID;
    if(_treeArray[parentID].leftChildID == _cutNodeID){ //_cutNodeID是parent的左子
        _treeArray[parentID].leftChildID = -1;
    }
    else{ //_cutNodeID是parent的右子
        _treeArray[parentID].rightChildID = -1;
    }

    while(parentID > 0){
        _treeArray[parentID].cummulativeLight -= _treeArray[_cutNodeID].cummulativeLight;
        parentID = _treeArray[parentID].parentID;
    }
}

void findCutNode(struct Node* _treeArray, int _rootID, int _totalLight){
    // 使用Preorder進行traverse
    // 如果traverse到當前的點：
    int cutNode = 0;
    PreorderTraverse(_treeArray, _rootID, _totalLight, &cutNode);
    printf("cutNode = %d\n", cutNode);
    if(cutNode > 0){
        afterCutNode(_treeArray, cutNode);
    }
    else{//沒有任何切割方法
        printf("ans = -1 (No way to cut)\n");
        return;
    }

    cutNode = 0;
    PreorderTraverse(_treeArray, _rootID, _totalLight, &cutNode);
    printf("cutNode = %d\n", cutNode);
    if(cutNode > 0){
        afterCutNode(_treeArray, cutNode);
    }
    else{//沒有任何切割方法
        printf("ans = -1 (No way to cut)\n");
        return;
    }
}



int main(int argc, char* argv[]){
    char* datasetPath = argv[1];
    int rootID = -1;
    int totalLight = 0;
    struct Node* treeArray = buildTree(datasetPath, &rootID, &totalLight);
    if(totalLight % 3 != 0){
        printf("ans = -1 (the garland can't be devided into 3 partitions\n");
        return 0;
    }
    findCutNode(treeArray, rootID, totalLight);
}