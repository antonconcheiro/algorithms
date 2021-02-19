#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>
#include <unistd.h>
#define SZ 256000

typedef struct {
  int vector[SZ];
  int last;
} heap;

heap *initHeap(int n){
    heap *h = (heap * ) malloc(sizeof(heap)); //one is number of heap
    h->last = n;
    return h;
}


void percolateUp(heap *h,int index){
    int temp;
    int parent_node = (index-1)/2;

    if(h->vector[parent_node] > h->vector[index]){
        //swap and recursive call
        temp = h->vector[parent_node];
        h->vector[parent_node] = h->vector[index];
        h->vector[index] = temp;
        percolateUp(h,parent_node);
    }
}

void percolateDown(heap *h, int parent_node){
    int left = parent_node*2;
    int right = parent_node*2+1;
    int min;
    int temp;

    if(left >= h->last || left <0)
        left = -1;
    if(right >= h->last || right <0)
        right = -1;

    if(left != -1 && h->vector[left] < h->vector[parent_node])
        min=left;
    else
        min =parent_node;
    if(right != -1 && h->vector[right] < h->vector[min])
        min = right;


    if(min != parent_node){
        temp = h->vector[min];
        h->vector[min] = h->vector[parent_node];
        h->vector[parent_node] = temp;

        // recursive  call
        percolateDown(h, min);
    }
}

void create_heap(int v[], int n, heap *h){
  for (int j=0; j<n; j++){
    h->vector[j] = v[j];
  }
  h->last = n;
  for (int i=(h->last/2); i>=0; i--){
    percolateDown(h,i);
  }
}

int get_min(heap *h){
    if(h->last==0){
        printf("\nHeap is Empty\n");
        return -1;
    }
    return h->vector[0];
}

void remove_min(heap *h){
    if(h->last==0){
        printf("\nHeap is Empty\n");
        return;
    }
    // replace first node by last and delete last
    h->vector[0]=h->vector[h->last-1];
    h->last--;
    if(h->last>0)
      percolateDown(h,0);
}

void init_seed() {
    srand(time(NULL));
}

void random_init(int v[], int n) {
    int m=2*n+1;
    for (int i=0; i < n; i++){
        v[i] = (rand() % m) - n;
    }
}

void descending_init(int v[], int n) {
  int j = 0;
  for (int i=n; i != 0; i--){
    v[j] = i;
    j++;
  }
}

void ascending_init(int v[], int n) {
  for (int i=0; i < n ; i++)
    v[i] = i;
}

void heap_sort(int v[],int n){
  heap *h = initHeap(n);
  create_heap(v,n,h);
  for(int i=0;i<n;i++){
    v[n]=get_min(h);
    remove_min(h);
    //printf("%d\n",v[n]);
    //sleep(1);
  }
}

double microseconds() {   /* obtains the system time in microseconds */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

double testTime(int v[],int n, void (*init)(int v[],int n)) {
    int i;
    int k = 1000; //nº times we repeat the t<500 loop
    double ta,tb,t1,t2,t = 0;

    (*init)(v,n);
    ta = microseconds();
    heap_sort(v,n);
    tb = microseconds();
    t = tb - ta;

    if (t < 500){
        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
            heap_sort(v,n);
        }
        tb = microseconds();
        t1 = tb - ta;

        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
        }
        heap_sort(v,n);

        tb = microseconds();
        t2 = tb - ta;
        t = (t1 - t2) / k;
    }
    return t;
}

double testTimeCreate(int v[],int n, void (*init)(int v[],int n)) {
    int i;
    int k = 1000; //nº times we repeat the t<500 loop
    double ta,tb,t1,t2,t = 0;
    heap *h = initHeap(n);

    (*init)(v,n);
    ta = microseconds();
    create_heap(v,n,h);
    tb = microseconds();
    t = tb - ta;

    if (t < 500){
        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
            create_heap(v,n,h);
        }
        tb = microseconds();
        t1 = tb - ta;

        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
        }
        create_heap(v,n,h);

        tb = microseconds();
        t2 = tb - ta;
        t = (t1 - t2) / k;
    }
    return t;
}

void heapCreateAscending(){
  int n = 500;
  double time;

  printf("\ncreating heap with ascending initialization\n");
  printf("       n\t    t(n)\tt(n)/n^0.9\tt(n)/n        t(n)/n^1.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTimeCreate(v,n,ascending_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/pow(n,0.9),time/n,time/pow(n,1.1));
    n*=2;
  }
}

void heapCreateRandom(){
  int n = 500;
  double time;

  printf("\ncreate heap with random initializatioheap_sortn\n");
  printf("       n\t    t(n)\tt(n)/n^0.9\tt(n)/n        t(n)/n^1.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTimeCreate(v,n,random_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/pow(n,0.9),time/n,time/pow(n,1.1));
    n*=2;
  }
}

void heapCreateDescending(){
  int n = 500;
  double time;

  printf("\ncreate heap with descending initialization\n");
  printf("       n\t    t(n)\tt(n)/n^0.9\tt(n)/n        t(n)/n^1.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTimeCreate(v,n,descending_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/pow(n,0.9),time/n,time/pow(n,1.1));
    n*=2;
  }
}

void heapAscending(){
  int n = 500;
  double time;

  printf("\nheap sort with ascending initialization\n");
  printf("       n \t   t(n)  t(n)/n*log(n)^0.9  ");
  printf("t(n)/n*log(n)^1  t(n)/n*log(n)^1.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,ascending_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/(n*pow(log(n),0.9)),time/(n*pow(log(n),1)),
    time/(n*pow(log(n),1.1)));
    n*=2;
  }
}

void heapRandom(){
  int n = 500;
  double time;

  printf("\nheap sort with random initialization\n");
  printf("       n\t    t(n)\tt(n)/n^1\tt(n)/n^1.1        t(n)/n^1.3\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,random_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/pow(n,1),time/pow(n,1.1),time/pow(n,1.3));
    n*=2;
  }
}

void heapDescending(){
  int n = 500;
  double time;

  printf("\nheap sort with descending initialization\n");
  printf("       n\t    t(n)    t(n)/n*log(n)  ");
  printf("t(n)/n*log(n)^1.15  t(n)/n*log(n)^1.2\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,descending_init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/(n*log(n)),time/(n*pow(log(n),1.15)),
    time/(n*pow(log(n),1.2)));
    n*=2;
  }
}

int main(){
  init_seed();

  for(int i = 1; i<4;i++){
    printf("\n\nHeap creation iteration %d\n_\n\n",i);

    heapCreateAscending();
    heapCreateRandom();
    heapCreateDescending();
  }

  for(int i = 1; i<4;i++){
    printf("\n\nHeap sort iteration %d\n_\n\n",i);

    heapAscending();
    heapRandom();
    heapDescending();
  }

  return 0;
}
