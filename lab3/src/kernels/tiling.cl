#ifndef TILE_SIZE
#define TILE_SIZE -1
#pragma message("TILE_SIZE need to be passed from the host")
#endif

__kernel void multiply_matrix(const int M, const int N, const int K,
                              __global const float *A,
                              __global const float *B,
                              __global float *C) {

    const int localRow = get_local_id(1); // Local row (0..TILE_SIZE)
    const int localCol = get_local_id(0); // Local col (0..TILE_SIZE)

    const int tilesRow = get_group_id(1);
    const int tilesCol = get_group_id(0);

    const int globalRow = TILE_SIZE * tilesRow + localRow; // Row (0..M)
    const int globalCol = TILE_SIZE * tilesCol + localCol; // Col (0..N)

    __local float ATile[TILE_SIZE][TILE_SIZE];
    __local float BTile[TILE_SIZE][TILE_SIZE];

    float acc = 0.0f;

    int numTiles = K / TILE_SIZE;
    if (K % TILE_SIZE != 0) numTiles++;

    for (int t = 0; t < numTiles; t++) {

        // Load tile
        const int tileRow = t * TILE_SIZE + localRow;
        const int tileCol = t * TILE_SIZE + localCol;

        const int AIndex = globalRow * K + tileCol;
        if (AIndex < M * K) {
            ATile[localRow][localCol] = A[AIndex];
        } else {
            ATile[localRow][localCol] = 0;
        }

        const int BIndex = tileRow * N + globalCol;
        if (BIndex < N * K) {
            BTile[localRow][localCol] = B[BIndex];
        } else {
            BTile[localRow][localCol] = 0;
        }

        barrier(CLK_LOCAL_MEM_FENCE);

        // Calculate tile
        for (int k = 0; k < TILE_SIZE; k++) {
            acc += ATile[localRow][k] * BTile[k][localCol];
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    if (globalRow >= M || globalCol >= N) {
        return;
    }
    C[N * globalRow + globalCol] = acc;
}