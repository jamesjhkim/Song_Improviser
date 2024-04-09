#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BREAK 0
#define C1 5
#define C3 130.813
#define Cs3 138.591
#define D3 146.832
#define Ds3 155.563
#define E3 164.813
#define F3 174.614
#define Fs3 184.997
#define G3 195.997
#define Gs3 207.652
#define A3 219.999
#define As3 233.081
#define B3 246.941
#define C4 261.625
#define Cs4 277.182
#define D4 293.664
#define Ds4 311.126
#define E4 329.627
#define F4 349.227
#define Fs4 369.993
#define G4 391.994
#define Gs4 415.304
#define A4 439.999
#define As4 466.163
#define B4 493.882
#define C5 523.250
#define Cs5 554.364
#define D5 587.328
#define Ds5 622.252
#define E5 659.253
#define F5 698.455
#define Fs5 739.987
#define G5 783.989
#define Gs5 830.607
#define A5 879.998
#define As5 932.325
#define B5 987.764
#define C6 1046.500
#define Cs6 1108.728
#define PI 3.14159265358979323846
#define SINE_TABLE_SIZE 1024
#define SAMPLE_RATE 8000
#define AUDIO_BASE 0xFF203040
#define RLEDs ((volatile long *) 0xFF200000)

volatile int *audio_ptr = (int *)AUDIO_BASE;
volatile int *LED_ptr = (int *)RLEDs;

double sineLookupTable[SINE_TABLE_SIZE];

void fillSineLookupTable() {
    for (int i = 0; i < SINE_TABLE_SIZE; ++i) {
        sineLookupTable[i] = sin(2 * PI * i / SINE_TABLE_SIZE);
    }
}

double getSineFromTable(double phase) {
    int index = (int)(phase * SINE_TABLE_SIZE / (2 * PI)) % SINE_TABLE_SIZE;
    return sineLookupTable[index];
}

int main() {
    fillSineLookupTable();

    int volume = 3000;

    double notesFrequencies[] = {C4, E4, G4, C5}; // Melody
    int notesCount = sizeof(notesFrequencies) / sizeof(notesFrequencies[0]);
    int durationInSeconds = 2; // Note duration

    // Dynamically allocate buffer memory
    int bufferSize = SAMPLE_RATE * durationInSeconds;
    short int *buffer = malloc(bufferSize * sizeof(short int) * notesCount);
    if (!buffer) {
        fprintf(stderr, "Memory allocation error\n");
        return 1;
    }

    // Generate waveform for each note
    for (int noteIndex = 0; noteIndex < notesCount; noteIndex++) {
        double frequency = notesFrequencies[noteIndex];
        double phaseIncrement = 2 * PI * frequency / SAMPLE_RATE;
        double phase = 0.0;
        
        for (int i = 0; i < bufferSize; i++) {
            buffer[noteIndex * bufferSize + i] = (short int)(volume * getSineFromTable(phase));
            phase += phaseIncrement;
            if (phase >= 2 * PI) {
                phase -= 2 * PI;
            }
        }
    }

    volatile int *PS2_ptr = (int *)0xFF200100; // PS/2 port address
    unsigned char byte3 = 0;
    int PS2_data, RVALID;

    while (1) {
        PS2_data = *(PS2_ptr);
        RVALID = (PS2_data & 0x8000);
        if (RVALID != 0) {
            byte3 = PS2_data & 0xFF;

            // Key 1 pressed
            if (byte3 == 0x16) {
                *LED_ptr = 0b1111111111; 
                int buffer_ptr = 0;

                // Check for space availability in output FIFO
                while (buffer_ptr < bufferSize * notesCount) {
                    int out_fifo_left = (*(audio_ptr + 1) & 0xFF0000) >> 16;
                    int out_fifo_right = (*(audio_ptr + 1) & 0xFF000000) >> 24;

                    if (out_fifo_left > 0 && out_fifo_right > 0) {
                        *(audio_ptr + 2) = ((int)buffer[buffer_ptr]) << 16; // Left channel
                        *(audio_ptr + 3) = ((int)buffer[buffer_ptr++]) << 16; // Right channel
                    }
                }
                *LED_ptr = 0b0000000000;
            }
        }
    }

    free(buffer); // Free allocated memory
    return 0;
}
