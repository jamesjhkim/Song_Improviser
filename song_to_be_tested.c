#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
volatile int *audio_ptr = (int *)AUDIO_BASE;

int main() {
    int i, j;
    int buffer_ptr_1 = 0;
    int buffer_ptr_2 = 0;

    // Note periods
    double frequencies[] = {261.63, 261.63, 293.66, 261.63, 349.23, 329.63,
                            261.63, 261.63, 293.66, 261.63, 392.00, 349.23,
                            261.63, 261.63, 523.25, 440.00, 349.23, 329.63,
                            293.66, 466.16, 466.16, 440.00, 349.23, 392.00, 349.23};
    int periods[25];
    int BUFFER_SIZE_1 = 0;
    int multiple_1 = 1000;
    int volume_1 = 1000;
    for(i = 0; i < 25; i++) {
        periods[i] = 8000 / frequencies[i];
        BUFFER_SIZE_1 += periods[i] * multiple_1;
    }

    short int buffer_1[BUFFER_SIZE_1];
    int idx = 0; // This index is used to fill the buffer

    // Generate waveform for all notes
    for (i = 0; i < 25; i++) {
        for (j = 0; j < periods[i] * multiple_1; j++) {
            double sample = volume_1 * cos(2 * M_PI * j / periods[i]);
            buffer_1[idx++] = (short)(sample);
        }
    }

    // Sound 2 initialization (omitted for brevity, similar approach as Sound 1)

    while (1) {
        int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
        int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

        // Left speaker output (chord 1)
        if (out_fifo_1 > 0) {
            for (int j = 0; j < 128 && buffer_ptr_1 < BUFFER_SIZE_1; j++) {
                *(audio_ptr + 2) = ((int)buffer_1[buffer_ptr_1++]) << 16;
                if (buffer_ptr_1 >= BUFFER_SIZE_1) buffer_ptr_1 = 0; // Loop back if at the end
            }
        }

        // Right speaker output (similar approach for chord 2)
        // Omitted for brevity, follow the pattern from the left speaker output
    }

    return 0;
}