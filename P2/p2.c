#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

void ins_sort(int v[], int n) {
  int i, j, x;
    for (i = 1; i < n; i++) {
      x = v[i];
      j = i-1;
      while (j >= 0 && v[j] > x) {
        v[j+1] = v[j];
        j--;
      }
      v[j+1] = x;
    }
}

void shell_sort2(int v[], int n) {
  int tmp, i, j;
  int increment = n;
  int keepgoing = 0; //false
  do {
    increment = increment/2;
    for (i=increment; i<n; i++){
      tmp = v[i];
      j = i;
      keepgoing = 1; //true
      while (((j-increment)>=0) && (keepgoing == 1)){
        if (tmp < v[j-increment]){
          v[j] = v[j-increment];
          j = j-increment;
        }
        else keepgoing = 0; //false
      }
      v[j] = tmp;
    }
  } while(increment != 1);
}

void shell_sort(int arr[], int num){
    int i, j, k, tmp;
    for (i = num / 2; i > 0; i = i / 2){
        for (j = i; j < num; j++){
            for(k = j - i; k >= 0; k = k - i){
                if (arr[k+i] >= arr[k])
                    break;
                else{
                    tmp = arr[k];
                    arr[k] = arr[k+i];
                    arr[k+i] = tmp;
                }
            }
        }
    }
}

void printArray(int v[], int n){
  for(int i = 0; i < n; i++){
      if((v[i]<0 && v[i]>-10 )|| v[i]>9){
          printf(" %d  ",v[i]);
      }else if (v[i]<-9){
      printf(" %d ",v[i]);
      }else{
      printf("  %d  ",v[i]);
    }
  }
  printf("\n");
}

void init_seed() {
    srand(time(NULL));
}

void random_init(int v[], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++){
        v[i] = (rand() % m) - n;
    }
}

void descending_init(int v[], int n) {
  int i,j;
  j=0;
  for (i=n; i != 0; i--){
    v[j] = i;
    j++;
  }
}

void ascending_init(int v[], int n) {
  int i;
  for (i=0; i < n ; i++)
    v[i] = i;
}

double microseconds() {   /* obtains the system time in microseconds */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

double testTime(int v[],int n, void (*sort)(int v[],int n),
void (*init)(int v[],int n)) {
    int i;
    int k = 1000; //n1 times we repeat the t<500 loop
    double ta,tb,t1,t2,t = 0;

    (*init)(v,n);
    ta = microseconds();
    (*sort)(v,n);
    tb = microseconds();
    t = tb - ta;

    if (t < 500){
        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
            (*sort)(v,n);
        }
        tb = microseconds();
        t1 = tb - ta;

        ta = microseconds();
        for (i=0; i<k; i++){
            (*init)(v,n);
        }
        (*sort)(v,n);

        tb = microseconds();
        t2 = tb - ta;
        t = (t1 - t2) / k;
    }
    return t;
}

int sorted(int v[], int n){
  int tmp = 1;

  for (int i = 0; i<n-1; i++){
    if (v[i] > v[i+1]){
      tmp = 0;}
  }
  return tmp;
}


void printing(void (*sort)(int v[],int n), void (*init)(int v[],int n)){
  int v[15];

  (*init)(v,15);
  printArray(v,15);
  if (sorted(v, 15) == 0){
    printf("sorted? 0\n");
    if(*sort == shell_sort)
      printf("Shell sort\n");
    else
      printf("Insertion sort\n");
    (*sort)(v,15);
    printArray(v,15);
    printf("sorted? 1\n\n");
  } else
    printf("sorted? 1\n\n");
}

void insertionAscending(void(*sort)(int v[],int n),void(*init)(int v[],int n)){
  int n = 500;
  double time;

  printf("Insertion sort with ascending initialization\n");
  printf("       n    \t   t(n) \tt(n)/n^0.9  \t t(n)/n \tt(n)/n^1.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,sort,init);
    printf("   %5d     %11.3lf \t%8.7lf       %8.7lf \t%8.7lf\n",n,time,
    time/pow(n,0.9),time/n,time/pow(n,1.1));
    n*=2;
  }
}

void insertionRandom(){
  int n = 500;
  double time;

  //if(*init == random_init){
    printf("Insertion sort with random initialization\n");
    printf("       n\t    t(n)\tt(n)/n^1.9\tt(n)/n^2\tt(n)/n^2.1\n");
    for(int i = 0; i<7;i++){
      int v[n];
      time = testTime(v,n,ins_sort,random_init);
      printf("   %5d     %11.3lf \t%8.7lf       %8.7lf \t%8.7lf\n",n,time,
      time/pow(n,1.9),time/pow(n,2.),time/pow(n,2.1));
      n*=2;
    }
  //}
}

void insertionDescending(void(*sort)(int v[],int n),void(*init)(int v[],int n)){
  int n = 500;
  double time;

  printf("Insertion sort with descending initialization\n");
  printf("       n\t  t(n)\t\tt(n)/n^1.9 \t t(n)/n^2 \tt(n)/n^2.1\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,sort,init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf \t%8.7lf\n",n,
    time,time/pow(n,1.9),time/pow(n,2.),time/pow(n,2.1));
    n*=2;
  }
}

void shellAscending(void (*sort)(int v[],int n), void (*init)(int v[],int n)){
  int n = 500;
  double time;

  printf("\nShell sort with ascending initialization\n");
  printf("       n \t   t(n)  t(n)/n*log(n)^1.1  ");
  printf("t(n)/n*log(n)^1.2  t(n)/n*log(n)^1.3\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,sort,init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/(n*pow(log(n),1.1)),time/(n*pow(log(n),1.2)),
    time/(n*pow(log(n),1.3)));
    n*=2;
  }
}

void shellRandom(void (*sort)(int v[],int n), void (*init)(int v[],int n)){
  int n = 500;
  double time;

  printf("\nShell sort with random initialization\n");
  printf("       n\t    t(n)\tt(n)/n^1.1\tt(n)/n^1.2        t(n)/n^1.3\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,sort,init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/pow(n,1.1),time/pow(n,1.2),time/pow(n,1.3));
    n*=2;
  }
}

void shellDescending(void (*sort)(int v[],int n), void (*init)(int v[],int n)){
  int n = 500;
  double time;

  printf("\nShell sort with descending initialization\n");
  printf("       n\t    t(n)    t(n)/n*log(n)  ");
  printf("t(n)/n*log(n)^1.15  t(n)/n*log(n)^1.2\n");
  for(int i = 0; i<7;i++){
    int v[n];
    time = testTime(v,n,sort,init);
    printf("   %5d     %11.3lf         %8.7lf       %8.7lf\t %8.7lf\n",n,
    time,time/(n*log(n)),time/(n*pow(log(n),1.15)),
    time/(n*pow(log(n),1.2)));
    n*=2;
  }
}

int main(){
  init_seed();

  printf("Ascending initialization\n");
  printing(ins_sort,ascending_init);
  printf("Random initialization\n");
  printing(ins_sort,random_init);
  printf("Descending initialization\n");
  printing(ins_sort,descending_init);

  printf("Ascending initialization\n");
  printing(shell_sort,ascending_init);
  printf("Random initialization\n");
  printing(shell_sort,random_init);
  printf("Descending initialization\n");
  printing(shell_sort,descending_init);

  for(int i = 1; i<4;i++){
    printf("\n");printf("\n");printf("\n");
    printf("Iteration %d\n",i);printf("___");printf("\n");
    //sort=ins_sort,shell_sort
    //init=random_init,descending_init,ascending_init
    insertionAscending(ins_sort,ascending_init);
    insertionRandom();
    insertionDescending(ins_sort,descending_init);

    shellAscending(shell_sort,ascending_init);
    shellRandom(shell_sort,random_init);
    shellDescending(shell_sort,descending_init);
  }

  return 0;
}
