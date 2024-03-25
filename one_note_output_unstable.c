#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
#define BUFFER_SIZE 1000000 // Define buffer size

short int buffer[BUFFER_SIZE];
int buffer_ptr = 0; // Initialize buffer pointer

int main() {
    //generate_audio("A", 12, 44100);
    double duration = 12.0;
    int fs = 44100;
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0.0;
    }
    buffer_ptr = 0;
    //get_chord_wave(chord, duration, fs, buffer);

    // Output waveform to audio interface
    volatile int *audio_ptr = (int*) AUDIO_BASE;
    int period = 8000 / 440;

    while (1) {
        int out_fifo = (*(audio_ptr + 1) & 0xFF0000) >> 16;
        
        if (out_fifo >= period) {
           // Generate waveform for chord
            for (int i = 0; i < period; i++) {
                double sample = 5000 * sin(2 * M_PI * i / period);
                buffer[i] = (short)sample;
            }
        
            for (int i = 0; i < period; i++) {
                *(audio_ptr + 2) = buffer[buffer_ptr] * 0xFFFF; // Write left channel sample
                *(audio_ptr + 3) = buffer[buffer_ptr] * 0xFFFF; // Write right channel sample
                buffer_ptr = (i + 1) % BUFFER_SIZE;
            }
        }
    }
    return 0;
}