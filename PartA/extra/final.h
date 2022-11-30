//helper functions

typedef int vec __attribute__ (( vector_size(32) ));

vec* alloc(int n) {                // allocates n vectors and initializes them with zeros
    vec *ptr = (vec *) aligned_alloc(32, 32 * n);   
    memset(ptr, 0, (32 * n));
    return ptr;
}


//main function performing single threading

void singleThread(int n, int *matA, int *matB, int *output)
{

    //Taking transpose of matrix B

    int *temp = (int *)malloc(n * n * sizeof(int));

    for(int i=0 ; i<n ; i++){
        for(int j=0 ; j<n ; j++){
            temp[(i * n) + j] = matB[(j * n) + i];
        }
    }



    //Convert matrix A & B to (n*2) * n matrix by adding the consecutive two rows

    int *tempA = (int *)malloc(n/2 * n * sizeof(int));
    int *tempB = (int *)malloc(n/2 * n * sizeof(int));
    
    int t=0;
    for(int i=0; i<N ; i=i+2){
        for(int j=0 ; j<N ; j++){
            tempA[(t * N) + j] = matA[(i * N) + j] + matA[(i+1) * N + j];
            tempB[(t * N) + j] = temp[(i * N) + j] + temp[(i+1) * N + j];
        }
        t++;
    }



    //Using vectorization for RMM

    int count = (n + 7) / 8; // it will give number of 8-element vectors in a row 

    vec *va = alloc(n/2 * count);
    vec *vb = alloc(n/2 * count);


    //both matrices A & B are moved to the aligned region
    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n; j++) {
            va[(i * count) + j / 8][j % 8] = matA[(i * n) + j];
            vb[(i * count) + j / 8][j % 8] = matB[(i * n) + j]; 
        }
    }


    //matrix multiplication
    for (int i = 0; i < n/2; i++) {
        for (int j = 0; j < n/2; j++) {
            vec v; 

            for(int k = 0 ; k < n/2 ; k++){
              v[k]=0;
            }

            for (int k = 0; k < count; k++){
                v += va[(i * count) + k] * vb[(j * count) + k];
            }
               
            for (int k = 0; k < 8; k++)
                output[(i * n/2) + j] += v[k];

        }
    }

    std::free(va);
    std::free(vb);
  
}
