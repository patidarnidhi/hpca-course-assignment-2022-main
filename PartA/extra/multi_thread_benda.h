#include <pthread.h>
#include <stdlib.h>
// Create other necessary functions here

struct thread_data{
    int size;
    int start;
    int end;
    int *matA;
    int *matB;
    int *output;
};
//Each thread computes single element in the resultant matrix
void* mult(void* arg)
{
    struct thread_data *data = (struct thread_data *) arg ;

    int start = data->start;
    int end = data->end;
    int size = data->size;

    for(int i = start ; i < end; i++){
        for(int j = 0 ; j < size ; j++){
            int sum = 0;
            for(int k = 0 ; k < size ; k++){
                sum += data->matA[i*size + k] * data->matB[j*size + k];
            }
            data->output[i*(size/2) + j] = sum;
            //cerr<<" "<<sum;
        }
        //cerr<<endl;
    }

    // int temp=0;
    // int sum=0;
    // int j = data->startB;
    // for(int i = data->startA ; i < data->startA + data->size ; i++){
    //     sum += data->matA[i] * data->matB[j];
    //     j++;
    // }
    // data->output[data->out] = sum;
    // //cerr<<" "<<data->output[data->start];
    pthread_exit(NULL);
}

// Fill in this function
void multiThread(int N, int *matA, int *matB, int *output)
{
    //transpose matrix B
    // int *A=new int(2*2);
    //int *A=(int *)malloc(16*sizeof(int));
    //cout<<sizeof(*A)/sizeof(int)<<endl;
    // A={1,1,2,2};
    int *temp = (int *)malloc(N*N*sizeof(int));
    //cerr<<"Transpose B"<<endl;
    for(int i=0 ; i<N ; i++){
        for(int j=0 ; j<N ; j++){
            temp[i * N + j] = matB[j * N + i];
            //cerr<<" "<<temp[i * N + j];
        }
        //cerr<<endl;
    }

    //addition of consecutive two rows & columns of both matrix
    int *tempA = (int *)malloc(N/2*N*sizeof(int));
    int *tempB = (int *)malloc(N/2*N*sizeof(int));
    int c=0;
    for(int i=0; i<N ; i=i+2){
        for(int j=0 ; j<N ; j++){
            tempA[c * N + j] = matA[i * N + j] + matA[(i+1) * N + j];
            tempB[c * N + j] = temp[i * N + j] + temp[(i+1) * N + j];
        }
        c++;
    }

    // declaring array of threads
    int max_thread=2;
    //int thread = (N/2 < max_thread) ? N/2 : max_thread;
    int thread=2;
    int work = (N/2) / thread; //work load per thread

    pthread_t child_threads[thread];
    struct thread_data data[thread];

    for(int i = 0 ; i < thread ; i++){
        data[i].size = N;
        data[i].start = i * work;
        data[i].end = data[i].start + work;
        data[i].matA = tempA;
        data[i].matB = tempB;
        data[i].output = output;
    }

    for(int k = 0 ; k<thread ; k++){
        pthread_create(&child_threads[k] , NULL , mult , &data[k]);
    }

    for(int k = 0 ; k<thread ; k++){
        pthread_join(child_threads[k] , NULL);
    }

    // for (int i = 0; i < N/2; i++){
    //     for(int j = 0 ; j < N/2 ; j++){
    //         for(int k = 0 ; k < 2 ; k++){
    //             data[k].size = N ; 
    //             data[k].startA = i*N;
    //             data[k].startB = (j+k)*N;
    //             data[k].out = i*N + (j+k);
    //             data[k].matA = tempA;
    //             data[k].matB = tempB;
    //             data[k].output = output;
    //         }
                    
            // for(int k = 0 ; k<2 ; k++){
            //     pthread_create(&child_threads[k] , NULL , mult , &data[k]);
            // }

            // for(int k = 0 ; k<thread ; k++){
            //     pthread_join(child_threads[k] , NULL);
            // }
        //}
        
    //}
}
