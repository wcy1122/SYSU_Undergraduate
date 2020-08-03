/*
 * In general, you don't need to modify this file to finish hw1.
 */
#ifndef _INCL_SAMPLES
#define _INCL_SAMPLES

#include <stdio.h>
#include <stdlib.h>

// getNextSample reads in the next sample as a float array.
int getNextSample(FILE *stream, int *width, int *height, float **sample) {
    if (fread(width, 4, 1, stream) == 0) {
        // eof
        return 0;
    }
    fread(height, 4, 1, stream);
    int size = (*width) * (*height);

    *sample = (float*)malloc(sizeof(float)*size);
    if (*sample == NULL) {
        printf("failed to allocate\n");
        return 0;
    }

    char elem;
    for (int i = 0; i < size; i++) {
        fread(&elem, 1, 1, stream);
        (*sample)[i] = elem;
    }

    return 1;
}

#endif