#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define _FUNCTION_DEBUG_
// #define _MAIN_DEBUG_

void getEachEdge(char* _row, int* _node1, int* _node2){
    char* token;
    
    token = strtok(_row, " ");
    *_node1 = atoi(token);

    token = strtok(NULL, " ");
    *_node2 = atoi(token);
}

void buildTree(char* _datasetPath, int** _coins, int* _coinsSize, int*** _edges, int* _edgesSize){
    FILE* fptr;
    fptr = fopen(_datasetPath, "r");
    if(fptr == NULL){
        printf("[Error] : Can't not open file\n");
        exit(1);
    }
    char row[256];

    // 取得node Number
    fgets(row, 256, fptr);
    int nodeNum = atoi(row);
    #ifdef _FUNCTION_DEBUG_
    printf("nodeNum = %d\n", nodeNum);
    #endif

    // 取得各點是否有coin，並記錄在coins陣列中
    int* coins = (int*)malloc(sizeof(int) * nodeNum);
    for(int nodeID = 0 ; nodeID < nodeNum ; nodeID ++){
        fgets(row, 256, fptr);
        int coinExistence = atoi(row);
        coins[nodeID] = coinExistence;
    }
    #ifdef _FUNCTION_DEBUG_
    for(int nodeID = 0 ; nodeID < nodeNum ; nodeID ++){
        printf("coins[%d] = %d\n", nodeID, coins[nodeID]);
    }
    #endif

    int node1 = -1;
    int node2 = -1;
    int** edges = (int**)malloc(sizeof(int*) * (nodeNum - 1));
    for(int edgeID = 0 ; edgeID < nodeNum - 1 ; edgeID ++){
        fgets(row, 256, fptr);
        getEachEdge(row, &node1, &node2);
        edges[edgeID] = (int*)malloc(sizeof(int) * 2);
        edges[edgeID][0] = node1;
        edges[edgeID][1] = node2;
    }
    #ifdef _FUNCTION_DEBUG_
    for(int edgeID = 0 ; edgeID < nodeNum - 1 ; edgeID ++){
        printf("edge[%d] = (%d, %d)\n", edgeID, edges[edgeID][0], edges[edgeID][1]);
    }
    #endif
    int edgesSize = nodeNum - 1;
    fclose(fptr);

    *_coins = coins;
    *_coinsSize = nodeNum;
    *_edges = edges;
    *_edgesSize = edgesSize;
}

int collectTheCoins(int* coins, int coinsSize, int** edges, int edgesSize, int* edgesColSize){

    int* nodesStatus = (int*)calloc(coinsSize, sizeof(int)); //1代表是parent of coin leaf，2代表必須要踩的點，最後會算全部有幾個2。
    int* nodesDegree = (int*)calloc(coinsSize, sizeof(int));
    int* edgesStatus = (int*)calloc(edgesSize, sizeof(int));

    int node1 = -1;
    int node2 = -1;
    
    if(edgesSize == 1){
        return 0;
    }

    // 取得nodesDegree
    for(int edgeID = 0 ; edgeID < edgesSize ; edgeID ++){
        node1 = edges[edgeID][0];
        node2 = edges[edgeID][1];
        nodesDegree[node1] ++;
        nodesDegree[node2] ++;
    }
    // 初始化adjList
    int** adjList = (int**)malloc(sizeof(int*) * coinsSize);
    int neighborIndex = 0;
    // 用一個queue紀錄準備要折的degree-1
    int* degreeOneQueue = (int*)calloc(coinsSize, sizeof(int));
    int qRear = -1;
    for(int nodeID = 0 ; nodeID < coinsSize ; nodeID ++){
        // 開各點的neighbor空間，第一個neighbor的位置是紀錄index of neighbor in the neighbor list。
        adjList[nodeID] = (int*)calloc(nodesDegree[nodeID] + 1, sizeof(int));
        // 紀錄還未處理的degree-1
        if(nodesDegree[nodeID] == 1){
            qRear ++;
            degreeOneQueue[qRear] = nodeID;
            nodesStatus[nodeID] = -1;
            nodesDegree[nodeID] --;
            printf("nodeID = %d, add into degree one queue\n", nodeID);
        }
    }
    // 建立adjList
    for(int edgeID = 0 ; edgeID < edgesSize ; edgeID ++){
        node1 = edges[edgeID][0];
        node2 = edges[edgeID][1];

        adjList[node1][0] ++;
        neighborIndex = adjList[node1][0];
        adjList[node1][neighborIndex] = node2;

        adjList[node2][0] ++;
        neighborIndex = adjList[node2][0];
        adjList[node2][neighborIndex] = node1;
    }

    // 印出adjList
    for(int nodeID = 0 ; nodeID < coinsSize ; nodeID ++){
        printf("coin = %d, ", coins[nodeID]);
        printf("neighbors[%d] = {", nodeID);
        for(int neighborIndex = 0 ; neighborIndex < adjList[nodeID][0] + 1; neighborIndex ++){
            printf("%d, ", adjList[nodeID][neighborIndex]);
        }
        printf("}\n");
    }

    // 開始連續折degree-1，直到沒有任何degree-1可以折
    int parentOfLeaf = -1;
    int leaf = -1;
    for(int qFront = -1 ; qFront < qRear;){ //不用qFront < qRear + 1，會超過index。
        qFront ++;
        leaf = degreeOneQueue[qFront];
        printf("leaf[%d] = %d, status = %d, coin = %d\n", qFront, leaf, nodesStatus[leaf], coins[leaf]);
        parentOfLeaf = adjList[leaf][1];

        switch(nodesStatus[leaf]){
            case 1:
                if(nodesDegree[leaf] <= 1){
                    nodesStatus[leaf] = -1;
                }
                nodesStatus[parentOfLeaf] = 2;
                nodesDegree[parentOfLeaf] --;
                break;
            case -1:
                if(coins[leaf] == 1 && nodesStatus[parentOfLeaf] == 0){
                    nodesStatus[parentOfLeaf] = 1;
                    printf("\tparent = %d, status = %d\n", parentOfLeaf, nodesStatus[parentOfLeaf]);
                }
                if(nodesStatus[parentOfLeaf] == 0 && nodesDegree[parentOfLeaf] == 2){ //parent是無用的bridge，所以parent.status = -1
                    nodesStatus[parentOfLeaf] = -1;
                }
                //刪除parent的neighbor中的當前leaf(以tailNode覆蓋targetNode)
                for(int neighborIndex = 1 ; neighborIndex < adjList[parentOfLeaf][0] + 1 ; neighborIndex ++){
                    if(adjList[parentOfLeaf][neighborIndex] == leaf){
                        int tailIndex = adjList[parentOfLeaf][0];
                        adjList[parentOfLeaf][neighborIndex] = adjList[parentOfLeaf][tailIndex]; //以tailNode覆蓋targetNode
                        adjList[parentOfLeaf][0] --; //tailIndex - 1，代表有效的neighbor減去一個。
                        break;
                    }
                }
                nodesDegree[parentOfLeaf] --;
                if(nodesDegree[parentOfLeaf] == 1){
                    int parentOfParent = adjList[parentOfLeaf][1];
                    if(nodesStatus[parentOfLeaf] == 1){
                        nodesStatus[parentOfParent] = 2;
                        printf("\t[PL]\t %d's parent %d, status = %d\n", parentOfLeaf, parentOfParent, nodesStatus[parentOfParent]);
                    }

                    printf("\t[L]\t %d's parent %d add into degree one queue\n", leaf, parentOfLeaf);
                    qRear ++;
                    degreeOneQueue[qRear] = parentOfLeaf;
                }
                break;
        }
    }
    
    // 收集status == 2的node
    int needNode = 0;
    for(int nodeID = 0 ; nodeID < coinsSize ; nodeID ++){
        if(nodesStatus[nodeID] == 1 && nodesDegree[nodeID] > 1){
            nodesStatus[nodeID] = 2;
        }
        if(nodesStatus[nodeID] != -1){
            needNode ++;
        }
        printf("status[%d] = %d\n", nodeID, nodesStatus[nodeID]);
    }
    
    int ans = 0;
    if(needNode > 0){
        ans = (needNode - 1) * 2;
    }
    return ans;
}

int main(int argc, char* argv[]){
    char* datasetPath = argv[1];
    
    int* coins;
    int coinsSize;
    int** edges;
    int edgesSize;
    buildTree(datasetPath, &coins, &coinsSize, &edges, &edgesSize);
    int needStep = collectTheCoins(coins, coinsSize, edges, edgesSize, NULL);
    printf("needStep = %d\n", needStep);
    #ifdef _MAIN_DEBUG_
    printf("coinsSize = %d\n", coinsSize);
    printf("edgesSize = %d\n", edgesSize);
    for(int i = 0 ; i < coinsSize ; i ++){
        printf("coins[%d] = %d\n", i, coins[i]);
    }
    for(int i = 0 ; i < edgesSize ; i ++){
        printf("edges[%d] = (%d, %d)\n", i, edges[i][0], edges[i][1]);
    }
    #endif

}