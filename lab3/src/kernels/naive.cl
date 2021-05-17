__kernel void multiply_matrix(const int M, const int N, const int K,
                              __constant const float *A,
                              __constant const float *B,
                              __global float *C) {

    const int m = get_global_id(0); // Row (0..M)
    const int n = get_global_id(1); // Col (0..N)

    if (m >= M || n >= N) {
        return;
    }

    float acc = 0.0f;
    for (int k = 0; k < K; k++) {
        acc += A[K * m + k] * B[N * k + n];
    }

    C[N * m + n] = acc;
}