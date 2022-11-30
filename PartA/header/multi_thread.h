#include <pthread.h>
#include <stdlib.h>

// Helper Functions


//Struct to assign data to each thread
struct thread_data{
    int size;
    int start; //harsh
    int end;
    int *matA;
    int *matB;
    int *output;
};


// function to find transpose of matrix B
void transpose_matrixB(int *matB , int *temp , int N){
    for(int i=0 ; i<N ; i++){
        for(int j=0 ; j<N ; j++){
            temp[(i * N) + j] = matB[(j * N) + i];
        }
    }
}


//function for creating (N/2)*N matrix from matrix A & transpose of matrix B by adding two rows
void create_matrix(int *matA , int *matB , int *tempA , int *tempB , int n){
    int t=0;
    for(int i=0; i<n ; i=i+2){
        for(int j=0 ; j<n ; j++){
            tempA[(t * n) + j] = matA[(i * n) + j] + matA[(i+1) * n + j];
            tempB[(t * n) + j] = matB[(i * n) + j] + matB[(i+1) * n + j];
        }
        t++;
    }
}



//function to decide no. of threads
int cal_threads(int N , int max_threads){
    return (N/2 < max_threads) ? N/2 : max_threads;
}



//Function to assign work to each thread
void assign_data(int thread , int work , int *tempA , int *tempB , int *output , int N){

    struct thread_data data[thread]; //array of struct
    
    //assigning data to each thread using struct
    for(int i = 0 ; i < thread ; i++){
        data[i].size = N;
        data[i].start = i * work;
        data[i].end = data[i].start + work;
        data[i].matA = tempA;
        data[i].matB = tempB;
        data[i].output = output;
        }
}




//Matrix multiplication function used by each thread
void* mul(void* arg)
{
    struct thread_data *data = (struct thread_data *) arg ;

    int start = data->start;
    int end = data->end;
    int size = data->size;

    for(int i = start ; i < end; i++){
        for(int j = 0 ; j < size/2 ; j++){
            int sum = 0;
            for(int k = 0 ; k < size ; k++){
                sum += data->matA[i*size + k] * data->matB[j*size + k];
            } 
            data->output[i*(size/2) + j] = sum;  
        }
    }
    pthread_exit(NULL);
}




// Main multi-threading function

void multiThread(int N, int *matA, int *matB, int *output)
{

    //Transpose matrix B
   
    int *temp = (int *)malloc(N * N * sizeof(int));
    transpose_matrixB(matB , temp , N);
    

    //Convert matrix A & B to (n*2) * n matrix by adding the consecutive two rows

    int *tempA = (int *)malloc(N/2 * N * sizeof(int));
    int *tempB = (int *)malloc(N/2 * N * sizeof(int));
    create_matrix(matA , temp , tempA , tempB , N);


    // creating threads & assigning work to them
    
    int max_thread=128;  //maximum threads 

    int thread = cal_threads(N , max_thread);  //decide no. of threads according to matrix size
    
    int work = (N/2) / thread; //work load per thread

    pthread_t mul_threads[thread]; //declaring array of threads

    struct thread_data data[thread]; //array of struct
    
    assign_data(thread , work , tempA , tempB , output , N);

    //assigning data to each thread using struct
    for(int i = 0 ; i < thread ; i++){
        data[i].size = N;
        data[i].start = i * work;
        data[i].end = data[i].start + work;
        data[i].matA = tempA;
        data[i].matB = tempB;
        data[i].output = output;
        }


    //creating threads
    for(int k = 0 ; k<thread ; k++){
        pthread_create(&mul_threads[k] , NULL , mul , &data[k]);
    }

    //joining threads
    for(int k = 0 ; k<thread ; k++){
        pthread_join(mul_threads[k] , NULL);
    }

}