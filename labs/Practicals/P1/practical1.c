#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <math.h>

double microseconds() {   /* obtains the system time in microseconds */
  struct timeval t;
  if (gettimeofday(&t, NULL) < 0 )
    return 0.0;
  return (t.tv_usec + t.tv_sec * 1000000.0);
}

int maxSubSum1(int v[], int n) {
    int i, j;
    int thisSum, maxSum = 0;
    for (i = 0; i < n; i++) {
        thisSum = 0;
        for (j = i; j < n; j++) {
            thisSum += v[j];
            if (thisSum > maxSum) {
                maxSum = thisSum;
            }
        }
    }
    return maxSum;
}

int maxSubSum2(int v[], int n) {
    int j;
    int thisSum = 0, maxSum = 0;

    for(j = 0; j < n; j++){
        thisSum += v[j];
        if (thisSum > maxSum){
            maxSum=thisSum;
        } else if (thisSum < 0){
            thisSum=0;
        }
    }
    return maxSum;
}

void init_seed() {
    srand(time(NULL));
    /* set the seed of a new sequence of pseudo-random integers */
}

void random_init(int v [], int n) {
    int i, m=2*n+1;
    for (i=0; i < n; i++){
      /* generate pseudo-random numbers between -n and +n */
        v[i] = (rand() % m) - n;
    }
}

double testTime1(int n, int k) {
    int i;
    double ta,tb,t1,t2,t = 0;
    int v[n];

    random_init(v,n);

    ta = microseconds();
    maxSubSum1(v, n);
    tb = microseconds();
    t = tb - ta;

    if (t < 500){
        ta = microseconds();
        for (i=0; i<k; i++){
            random_init(v,n);
            maxSubSum1(v, n);
        }
        tb = microseconds();
        t1 = tb - ta;

        ta = microseconds();
        for (i=0; i<k; i++){
            random_init(v,n);
        }
        tb = microseconds();
        t2 = tb - ta;
        t = (t1 - t2) / k;
    }
    return t;
}

double testTime2(int n, int k) {
    int i;
    double ta,tb,t1,t2,t = 0;
    int v[n];

    random_init(v,n);

    ta = microseconds();
    maxSubSum2(v, n);
    tb = microseconds();
    t = tb - ta;

    if (t < 500){
        ta = microseconds();
        for (i=0; i<k; i++){
            random_init(v,n);
            maxSubSum2(v, n);
        }
        tb = microseconds();
        t1 = tb - ta;

        ta = microseconds();
        for (i=0; i<k; i++){
            random_init(v,n);
        }
        tb = microseconds();
        t2 = tb - ta;
        t = (t1 - t2) / k;
    }
    return t;
}

void print_array(int v[], int n){
    printf("[");
    for(int i = 0; i < n; i++){
        if((v[i]<0 && v[i]>-10 )|| v[i]>9){
            printf("%d  ",v[i]);
        }else if (v[i]<-9){
        printf(" %d",v[i]);
        }else{
        printf(" %d  ",v[i]);
        }
    }
    printf("]");
}

void print_time(){
    printf("\n");

    printf("maxSubSum1\n");
    printf("n         t(n)        t(n)/n^1.8      t(n)/n^2       t(n)/n^2.2\n");
    double a;
    int n = 500;

    for(int i = 0; i<7;i++){
      a=testTime1(n,1000);
      printf("   %5d     %11.3lf         %8.7lf       %8.7lf         %8.7lf\n",
              n,a,a/pow(n,1.8),a/pow(n,2.),a/pow(n,2.2));
      n*=2;
    }

    printf("\n");

    printf("maxSubSum2\n");
    printf("n        t(n)      t(n)/n^0.85      t(n)/n^0.92      t(n)/n^1.1\n");
    double b;
    n = 500;

    for(int i = 0; i<7;i++){
      b=testTime2(n,1000);
      printf("   %5d     %11.3lf         %8.7lf       %8.7lf         %8.7lf\n",
              n,b,b/pow(n,0.85),b/pow(n,0.92),b/pow(n,1.1));
      n*=2;
    }
}

void test1() {
    int a,b;
    int v[][5]={{-9,2,-5,-4,6},{4,0,9,2,5},{-2,-11,-9,-7,-1},
                {9,-2,1,-7,-8},{15,-2,-5,-4,16},{7,-5,6,7,-7}};

    printf("\n");
    printf("test 1\n");
    printf("%33s%15s%15s\n", "", "maxSubSum1", "maxSubSum2");
    for(int i = 0;i<6;i++){
        print_array(v[i],5);
        a = maxSubSum1(v[i], 5);
        b = maxSubSum2(v[i], 5);
        printf("%27d%15d\n", a, b);
    }
    printf("\n");
}

void test2() {
    int i, a, b;
    int v[9];

    printf("\n");
    printf("test 2\n");
    printf("%33s%15s%15s\n", "", "maxSubSum1", "maxSubSum2");
    for (i=0; i<10; i++) {
        random_init(v,9);
        print_array(v, 9);
        a = maxSubSum1(v, 9);
        b = maxSubSum2(v, 9);
        printf("%15d%15d\n", a, b);
    }
}

int main() {
    init_seed();
    test1();
    test2();
    for (int i=0;i<3;i++)
    print_time();
    return 0;
}
