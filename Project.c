#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#define INFINITY INT_MAX

// Structure representing a graph node
typedef struct{
    int index;
    int sum;
} Graph;

// Prints the indices of the top-k elements in the list
void printTopK(Graph topK[], int k, int idx){
    k--;
    idx--;
    if(idx == -1){
        printf("\n");
    }
    else if(idx < k){
        while(idx > 0){
            printf("%d ", topK[idx].index);
            idx--;
        }
        printf("%d\n", topK[idx].index);
    }
    else{
        while(k > 0){
            printf("%d ", topK[k].index);
            k--;
        }
        printf("%d\n", topK[k].index);
    }
    return;
}

// Maintains the max-heap property
void maxHeapify(Graph topK[], int i, int size){
    int left, right, largest;
    left = 2 * i + 1;
    right = 2 * i + 2;
    largest = i;
    Graph temp;

    if(left < size && topK[left].sum > topK[i].sum){
        largest = left;
    }
    if(right < size && topK[right].sum > topK[largest].sum){
        largest = right;
    }
    if(largest != i){
        temp = topK[i];
        topK[i] = topK[largest];
        topK[largest] = temp;
        maxHeapify(topK, largest, size);
    }
    return;
}

// Builds a max-heap from an array
void createMaxHeap(Graph topK[], int k){
    for(int i = (k - 1) / 2; i >= 0; i--){
        maxHeapify(topK, i, k);
    }
    return;
}

// Sorts an array using heapsort
void heapSort(Graph topK[], int k){
    Graph temp;
    int size = k;
    createMaxHeap(topK, k);
    for(int i = k - 1; i >= 1; i--){
        temp = topK[i];
        topK[i] = topK[0];
        topK[0] = temp;
        size--;
        maxHeapify(topK, 0, size);
    }
    return;
}

// Maintains the min-heap property
void minHeapify(Graph nodes[], int i, int size){
    int left, right, smallest;
    left = 2 * i + 1;
    right = 2 * i + 2;
    smallest = i;
    Graph temp;

    if(left < size && nodes[left].sum < nodes[i].sum){
        smallest = left;
    }
    if(right < size && nodes[right].sum < nodes[smallest].sum){
        smallest = right;
    }
    if(smallest != i){
        temp = nodes[i];
        nodes[i] = nodes[smallest];
        nodes[smallest] = temp;
        minHeapify(nodes, smallest, size);
    }
    return;
}

// Builds a min-heap from an array
void createMinHeap(Graph nodes[], int size){
    for(int i = (size - 1) / 2; i >= 0; i--){
        minHeapify(nodes, i, size);
    }
    return;
}

// Calculates the sum of the shortest paths in a graph and returns the result
int addGraph(int d){
    int matrix[d][d];
    int r, c, i, total, size;
    Graph nodes[d], minNode;
    char x, buffer[15];
    total = 0;
    i = 0;
    r = 0;
    c = 0;
    size = d - 1;
    x = 0;
    minNode.sum = INFINITY;
    minNode.index = 0;

    // Read adjacency matrix input
    getchar();
    for(r = 0; r < d; r++){
        for(c = 0; c < d; c++){
            while(x != ',' && x != '\n'){
                x = getchar_unlocked();
                buffer[i] = x;
                i++;
            }
            x = 0;
            matrix[r][c] = atoi(buffer);
            i = 0;
        }
        nodes[r].sum = INFINITY;
        nodes[r].index = r;
    }
    nodes[0].sum = 0;

    // Process graph nodes using Dijkstra-like approach
    while(size >= 0){
        minNode = nodes[0];
        nodes[0] = nodes[size];
        if(minNode.sum == INFINITY){
            break;
        }
        total += minNode.sum;
        for(i = 0; i < size; i++){
            if(matrix[minNode.index][nodes[i].index] > 0){
                if(nodes[i].sum > matrix[minNode.index][nodes[i].index] + minNode.sum){
                    nodes[i].sum = matrix[minNode.index][nodes[i].index] + minNode.sum;
                }
            }
        }
        createMinHeap(nodes, size);
        size--;
    }
    return total;
}

int main(){
    int d, k, i, dec, minVal, idx;
    Graph newGraph;
    char command[14], c;
    idx = 0;
    c = 0;
    i = 0;
    minVal = 0;
    dec = 1;

    // Read dimensions and k
    while(c != ' '){
        c = getchar_unlocked();
        command[idx++] = c;
    }
    d = atoi(command);
    idx = 0;
    while(c != '\n'){
        c = getchar_unlocked();
        command[idx++] = c;
    }
    idx = 0;
    k = atoi(command);
    Graph topK[k];

    // Process commands
    while(fgets(command, 14, stdin) != NULL){
        if(command[0] == 'A'){ // If the command is "Add Graph"
            newGraph.sum = addGraph(d);
            newGraph.index = idx;

            if(idx < k){
                topK[idx] = newGraph;
                if(idx >= 1 && newGraph.sum >= topK[idx - 1].sum){
                    dec = 0;
                }
                if(dec == 1){
                    minVal = topK[k - 1].sum;
                }
            }
            else{ // When top-k is already full
                if(dec == 1){
                    if(newGraph.sum < minVal){
                        if(i < k){
                            topK[i] = newGraph;
                            i++;
                            minVal = newGraph.sum;
                        }
                        else if(i == k){
                            i = 0;
                            topK[i] = newGraph;
                            i++;
                            minVal = newGraph.sum;
                        }
                    }
                    else{
                        dec = 0;
                        if(newGraph.sum < topK[0].sum){
                            topK[0] = newGraph;
                            heapSort(topK, k);
                        }
                    }
                }
                else{
                    heapSort(topK, k);
                    if(newGraph.sum < topK[k - 1].sum){
                        topK[k - 1] = newGraph;
                    }
                }
            }
            idx++;
        }
        else{ // Print top-k
            printTopK(topK, k, idx);
        }
    }
    return 0;
}
