#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
#define BUFFER_SIZE 1000000 // Define buffer size

short int buffer[BUFFER_SIZE];
int buffer_ptr = 0; // Initialize buffer pointer

int main() {
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = 0.0;
    }
    buffer_ptr = 0;
    //get_chord_wave(chord, duration, fs, buffer);

    // Output waveform to audio interface
    volatile int *audio_ptr = (int*) AUDIO_BASE;
    int period1 = 8000 / 440;
    int period2 = 8000 / 659;
    int period3 = 8000 / 554;
    int period_total = period1 + period2 + period3;
    double sample1, sample2, sample3;
    //short sample;
	int multiple = 20;
	int volume = 10000;

    while (1) {
        int out_fifo = (*(audio_ptr + 1) & 0xFF0000) >> 16;
        
		if (out_fifo >= period_total) {
            int i = 0;
		   // Generate waveform for chord
			for (i = 0; i < period1*multiple; i++) {
				sample1 = volume * sin(2 * M_PI * i / period1);  

				buffer[i] = (short)(sample1);
			}
            for (i=period1*multiple; i<period1*multiple+period2*multiple; i++){
                sample2 = volume * sin(2 * M_PI * i / period2);
                buffer[i] = (short)(sample2);
            }
            for (i=period1*multiple+period2*multiple; i<period_total*multiple; i++){
                sample3 = volume * sin(2 * M_PI * i / period3);
                buffer[i] = (short)(sample3);
            }
        
            for (int j = 0; j < period_total*multiple; j++) {
                *(audio_ptr + 2) = buffer[buffer_ptr] * 0xFFFF; // Write left channel sample
                *(audio_ptr + 3) = buffer[buffer_ptr] * 0xFFFF; // Write right channel sample
                buffer_ptr = (j + 1) % BUFFER_SIZE;
            }
        }
    }
    return 0;
}