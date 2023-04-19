#include <stdio.h>

#define nV 4
// defines # of vertices

#define INF 999

void printMatrix(int matrix[][nV]);
// forward decl

void floydWarshall(int graph[][nV]) {
  int matrix[nV][nV], i, j, k;

  for(int i = 0; i < nV; i++) {
    for(int j = 0; j < nV; j++){
      matrix[i][j] = graph[i][j];
    }
    // store as matrix
  }
  for(i = 0; i < nV; i++) {
    for(j = 0; j < nV; j++) {
      for(k = 0; k < nV; k++) {
        if(matrix[i][k] + matrix[k][j] < matrix[i][j]) {
          matrix[i][j] = matrix[i][k] + matrix[k][j];
	// exponentiation given if condition
        }
      }
    }
  }
  printMatrix(matrix);
}

void printMatrix(int matrix[][nV]) {
// self explanatory
  for(int i = 0; i < nV; i++) {
    for(int j = 0; j < nV; j++) {
      if(matrix[i][j] == INF) {
        printf("%4s", "INF");
	} else {
	  printf("%4d", matrix[i][j]);
	}
      }
	printf("\n");
    }  
}

int main() {
  int graph [nV][nV] = {{0, 1, INF, 1}, {INF, 0, 1, INF}, {1, INF, 0, INF}, {INF, INF, 1, 0}};
  // may want to change to read txts
  printf("All Pairs Shortest Path Matrix is:\n");  
  floydWarshall(graph);
}
