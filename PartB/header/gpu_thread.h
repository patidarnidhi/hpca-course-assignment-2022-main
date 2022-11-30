// Helper functions




//Calculate no. of blocks
int num_threads(int N , int threads){
    return (N/2) / threads ;
}




__global__ void reduced_matrix_multiplication(int *matA, int *matB, int *output_matrix, int N, clock_t *time) {
  // Compute each thread's global row and column index


  //initialising value of row & column
  int row_iter = blockIdx.x * blockDim.x + threadIdx.x;
  int col_iter = blockIdx.y * blockDim.y + threadIdx.y;


  output_matrix[row_iter * (N/2) + col_iter] = 0; //initialising entry of output matrix to 0

  //performing matrix multiplication to get a single element of resultant matrix
  for (int k = 0; k < N/2; k++) {
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * (N<<1)) + k]  *  matB[(k * N) + (col_iter * 2)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * 2 + 1) * N + k]  *  matB[(k * N) + (col_iter * 2)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * (N<<1)) + k]  *  matB[(k * N) + (col_iter * 2 + 1)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * 2 + 1) * N + k]  *  matB[(k * N) + (col_iter * 2 + 1)];
  }
    int k=N/2;
  for (int k = N/2; k < N; k++) {
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * (N<<1)) + k]  *  matB[(k * N) + (col_iter * 2)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * 2 + 1) * N + k]  *  matB[(k * N) + (col_iter * 2)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * (N<<1)) + k]  *  matB[(k * N) + (col_iter * 2 + 1)];
      output_matrix[(row_iter * (N>>1)) + col_iter] += matA[(row_iter * 2 + 1) * N + k]  *  matB[(k * N) + (col_iter * 2 + 1)];
  }

}






//Transpose of matrix B
void transpose_matB(int *matB , int *temp , int N){
    for(int i=0 ; i<N ; i++){
        for(int j=0 ; j<N/2 ; j++){
            temp[(i * N) + j] = matB[(j * N) + i];
        }
        for(int j=N/2 ; j<N ; j++){
            temp[(i * N) + j] = matB[(j * N) + i];
        }
    }
}


//Clock function
void print_time(clock_t *check_time , int num_of_blocks){
    // cout<<"Blocks , Time"<<endl;
    // for(int i=0 ; i<num_of_blocks; i++){
    //     cout<<i<<","<<(check_time[i+num_of_blocks] - check_time[i])<<endl;
    // }
}



// Main GPU function


void gpuThread(int N, int *matA, int *matB, int *output)
{

    //allocating memory to 3 temp matrices
    int *temp_matA, *temp_matB, *temp_output;

    size_t size_of_matrix = N * N * sizeof(int); 

    cudaMalloc((void **)&temp_matA, size_of_matrix);
    cudaMalloc((void **)&temp_matB, size_of_matrix);
    cudaMalloc((void **)&temp_output, size_of_matrix/4);


    int max_num_of_threads = 16;

    //copy value of input matrices to these temp matrices

    cudaMemcpy(temp_matA , matA , size_of_matrix , cudaMemcpyHostToDevice);
    cudaMemcpy(temp_matB , matB , size_of_matrix , cudaMemcpyHostToDevice);

    int *temp;
    cudaMalloc((void **)&temp, size_of_matrix);
    cudaMemset(temp,0,size_of_matrix);
    //transpose_matB(matB , temp , N);


    int thread_count = 8;

    int num_of_blocks = num_threads(N,thread_count); //call function to get no. of blocks
    
    //Clock
    clock_t *check_time = new clock_t[num_of_blocks * 2];
    clock_t *temp_time;
    cudaMalloc(&temp_time , sizeof(clock_t) * num_of_blocks * 2);

    dim3 thread_dim(thread_count , thread_count);
    dim3 block_dim(num_of_blocks , num_of_blocks);

    reduced_matrix_multiplication <<<thread_dim , block_dim>>>(temp_matA , temp_matB , temp_output , N ,temp_time) ;  //call matrix multiplication function

    cudaMemcpy(output , temp_output , size_of_matrix/4 , cudaMemcpyDeviceToHost); //copy result to output matrix

    cudaMemcpy(check_time , temp_time , sizeof(clock_t) * num_of_blocks * 2 , cudaMemcpyDeviceToHost);

    print_time(check_time , num_of_blocks);
    // cout<<"Blocks , Time"<<endl;
    // for(int i=0 ; i<num_of_blocks; i++){
    //     cout<<i<<","<<(check_time[i+num_of_blocks] - check_time[i])<<endl;
    // }

    cudaFree(temp_matA);
    cudaFree(temp_matB);
    cudaFree(temp_output);
}
