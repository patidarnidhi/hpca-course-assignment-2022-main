//helper functions

typedef int vec __attribute__ (( vector_size(32) ));

vec* alloc(int n) {         // allocates n vectors and initializes them with zeros
    vec *ptr = (vec *) aligned_alloc(32, 32 * n);
    memset(ptr, 0, 32 * n);
    return ptr;
}


//main function performing single threading using vectorisation

void singleThread(int n, int *matA, int *matB, int *output)
{   
    //divinding elements of a row into chunks of 8 elememnts

    int count = (n + 7) / 8; // it will give number of 8-element vectors in a row 


    //allocating memory to vectors a & b

    vec *va = alloc(n * count);
    vec *vb = alloc(n * count);


    //both matrices A & B are moved to the aligned region

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            va[i * count + j / 8][j % 8] = matA[i * n + j];
            vb[i * count + j / 8][j % 8] = matB[j * n + i]; 
        }
    }


    //matrix multiplication

    for (int i = 0; i < n; i=i+2) {
        for (int j = 0; j < n; j=j+2) {
            vec v; 

            for(int k = 0 ; k < 8 ; k++){
              v[k]=0;
            }
            
            for (int k = 0; k < count; k++){
                v += va[i * count + k]      *  vb[j * count + k];
                v += va[(i+1) * count + k]  *  vb[j * count + k];
                v += va[i * count + k]      *  vb[(j+1) * count + k];
                v += va[(i+1) * count + k]  *  vb[(j+1) * count + k]; 
            }
            
            for (int k = 0; k < 8; k++)
                output[(i>>1) * (n>>1) + (j>>1)] += v[k];
            
        }
    }

    std::free(va);
    std::free(vb);
  
}
