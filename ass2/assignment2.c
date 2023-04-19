#include <stdio.h>
#include <stdlib.h>
// header files for in/out and dynamic memory alloc

typedef struct Node {
  int cur;
  int complNum;
  int finish;
} node;
// defines a struct 'node' that stores info about each node. 
// cur == node's index
// complNum == node's completion number
// finish == node's finishing time


int compare (const void* p1, const void* p2) {
  int x = ((node*)p1)->finish;
  int y = ((node*)p2)->finish;
  return (y - x);
}
// helper function used by qsort to sort the 'finish' array of nodes in desc order


int time = 0;
// init time at 0 to keep track of dfs progress


void dfs(int **graph, int n, int cur, int *visited, node *finish, int complNum) {
  int i;
  time += 1;
  if(visited[cur] == 1) {
    return;
  }

  visited[cur] = 1;
  finish[cur].complNum = complNum; // mark a node as complete
  for(i = 1; i <= n; i++){
    if(graph[cur][i] > 0) {
      dfs(graph, n, i, visited, finish, complNum);
      // recurses if there are unvisited nodes
    }
  }
  finish[cur].finish = time;
  time += 1;
}
/*
  Recursive DFS:
  input:
  - adjacency matrix: 'graph'
  - current node: 'cur'
  - array of visited nodes: 'visited'
  - completion number: 'complNum
  - finish time: 'finish'
  Behavior: Updates the visited array, sets the complNum field of the curr node in the finish array to complNum. Recursively calls itself on unvisited nodes until they are all complete
*/

void initGraph(int*** graph, int n){
  *graph = (int**)malloc(sizeof(int*)*(n+1));
  for(int i = 0; i <= n; i++){
    (*graph)[i] = (int*)calloc(n+1, sizeof(int));
  }
}
// initializes memory to store an adjacency matrix

int loadGraph(int** graph, int m) {
  int a, b, w;
  for(int i = 0; i < m; i++) {
    scanf("%d%d%d", &a, &b, &w);
    graph[a][b] = w;
  }
}
// loads .txt into adjacency matrix

void init_finish(node** finish, int n) {
    *finish = (node*)calloc(n+1, sizeof(node));
    for(int i = 0; i <= n; i++) {
        (*finish)[i].cur = i;
        (*finish)[i].finish = 0;
    }
}
// creates a separate matrix for finish times

void order_topological(int** graph, int n, int* visited, node* finish, int* j, int i) {
    if(visited[i] != 1) {
        dfs(graph, n, i, visited, finish, *j);
        (*j)++;
    }
}
// sorts nodes in topological order

void process_topological(node* finish, int n) {
    qsort((void*)finish, n+1, sizeof(node), compare);
}
// processes nodes in topological order

void init_dp(int*** dp, int n) {
    *dp = (int**)malloc(sizeof(int*)*(n+1));
    for(int i = 0; i <= n; i++) {
        (*dp)[i] = (int*)calloc(2,sizeof(int));
        (*dp)[i][0] = -1;
        (*dp)[i][1] = 0;
    }
}
// init allocation for dp table

int find_node1_index(node* finish, int n) {
    for(int j = 0; j <= n; j++) {
        if(finish[j].cur == 1) {
            return j;
        }
    }
    return -1;
}
// find where the finish time is 1, return it, otw return -1

void relax(int** graph, int** dp, node* finish, int n, int j) {
    dp[1][0] = 0;
    dp[1][1] = 1;
    int cur;
    for(int i = j; i <= n; i++) {
        cur = finish[i].cur;
        if(dp[cur][0] != -1) {
            for(int k = 1; k <= n; k++) {
                if(graph[cur][k] > 0) {
                    if(dp[k][0] < dp[cur][0] + graph[cur][k]) {
                        dp[k][0] = dp[cur][0] + graph[cur][k];
                    }
                }
            }
        }
    }
}
// relax, but backwards

void count_distinct_paths(int** graph, int** dp, node* finish, int n, int j) {
    int cur;
    for(int i = j + 1; i <= n; i++) {
        cur = finish[i].cur;
        for(int k = 1; k <= n; k++) {
            if(graph[k][cur] > 0) {
                if(dp[cur][0] == dp[k][0] + graph[k][cur]) {
                    dp[cur][1] += dp[k][1];
                }
            }
        }
    }
}
// # of distinct path calculations

void print_results(int** dp, int n) {
    if(dp[n][0] > -1) {
        printf("longest path: %d\n", dp[n][0]);
        printf("number of longest paths: %d\n", dp[n][1]);
    } else {
        printf("This node is not reachable.");
}
}
// simple print



int main() {
    int n, m;
    int** graph;
    node* finish;
    int* visited;
    int** dp;
    // init pointers for all functions

    scanf("%d%d", &n, &m);
    // get V and E

    initGraph(&graph, n);
    loadGraph(graph, m);
    // build graphs using V and E

    visited = (int*)calloc(n+1, sizeof(int));
    init_finish(&finish, n);
    // make finish array and visited array

    int j = 0;
    for(int i = 1; i <= n; i++) {
        order_topological(graph, n, visited, finish, &j, i);
    }

    process_topological(finish, n);

    int node1_index = find_node1_index(finish, n);

    init_dp(&dp, n);
    relax(graph, dp, finish, n, node1_index);
    count_distinct_paths(graph, dp, finish, n, node1_index);

    print_results(dp, n);

    for(int i = 0; i <= n; i++) {
        free(graph[i]);
    }
    free(graph);
    free(finish);
    free(visited);
    for(int i = 0; i <= n; i++) {
        free(dp[i]);
    }
    free(dp);
    // free everything~

    return 0;
}
