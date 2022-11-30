// Helper functions

__global__ void matrixMul(int *matA, int *matB, int *output, int N , clock_t *time) {
  // Compute each thread's global row and column index

  if(threadIdx.x == 0){
    time[blockIdx.x] = clock();
  }

  //initialising value of row & column
  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;


  output[row * (N/2) + col] = 0; //initialising entry of output matrix to 0

  //performing matrix multiplication to get a single element of resultant matrix
  for (int k = 0; k < N; k++) {
      output[(row * N/2) + col] += matA[(row * 2 * N) + k]  *  matB[(k * N) + (col * 2)];
      output[(row * N/2) + col] += matA[(row * 2 + 1) * N + k]  *  matB[(k * N) + (col * 2)];
      output[(row * N/2) + col] += matA[(row * N * 2) + k]  *  matB[(k * N) + (col * 2 + 1)];
      output[(row * N/2) + col] += matA[(row * 2 + 1) * N + k]  *  matB[(k * N) + (col * 2 + 1)];
  }

    if(threadIdx.x == 0){
        time[blockIdx.x + gridDim.x] = clock();
    }
}




// Main GPU function


void gpuThread(int N, int *matA, int *matB, int *output)
{

    //allocating memory to 3 temp matrices
    int *tempA, *tempB, *tempC;

    size_t size = N * N * sizeof(int); 

    cudaMalloc((void **)&tempA, size);
    cudaMalloc((void **)&tempB, size);
    cudaMalloc((void **)&tempC, size/4);


    //copy value of input matrices to these temp matrices

    cudaMemcpy(tempA , matA , size , cudaMemcpyHostToDevice);
    cudaMemcpy(tempB , matB , size , cudaMemcpyHostToDevice);

    

    int threads = 8;

    int blocks = (N/2) / threads;

    //Clock
    clock_t *time = new clock_t[blocks*2];
    clock_t *d_time;
    cudaMalloc(&d_time , sizeof(clock_t) * blocks * 2);

    dim3 thread(threads , threads);
    dim3 block(blocks , blocks);

    matrixMul<<<block , thread>>>(tempA , tempB , tempC , N , d_time) ;  //call matrix multiplication function

    cudaMemcpy(output , tempC , size/4 , cudaMemcpyDeviceToHost); //copy result to output matrix

    cudaMemcpy(time , d_time , sizeof(clock_t) * blocks * 2 , cudaMemcpyDeviceToHost);

    //Print time
    cout<<"Block , Clock"<<endl;
    for(int i=0 ; i< blocks ;i++){
        cout<<i<<" , "<<time[i+blocks] - time[i]<<endl;
    }

    cudaFree(tempA);
    cudaFree(tempB);
    cudaFree(tempC);
}
