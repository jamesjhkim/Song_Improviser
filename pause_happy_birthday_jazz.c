#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
volatile int *audio_ptr = (int *)AUDIO_BASE;

int main() {
    int i = 0;
    int buffer_ptr_1 = 0;
    int buffer_ptr_2 = 0;

    double notesFrequencies[] = {
        261.63, 261.63, 293.66, 261.63, 349.23, 329.63,
        261.63, 261.63, 293.66, 261.63, 392.00, 349.23,
        261.63, 261.63, 523.25, 440.00, 349.23, 329.63, 293.66,
        466.16, 466.16, 440.00, 349.23, 392.00, 349.23
    };
    double bassLineFrequencies[] = {
        130.81, 130.81, 130.81, 130.81, 130.81, 130.81,
        196.00, 196.00, 196.00, 196.00, 196.00, 196.00,
        130.81, 130.81, 174.61, 174.61, 130.81, 130.81, 130.81,
        196.00, 196.00, 196.00, 130.81, 196.00, 130.81
    };

    int notesLength = sizeof(notesFrequencies) / sizeof(notesFrequencies[0]);
    int bassLength = sizeof(bassLineFrequencies) / sizeof(bassLineFrequencies[0]);
    int swingDurations[] = {5600, 2800};
    int volume_1 = 3000;
    int volume_2 = 9000;

    // Define a silence duration. Let's say 5% of your swing duration for simplicity.
    int silenceDuration = swingDurations[0] * 0.05;

    // Adjust buffer size calculation to account for additional silence
    int BUFFER_SIZE_1 = ((8000 + silenceDuration) * notesLength) / 2; // Assuming each note duration includes the silence duration
    int BUFFER_SIZE_2 = ((8000 + silenceDuration) * bassLength) / 2;

    short int buffer_1[BUFFER_SIZE_1];
    short int buffer_2[BUFFER_SIZE_2];
    int idx = 0, idy = 0;

    // Initialize buffers
    memset(buffer_1, 0, sizeof(short int) * BUFFER_SIZE_1);
    memset(buffer_2, 0, sizeof(short int) * BUFFER_SIZE_2);

    int noteDurationIndex = 0;
    
    // Fill the buffers with notes and silences
    for (int noteIdx = 0; noteIdx < notesLength; noteIdx++) {
        double freq1 = notesFrequencies[noteIdx];
        int period1 = 8000 / freq1;
        double freq2 = bassLineFrequencies[noteIdx % bassLength];
        int period2 = 8000 / freq2;

        int noteDuration = swingDurations[noteDurationIndex % (sizeof(swingDurations) / sizeof(swingDurations[0]))];
        noteDurationIndex++;

        // Fill melody and silence
        for (i = 0; i < noteDuration; i++) {
            if (idx < BUFFER_SIZE_1) {
                double sample1 = volume_1 * sin(2 * M_PI * i / period1);
                buffer_1[idx++] = (short)(sample1);
            }
        }
        for (i = 0; i < silenceDuration; i++) {
            if (idx < BUFFER_SIZE_1) buffer_1[idx++] = 0;
        }

        // Fill bass and silence
        for (i = 0; i < noteDuration; i++) {
            if (idy < BUFFER_SIZE_2) {
                double sample2 = volume_2 * sin(2 * M_PI * i / period2);
                buffer_2[idy++] = (short)(sample2);
            }
        }
        for (i = 0; i < silenceDuration; i++) {
            if (idy < BUFFER_SIZE_2) buffer_2[idy++] = 0;
        }
    }

    // Playback loop
    while (1) {
        int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
        int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

        // Melody output (left speaker)
        if (out_fifo_1 >= 128) {
            for (int j = 0; j < 128; j++) {
                *(audio_ptr + 3) = ((int)buffer_1[buffer_ptr_1]) << 16;
                buffer_ptr_1 = (buffer_ptr_1 + 1) % BUFFER_SIZE_1;
            }
        }

        // Bass output (right speaker)
        if (out_fifo_2 >= 128) {
            for (int j = 0; j < 128; j++) {
                *(audio_ptr + 2) = ((int)buffer_2[buffer_ptr_2]) << 16;
                buffer_ptr_2 = (buffer_ptr_2 + 1) % BUFFER_SIZE_2;
            }
        }
    }

    return 0;
}
