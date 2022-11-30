// Create other necessary functions here
__global__ void matrixMul(int *matA, int *matB, int *output, int N , int threads) {
  // Compute each thread's global row and column index
  for(int rowA=threadIdx.x ; rowA<N ; rowA+=threads){
		for(int colB = 0; colB < N; colB += 2){
			int s = 0;

			for(int iter = 0; iter < N; iter++) 
			{
				s += matA[rowA * N + iter] * matB[iter * N + colB];	
			}
			output[rowA>>1 * (N>>1) +colB>>1]+=s;
        }
			
		for(int colB = 1; colB < N; colB += 2){
			int s = 0;

			for(int iter = 0; iter < N; iter++) 
			{
				s += matA[rowA * N + iter] * matB[iter * N + colB];
			}
		    output[rowA>>1 * (N>>1) +colB>>1]+=s;
		}
	}
}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
    size_t bytes = N * N * sizeof(int);
    size_t bytes1 = (N/2) * (N/2) * sizeof(int);

    int *d_a, *d_b, *d_c;
    cudaMalloc((void **)&d_a, bytes);
    cudaMalloc((void **)&d_b, bytes);
    cudaMalloc((void **)&d_c, bytes1);

    d_a ={0};
    d_b ={0};
    d_c ={0};

    cudaMemcpy(d_a , matA , bytes , cudaMemcpyHostToDevice);
    cudaMemcpy(d_b , matB , bytes , cudaMemcpyHostToDevice);

    int threads = N;

    int blocks = 1;

    matrixMul<<<blocks , threads>>>(d_a , d_b , d_c, N , threads);

    cudaMemcpy(output , d_c , bytes1 , cudaMemcpyDeviceToHost);

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}





// Create other necessary functions here
__global__ void matrixMul(int *a, int *b, int *c, int N , clock_t *time) {
  // Compute each thread's global row and column index

  if(threadIdx.x == 0){
    time[blockIdx.x] = clock();
  }

  int row = blockIdx.x * blockDim.x + threadIdx.x;
  int col = blockIdx.y * blockDim.y + threadIdx.y;

  // Iterate over row, and down column

  c[row * (N/2) + col] = 0;
  for (int k = 0; k < N; k++) {
    // Accumulate results for a single element
    c[row*(N/2) + col] += a[row *2*N + k] * b[k * N + col*2];
     c[row * (N/2) + col] += a[(row*2+1)*N + k] * b[k * N + col*2];
      c[row * (N/2) + col] += a[row *N*2 + k] * b[k * N + col*2+1];
       c[row * (N/2) + col] += a[(row*2+1) * N + k] * b[k * N + col*2+1];
  }

    if(threadIdx.x == 0){
        time[blockIdx.x + gridDim.x] = clock();
    }

//   for (int rowA = 0; rowA < N; rowA += 2)
//   {
//     for (int colB = 0; colB < N; colB += 2)
//     {
//       int sum = 0;
//       for (int iter = 0; iter < N; iter++)
//       {
//         sum += matA[rowA * N + iter] * matB[iter * N + colB];
//         sum += matA[(rowA + 1) * N + iter] * matB[iter * N + colB];
//         sum += matA[rowA * N + iter] * matB[iter * N + (colB + 1)];
//         sum += matA[(rowA + 1) * N + iter] * matB[iter * N + (colB + 1)];
//       }

//       // compute output indices
//       int rowC = rowA >> 1;
//       int colC = colB >> 1;
//       int indexC = rowC * (N >> 1) + colC;
//       output[indexC] = sum;
//     }
//   }

//   for(int rowA=threadIdx.x ; rowA<N ; rowA+=threads){
// 		for(int colB = 0; colB < N; colB += 2){
// 			int s = 0;

// 			for(int iter = 0; iter < N; iter++) 
// 			{
// 				s += matA[rowA * N + iter] * matB[iter * N + colB];	
// 			}
// 			output[(rowA>>1) * (N>>1) +(colB>>1)]+=s;
//         }
			
// 		for(int colB = 1; colB < N; colB += 2){
// 			int s = 0;

// 			for(int iter = 0; iter < N; iter++) 
// 			{
// 				s += matA[rowA * N + iter] * matB[iter * N + colB];
// 			}
// 		    output[(rowA>>1) * (N>>1) +(colB>>1)]+=s;
// 		}
// 	}
}

// Fill in this function
void gpuThread(int N, int *matA, int *matB, int *output)
{
    size_t bytes = N * N * sizeof(int);
    size_t bytes1 = (N/2) * (N/2) * sizeof(int);

    // //Transpose matrix B
    // int *temp = (int *)malloc(N*N*sizeof(int));
    // for(int i=0 ; i<N ; i++){
    //     for(int j=0 ; j<N ; j++){
    //         temp[i * N + j] = matB[j * N + i];
    //     }
    // }

    // //Adding consecutive rows of matrix A
    // int *tempA = (int *)malloc(N/2*N*sizeof(int));
    // //int *tempB = (int *)malloc(N/2*N*sizeof(int));
    // int c=0;
    // for(int i=0; i<N ; i=i+2){
    //     for(int j=0 ; j<N ; j++){
    //         tempA[c * N + j] = matA[i * N + j] + matA[(i+1) * N + j];
    //         //tempB[c * N + j] = temp[i * N + j] + temp[(i+1) * N + j];
    //     }
    //     c++;
    // }

    int *d_a, *d_b, *d_c;
    cudaMalloc((void **)&d_a, bytes);
    cudaMalloc((void **)&d_b, bytes);
    cudaMalloc((void **)&d_c, bytes1);

    // d_a ={0};
    // d_b ={0};
    // d_c ={0};

    //init_matrices(output,N);

    cudaMemcpy(d_a , matA , bytes , cudaMemcpyHostToDevice);
    cudaMemcpy(d_b , matB , bytes , cudaMemcpyHostToDevice);
    //cudaMemcpy(d_c , output , bytes , cudaMemcpyHostToDevice);

    int threads = 8;

    int blocks = (N/2) / threads;

    //Clock
    clock_t *time = new clock_t[blocks*2];
    clock_t *d_time;
    cudaMalloc(&d_time , sizeof(clock_t) * blocks * 2);

    dim3 thread(threads , threads);
    dim3 block(blocks , blocks);

    matrixMul<<<block , thread>>>(d_a , d_b , d_c, N , d_time)  ;

    cudaMemcpy(output , d_c , bytes1 , cudaMemcpyDeviceToHost);
    cudaMemcpy(time , d_time , sizeof(clock_t) * blocks * 2 , cudaMemcpyDeviceToHost);

    //Print time
    cout<<"Block , Clock"<<endl;
    for(int i=0 ; i< blocks ;i++){
        cout<<i<<" , "<<time[i+blocks] - time[i]<<endl;
    }

    cudaFree(d_a);
    cudaFree(d_b);
    cudaFree(d_c);
}

