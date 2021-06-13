#ifndef TILE_SIZE
#define TILE_SIZE -1
#pragma message("TILE_SIZE need to be passed from the host")
#endif

#ifndef VECTOR_SIZE
#define VECTOR_SIZE -1
#pragma message("VECTOR_SIZE need to be passed from the host")
#endif

#define VECTORS_PER_TILE TILE_SIZE / VECTOR_SIZE

__kernel void multiply_matrix(const int M, const int N, const int K,
                              __global const float *A,
                              __global const float *B,
                              __global float *C) {

    const int localRow = get_local_id(1); // Local row (0..TILE_SIZE)
    const int localCol = get_local_id(0); // Local col (0..VECTORS_PER_TILE)

    const int tilesRow = get_group_id(1);
    const int tilesCol = get_group_id(0);

    const int globalRow = TILE_SIZE * tilesRow + localRow; // Row (0..M)
    const int globalCol = TILE_SIZE * tilesCol + localCol; // Col (0..N)

    __local float ATile[TILE_SIZE][TILE_SIZE];
    __local float BTile[TILE_SIZE][TILE_SIZE];

    float acc[VECTOR_SIZE] = {0.0f};

    int numTiles = K / TILE_SIZE;
    if (K % TILE_SIZE != 0) numTiles++;

    for (int t = 0; t < numTiles; t++) {

        // Load tile
        const int tileRow = t * TILE_SIZE + localRow;
        const int tileCol = t * TILE_SIZE + localCol;

        for (int i = 0; i < VECTOR_SIZE; i++) {
            const int AIndex = globalRow * K + tileCol + i * VECTORS_PER_TILE;
            if (AIndex < M * K) {
                ATile[localRow][localCol + i * VECTORS_PER_TILE] = A[AIndex];
            } else {
                ATile[localRow][localCol + i * VECTORS_PER_TILE] = 0;
            }

            const int BIndex = tileRow * N + globalCol + i * VECTORS_PER_TILE;
            if (BIndex < N * K) {
                BTile[localRow][localCol + i * VECTORS_PER_TILE] = B[BIndex];
            } else {
                BTile[localRow][localCol + i * VECTORS_PER_TILE] = 0;
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);

        // calculate tile
        for (int k = 0; k < TILE_SIZE; k++) {
            for (int i = 0; i < VECTOR_SIZE; i++) {
                acc[i] += ATile[localRow][k] * BTile[k][localCol + i * VECTORS_PER_TILE];
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }

    for (int i = 0; i < VECTOR_SIZE; i++) {
        const int row = globalRow;
        const int col = globalCol + i * VECTORS_PER_TILE;
        if (row >= M || col >= N) {
            continue;
        }
        C[row * N + col] = acc[i];
    }
}