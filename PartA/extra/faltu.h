#include <pthread.h>
#include <stdlib.h>
// Create other necessary functions here

//Each thread computes single element in the resultant matrix
void* mult(void* arg)
{
    int *arr = (int *)arg;
    int k = 0, i = 0, sum=0;

    int x = arr[0];
    for (i = 1; i <= x; i++)
           k += arr[i]*arr[i+x];
     
    int p = (int)malloc(sizeof(int));
    *p = k;
    return (void *)p;
    //pthread_exit(p);
}

// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    int max = (N*N)/4;
     
    //declaring array of threads of size (N*N)/4       
    pthread_t th1 , th2 , th3 , th4;
    // th1 = (pthread_t*)malloc(N*sizeof(pthread_t));
    // th2 = (pthread_t*)malloc(N*sizeof(pthread_t));
    // th3 = (pthread_t*)malloc(N*sizeof(pthread_t));
    // th4 = (pthread_t*)malloc(N*sizeof(pthread_t));

    int count = 0;
    int* data1 = NULL;
    int* data2 = NULL;
    int* data3 = NULL;
    int* data4 = NULL;

    for (int i = 0; i < N; i+=2){

        for (int j = 0; j < N; j+=2)
            {
            data1 = (int *)malloc((2*N + 1)*sizeof(int));
            data2 = (int *)malloc((2*N + 1)*sizeof(int));
            data3 = (int *)malloc((2*N + 1)*sizeof(int));
            data4 = (int *)malloc((2*N + 1)*sizeof(int));
            data1[0] = N;
            data2[0] = N;
            data3[0] = N;
            data4[0] = N;

            //Thread1 r1*c1
            for (int k = 0; k < N; k++)
                data1[k+1] = matA[i * N + k];
            for (int k = 0; k < N; k++)
                data1[k+1+N] = matB[k * N + j];

            pthread_create(&th1, NULL, &mult, (void*)(data1));

            //Thread2 r1*c2
            for (int k = 0; k < N; k++)
                data2[k+1] = matA[i * N + k];
            for (int k = 0; k < N; k++)
                data2[k+N+1] = matB[k * N + (j+1)];
            
            pthread_create(&th2, NULL, mult, (void*)(data2));

            //Thread3 r2*c1
            for (int k = 0; k < N; k++)
                data3[k+1] = matA[(i+1) * N + k];
            for (int k = 0; k < N; k++)
                data3[k+N+1] = matB[k * N + j];
            
            pthread_create(&th3, NULL, mult, (void*)(data3));

            //Thread4 r2*c2
            for (int k = 0; k < N; k++)
                data4[k+1] = matA[(i+1) * N + k];
            for (int k = 0; k < N; k++)
                data4[k+N+1] = matB[k * N + (j+1)];
            
            pthread_create(&th4, NULL, mult, (void*)(data4)); 


    int *sum1, *sum2 , *sum3 , *sum4;
       
      //Joining all threads and collecting return value
      //cout<<" "<<threads[i];
    pthread_join(th1, (void **)&sum1);
    pthread_join(th2, (void **)&sum2);
    pthread_join(th3, (void **)&sum3);
    pthread_join(th4, (void **)&sum4);

    // int *p1 = (int *)sum1;
    // int *p2 = (int *)sum2;
    // int *p3 = (int *)sum3;
    // int *p4 = (int *)sum4;

    int row = i>>1;
    int col = j>>1;
    int index = row * (N>>1) + col;
    output[index] = (*sum1)+(*sum2)+(*sum3)+(*sum4);
    }      
    }
}