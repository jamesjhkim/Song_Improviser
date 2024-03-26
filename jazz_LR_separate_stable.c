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

	// Sound 1 initalization
	int period1_1 = 70;
	int period1_2 = 30;
	int period1_3 = 60;
	int period1_4 = 40;

	int period_total_1 = period1_1 + period1_2 + period1_3 + period1_4;
	double sample1_1, sample1_2, sample1_3, sample1_4;

	int multiple_1 = 25;
	int volume_1 = 7500;

	int BUFFER_SIZE_1 = multiple_1 * (period_total_1);
	short int buffer_1[BUFFER_SIZE_1];	
	for (int i = 0; i < BUFFER_SIZE_1; i++) {
		buffer_1[i] = 0.0;
	}

	// Sound 2 intialization
	int period2_1 = 75;
	int period2_2 = 150;

	int period_total_2 = period2_1 + period2_2;
	double sample2_1, sample2_2;

	int multiple_2 = 25;
	int volume_2 = 5000;

	int BUFFER_SIZE_2 = multiple_2 * (period_total_2);
	short int buffer_2[BUFFER_SIZE_2]; 
	for (int i = 0; i < BUFFER_SIZE_2; i++) {
		buffer_2[i] = 0.0;
	}

	// Generate waveform for chord 1 (melody)
	for (i = 0; i < period1_1 * multiple_1; i++) {
		sample1_1 = volume_1 * cos(2 * M_PI * i / period1_1);

		buffer_1[i] = (short)(sample1_1);
	}
	for (i = period1_1 * multiple_1; i < period1_1 * multiple_1 + period1_2 * multiple_1; i++) {
		sample1_2 = volume_1 * cos(2 * M_PI * i / period1_2);
		buffer_1[i] = (short)(sample1_2);
	}
	for (i = period1_1 * multiple_1 + period1_2 * multiple_1; i < period_total_1 * multiple_1; i++) {
		sample1_3 = volume_1 * cos(2 * M_PI * i / period1_3);
		buffer_1[i] = (short)(sample1_3);
	}
	for (i = period1_1 * multiple_1 + period1_2 * multiple_1 + period1_3 * multiple_1; 
		 i < period_total_1 * multiple_1; i++) {
		sample1_4 = volume_1 * cos(2 * M_PI * i / period1_4);
		buffer_1[i] = (short)(sample1_4);
	}

	// Generate wavefrom for chord 2 (base)
	for (i = 0; i < period2_1 * multiple_2; i++) {
		sample2_1 = volume_2 * sin(2 * M_PI * i / period2_1);

		buffer_2[i] = (short)(sample2_1);
	}
	for (i = period2_1 * multiple_2; i < period2_1 * multiple_2 + period2_2 * multiple_2; i++) {
		sample2_2 = volume_2 * sin(2 * M_PI * i / period2_2);
		buffer_2[i] = (short)(sample2_2);
	}

	while (1) {
		int out_fifo = (*(audio_ptr + 1) & 0xFF0000) >> 16;

		// Left speaker output (chord 1)
		if (out_fifo >= period_total_1 / 30) {
			for (int j = 0; j < period_total_1 / 30; j++) {
				*(audio_ptr + 2) = buffer_1[buffer_ptr_1] * 0xFFFF;    
				buffer_ptr_1 = (buffer_ptr_1 + 1) % BUFFER_SIZE_1;
			}
		}

		// Right speaker output (chord 2)
		if (out_fifo >= period_total_2 / 30) {
			for (int j = 0; j < period_total_2 / 30; j++) {
				*(audio_ptr + 3) = buffer_2[buffer_ptr_2] * 0xFFFF;  
				buffer_ptr_2 = (buffer_ptr_2 + 1) % BUFFER_SIZE_2;
			}
		}

	}
	return 0;
}