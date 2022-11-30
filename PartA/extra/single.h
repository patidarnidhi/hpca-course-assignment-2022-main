typedef float vec __attribute__ (( vector_size(32) ));

// a helper function that allocates n vectors and initializes them with zeros
vec* alloc(int n) {
    vec* ptr = (vec*) aligned_alloc(32, 32 * n);
    memset(ptr, 0, 32 * n);
    return ptr;
}
void singleThread(int n, int *matA, int *matB, int *output)
{
  int nB = (n + 7) / 8; // number of 8-element vectors in a row (rounded up)

    vec *a = alloc(n * nB);
    vec *b = alloc(n * nB);

    // move both matrices to the aligned region
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            a[i * nB + j / 8][j % 8] = matA[i * n + j];
            b[i * nB + j / 8][j % 8] = matB[j * n + i]; // <- b is still transposed
        }
    }

    for (int i = 0; i < n; i=i+2) {
        for (int j = 0; j < n; j=j+2) {
            vec s; // initialize the accumulator with zeros
            for(int ll=0;ll<8;ll++){
              s[ll]=0;
            }
            // vertical summation
            for (int k = 0; k < nB; k++){
                s += a[i * nB + k] * b[j * nB + k];
                s += a[i * nB + k] * b[(j+1) * nB + k];
                s += a[(i+1) * nB + k] * b[j * nB + k];
                s += a[(i+1) * nB + k] * b[(j+1) * nB + k]; 
            }
               
            
            // horizontal summation
            
            for (int k = 0; k < 8; k++)
                output[(i>>1) * (n>>1) + (j>>1)] += s[k];
            
            // int rowC = i>>1;
            // int colC = j>>1;
            // int indexC = rowC * (n>>1) + colC;
            // output[indexC] = sum;
        }
    }

    std::free(a);
    std::free(b);
}
// int c=0;
    // for(int i=0; i<n ; i=i+2){
    //     for(int j=0 ; j<n ; j++){
    //         tempB[j * n + c] = matB[j * n + i] + matB[(j * n) + 1 + i];
    //     }
    //     c++;
    // }