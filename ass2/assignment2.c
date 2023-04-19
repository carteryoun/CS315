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

int time = 0;
// init time to 0 to keep track of DFS progress

int compare (const void* p1, const void* p2) {
  int x = ((node*)p1)->finish;
  int y = ((node*)p2)->finish;
  return (y - x);
}
// helper function used by qsort to sort the 'finish' array of nodes in desc order

void dfs(int **graph, int n, int cur, int *visited, node *finish, int complNum) {
  int i;
  time += 1;
  if(visited[cur] == 1) {
    return;
  }

  visited[cur] = 1;
  finish[cur].complNum = complNum;
  for(i = 1; i <= n; i++){
    if(graph[cur][i] > 0) {
      dfs(graph, n, i, visited, finish, complNum);
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
  Behavior: Updates the visited array, sets the complNum field of the curr node in the finish array to   complNum. Recursively calls itself on unvisited nodes until they are all complete
*/

int main() {
  // Init variables
  int i,j,k,n,m,a,b,w,cur;
  
  // Read through the .txt and represent through an adjacency matrix
  scanf("%d%d",&n,&m);
  int **graph = (int**)malloc(sizeof(int*)*(n+1));
  int *visited = (int*)calloc(n+1,sizeof(int));
  node *finish = (node*)calloc(n+1, sizeof(node));

  for(i = 0; i <= n; i++){
    graph[i] = (int*)calloc(n+1, sizeof(int));
  }
  // load in the v, e, and w
  for(i = 0; i < m; i++) {
    scanf("%d%d%d", &a, &b, &w);
    graph[a][b] = w;
  }

  // Order in topological order
  time = 0;
  for(i = 0; i <= n; i++) {
    finish[i].cur = i;
    finish[i].finish = 0;
  }
  j = 1;
  for(i = 1; i <= n; i++){
    if(visited[i] != 1){
      dfs(graph, n, i, visited, finish, j);
      j += 1;
    }
  }

  // Sort in descending order
  qsort((void*)finish, n+1, sizeof(node), compare);

  // Process in ascending order
  int **dp = (int**)malloc(sizeof(int*)*(n+1));
  for(i = 0; i <= n; i++){
    dp[i] = (int*)calloc(2,sizeof(int));
    // dp[i][0] represents the length of the longest path from 1 to i
    dp[i][0] = -1;
    // dp[i][1] represents the number of different paths whose length is dp[i][0]
    dp[i][1] = 0;
  }

  // Find the place of node 1 in the finish array
  for(j = 0; j <= n; j++) {
    if(finish[j].cur == 1) {
      break;
    }
  }

  // Relax
  dp[1][0] = 0;
  dp[1][1] = 1;
  for(i = j; i <= n; i++) {
    cur = finish[i].cur;
    if(dp[cur][0] != -1){
      for(k = 1; k <= n; k++){
        if(graph[cur][k] > 0){
          if(dp[k][0] < dp[cur][0] + graph[cur][k]) {
            dp[k][0] = dp[cur][0] + graph[cur][k];
          }
        }
      }
    }
  }

 // Distinct paths calculation
 for(i = j + 1; i <= n; i++) {
   cur = finish[i].cur;
   for(k = 1; k <= n; k++) {
     if(graph[k][cur] > 0) {
       if(dp[cur][0] == dp[k][0] + graph[k][cur]) {
         dp[cur][1] += dp[k][1];
       }
     }
   }
 }

 if(dp[n][0] > -1) {
   printf("longest path: %d\n", dp[n][0]);
   printf("number of longest paths: %d\n", dp[n][1]);
   // prints number of distinct paths, calculated in function above
 } else {
   printf("Node %d is not reachable from Node 1\n", n);
   // error statement in case some nonsense is encountered
 }
 return (0);
 // function returns 0 to indicate successful execution
}

