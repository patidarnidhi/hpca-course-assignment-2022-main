//helper functions

typedef int vec __attribute__ (( vector_size(32) ));


// function to allocate n vectors and initializes them with zeros
vec* alloc(int n) {                
    vec *ptr = (vec *) aligned_alloc(32, 32 * n);   
    memset(ptr, 0, (32 * n));
    return ptr;
}


// function to find transpose of matrix B
void transpose_matB(int *matB , int *temp , int N){
    for(int i=0 ; i<N ; i++){
        for(int j=0 ; j<N ; j++){
            temp[(i * N) + j] = matB[(j * N) + i];
        }
    }
}


//function for creating (N/2)*N matrix from matrix A & transpose of matrix B by adding two rows
void create_mat(int *matA , int *matB , int *tempA , int *tempB , int n){
    int t=0;
    for(int i=0; i<n ; i=i+2){
        for(int j=0 ; j<n ; j++){
            tempA[(t * n) + j] = matA[(i * n) + j] + matA[(i+1) * n + j];
            tempB[(t * n) + j] = matB[(i * n) + j] + matB[(i+1) * n + j];
        }
        t++;
    }
}


//function for matrix multiplication
void multiply(vec *va , vec *vb , int *output , int n , int count){
    for (int i = 0; i < n; i=i+2) {
        for (int j = 0; j < n; j=j+2) {
            vec v; 

            for(int k = 0 ; k < n/2 ; k++){
              v[k]=0;
            }

            for (int k = 0; k < count; k++){
                //v += va[(i * count) + k] * vb[(j * count) + k];
                v += va[i * count + k]      *  vb[j * count + k];
                v += va[(i+1) * count + k]  *  vb[j * count + k];
                v += va[i * count + k]      *  vb[(j+1) * count + k];
                v += va[(i+1) * count + k]  *  vb[(j+1) * count + k]; 
            }
               
            for (int k = 0; k < 8; k++)
                //output[(i * n/2) + j] += v[k];
                output[(i>>1) * (n>>1) + (j>>1)] += v[k];

        }
    }
}



//main function performing single threading

void singleThread(int n, int *matA, int *matB, int *output)
{

    //Taking transpose of matrix B
    int *temp = (int *)malloc(n * n * sizeof(int));
    transpose_matB(matB , temp , n);
    


    //Convert matrix A & B to (n*2) * n matrix by adding the consecutive two rows
    int *tempA = (int *)malloc(n/2 * n * sizeof(int));
    int *tempB = (int *)malloc(n/2 * n * sizeof(int));
    create_mat(matA , temp , tempA , tempB , n);
    


    //Using vectorization for RMM

    int count = (n + 7) / 8; // it will give number of 8-element vectors in a row 

    // vec *va = alloc(n/2 * count);
    // vec *vb = alloc(n/2 * count);

    vec *va = alloc(n * count);
    vec *vb = alloc(n * count);


    //both matrices A & B are moved to the aligned region
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            va[(i * count) + j / 8][j % 8] = matA[(i * n) + j];
            vb[(i * count) + j / 8][j % 8] = matB[(j * n) + i]; 
        }
    }


    //matrix multiplication
    multiply(va , vb , output , n , count);
    

    std::free(va);
    std::free(vb);
  
}
