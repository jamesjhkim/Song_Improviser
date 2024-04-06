#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
	
#define RLEDs ((volatile long *) 0xFF200000)
#define AUDIO_BASE 0xFF203040
volatile int *audio_ptr = (int *)AUDIO_BASE;
volatile int *LED_ptr = (int*) 0xFF200000;
	
void play_song();

int main() {
	unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
	
  	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address

	int PS2_data, RVALID;

	while (1) {
		PS2_data = *(PS2_ptr);	
		RVALID = (PS2_data & 0x8000);	
		if (RVALID != 0)
		{
			byte1 = byte2;
			byte2 = byte3;
			byte3 = PS2_data & 0xFF;
		}
            // Handle key press
            switch (byte3) {
                // Key '1'
                case 0x16:
				*LED_ptr = 0b0000000000; //LEDS off
                    break;
                // Key '2'
                case 0x1E: 
                    break;
                // Key '3'
                case 0x26: 
                    break;
                // Key '4'
                case 0x25:
                    break;
                // Key 'Space'
                case 0x2E: 
                    break;
                // Key 'Enter'
                case 0x5A: 
					play_song();
                    break;
                // Key 'a'
                case 0x1C: 
					*LED_ptr = 0b1111111111; //LEDS on
					//play_song();
                    break;
                // Key 'b'
                case 0x32: 
                    break;
                // Key 'c'
                case 0x21: 
                    break;
				default:
					break;
            }

            // Display last byte on Red LEDs
            *RLEDs = byte3;
    }

    return 0;
}

//#define AUDIO_BASE 0xFF203040
//volatile int *audio_ptr = (int *)AUDIO_BASE;

void play_song() {
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
}