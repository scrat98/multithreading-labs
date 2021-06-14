__kernel void prefix_sum_stage1(__global const float *data,
                                __global float *prefixSum,
                                const int dataSize, const int elementsPerBlock) {

    const int offset = elementsPerBlock * get_global_id(0);

    float sum = 0;
    int nextDataIndex;
    for (int i = 0; i < elementsPerBlock; i++) {
        nextDataIndex = i + offset;
        if (nextDataIndex >= dataSize) {
            return;
        }
        sum += data[nextDataIndex];
        prefixSum[nextDataIndex] = sum;
    }
}

__kernel void prefix_sum_stage3(__global float *prefixSum,
                                __global const float *sums,
                                const int dataSize, const int elementsPerBlock) {

    if (get_global_id(0) == 0) {
        return;
    }

    const int offset = elementsPerBlock * get_global_id(0);
    const float toAdd = sums[get_global_id(0) - 1];

    int nextDataIndex;
    for (int i = 0; i < elementsPerBlock; i++) {
        nextDataIndex = i + offset;
        if (nextDataIndex >= dataSize) {
            return;
        }
        prefixSum[nextDataIndex] += toAdd;
    }
}