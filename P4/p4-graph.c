#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#define MAX_SIZE 1000
#define INF 9999

typedef int **matrix;

void printMatrix(matrix m,int n){
  for(int i =0;i<n;i++){
    for(int j=0 ;j<n;j++)
      printf("%d ",m[i][j]);
    printf("\n");
  }
}

matrix createMatrix(int n){
  int i;
  matrix aux;

  if ((aux = malloc(n * sizeof(int *))) == NULL)
    return NULL;
  for (i = 0; i < n; i++)
    if ((aux[i] = malloc(n * sizeof(int))) == NULL)
      return NULL;
  return aux;
}

/* Pseudorandom initialization [1..MAX_SIZE] of a complete undirected graph
with n nodes, represented by its adjacency matrix */
void initMatrix(matrix m, int n){
  int i, j;

  for (i = 0; i < n; i++)
    for (j = i + 1; j < n; j++)
      m[i][j] = rand() % MAX_SIZE + 1;
  for (i = 0; i < n; i++)
    for (j = 0; j <= i; j++)
      if (i == j)
        m[i][j] = 0;
      else
        m[i][j] = m[j][i];
}

void freeMatrix(matrix m, int n){
  int i;

  for (i = 0; i < n; i++)
    free(m[i]);
  free(m);
}

int minDistance(int *distances, int *unvisited,int sz,int min){
    int ind;
    for (int i = 0; i < sz; i++)
    {
        if(unvisited[i] && distances[i] < min)
        {
            min = distances[i];
            ind = i;
        }
    }
    return ind;
}

void dijkstra(matrix graph, matrix distances, int sz){
  int n, i,min, v = 0;
  int *unvisited = malloc(sz * sizeof(int));

  for (n = 0; n < sz; n++)
  {
    for (i = 0; i < sz; i++)
    {
      unvisited[i] = 1;
      distances[n][i] = graph[n][i];
    }
    for (int count = 0; count < sz-1; count++){
            min = INF;
            v = minDistance(distances[n],unvisited,sz,min);
            unvisited[v] = 0;
            for (int w = 0; w < sz ; w++)
                if (distances[n][w] > distances[n][v] + graph[v][w])
                    distances[n][w] = distances[n][v] + graph[v][w];
        }
  }
  free(unvisited);
}

double microseconds()
{ /* obtains the system time in microseconds */
  struct timeval t;

  if (gettimeofday(&t, NULL) < 0)
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

void showAlgorithm(){
  int m1[25]={0,1,8,4,7,1,0,2,6,5,8,2,0,9,5,4,6,9,0,3,7,5,5,3,0};
  matrix mygraph = createMatrix(5);
	for(int i=0; i<5; i++)
		for(int j=0; j<5; j++)
			mygraph[i][j] = m1[5*i + j];

  matrix d = createMatrix(5);
  dijkstra(mygraph,d,5);
  printf("Adjacency matrix:\n");
  printMatrix(mygraph,5);
  printf("\nMinimum distances:\n");
  printMatrix(d,5);
  printf("\n");

  freeMatrix(d,5);
}

void showAlgorithm2(){
  int m2[16]={0,1,4,7,1,0,2,8,4,2,0,3,7,8,3,0};
  matrix mygraph2 = createMatrix(4);
  for(int i=0; i<4; i++)
  	for(int j=0; j<4; j++)
  		mygraph2[i][j] = m2[4*i + j];

  matrix d2 = createMatrix(4);
  dijkstra(mygraph2,d2,4);
  printf("Adjacency matrix:\n");
  printMatrix(mygraph2,4);
  printf("\nMinimum distances:\n");
  printMatrix(d2,4);
  printf("\n");

  freeMatrix(d2,4);
}

double testTime(matrix m, int n)
{
  int i;
  int k = 1000; //nº times we repeat the t<500 loop
  double ta, tb, t1, t2, t = 0;
  matrix d;

  d=createMatrix(n);

  initMatrix(m, n);
  ta = microseconds();
  dijkstra(m, d, n);
  tb = microseconds();
  t = tb - ta;

  if (t < 500)
  {
    ta = microseconds();
    for (i = 0; i < k; i++)
    {
      initMatrix(m, n);
      dijkstra(m, d, n);
    }
    tb = microseconds();
    t1 = tb - ta;

    ta = microseconds();
    for (i = 0; i < k; i++)
    {
      initMatrix(m, n);
    }
    dijkstra(m, d, n);

    tb = microseconds();
    t2 = tb - ta;
    t = (t1 - t2) / k;
  }
  freeMatrix(d,n);
  return t;
}

void dijkstraTime()
{
  int n = 10;
  double time;
  matrix m;

  printf("       n\t    t(n)\tt(n)/n^1.7\t   ");
  printf("t(n)/n^2\t      t(n)/n^2.3\n");
  for (int i = 0; i < 7; i++)
  {
    m = createMatrix(n);
    time = testTime(m, n);
    freeMatrix(m,n);
    printf("   %5d     %11.3lf       %11.3lf        %11.3lf        %11.3lf\n",
           n, time, time / (pow(n,1.7)),
           time / (pow(n,2)), time / (pow(n, 2.3)));
    n *= 2;
  }
}

void init_seed()
{
  srand(time(NULL));
}

int main()
{
  init_seed();
  printf("Dijkstra's algorithm for a given matrix\n\n");
  showAlgorithm();
  printf("Dijkstra's algorithm for a given matrix\n\n");
  showAlgorithm2();

  for (int i = 1; i < 4; i++)
  {
    printf("\n\nDijkstra's algorithm iteration %d\n_\n", i);

    dijkstraTime();
  }

  return 0;
}
