#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
	
#define RLEDs ((volatile long *) 0xFF200000)
#define AUDIO_BASE 0xFF203040
#define HEX_BASE ((volatile int *) 0xFF200020) // Example base address
	
volatile int *audio_ptr = (int *)AUDIO_BASE;
volatile int *LED_ptr = (int*) 0xFF200000;

#define C1 32.703
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

double song1_treble[] = {C4, C4, D4, C4, F4, E4, 
						 C4, C4, D4, C4, G4, F4,
						 C4, C4, C5, A4, F4, E4, D4, 
						 As4, As4, A4, F4, G4, F4};

double song1_bass[] =  {C3, C3, C3, C3, C3, C3,
						G3, G3, G3, G3, G3, G3,
						C3, C3, F3, F3, C3, C3, C3,
						G3, G3, G3, C3, G3, C3};

double song2_treble[] = {
    A4, A4, A4, G4, F4, // "Fly me to the moon"
    E4, F4, G4,        // "Let me play among the stars"
    A4, A4, G4, F4,    // "Let me see what spring is like"
    E4, G4, A4, A4,    // "On Jupiter and Mars"
    G4, F4             // "In other words"
    // Add more notes as needed
};

// Accompaniment (Bass) notes for "Fly Me to the Moon"
double song2_bass[] = {
    A3, D3, G3, C3, F3, // Basic chord roots for the song's progression
    B3, E3, A3, D3, G3,
    C3, F3, B3, E3, A3,
    // Add more notes or adjust to fit your arrangement
};

//void play_song();

int main() {
	unsigned char byte1 = 0;
	unsigned char byte2 = 0;
	unsigned char byte3 = 0;
	
  	volatile int * PS2_ptr = (int *) 0xFF200100;  // PS/2 port address

	int PS2_data, RVALID;

// Song 1
    int buffer_ptr_1 = 0;
    int buffer_ptr_2 = 0;
	
    int notesLength1 = sizeof(song1_treble) / sizeof(song1_treble[0]);
    int bassLength1 = sizeof(song1_bass) / sizeof(song1_bass[0]);
    int swingDurations[] = {5600, 2800};
    int volume_1 = 3000;
    int volume_2 = 9000;

    // Define a silence duration. Let's say 5% of your swing duration for simplicity.
    int silenceDuration = swingDurations[0] * 0.05;

    // Adjust buffer size calculation to account for additional silence
    int BUFFER_SIZE1_1 = ((8000 + silenceDuration) * notesLength1) / 2; // Assuming each note duration includes the silence duration
    int BUFFER_SIZE1_2 = ((8000 + silenceDuration) * bassLength1) / 2;

    short int buffer1_1[BUFFER_SIZE1_1];
    short int buffer1_2[BUFFER_SIZE1_2];
    int idx = 0, idy = 0;
	
    // Initialize buffers
    memset(buffer1_1, 0, sizeof(short int) * BUFFER_SIZE1_1);
    memset(buffer1_2, 0, sizeof(short int) * BUFFER_SIZE1_2);

    int noteDurationIndex = 0;
	
	
	int notesLength2 = sizeof(song2_treble) / sizeof(song2_treble[0]);
    int bassLength2 = sizeof(song2_bass) / sizeof(song2_bass[0]);
    //int swingDurations[] = {4000, 2800};
    //int volume_1 = 3000;
    //int volume_2 = 9000;

    // Define a silence duration. Let's say 5% of your swing duration for simplicity.
    //int silenceDuration = swingDurations[0] * 0.05;

    // Adjust buffer size calculation to account for additional silence
    int BUFFER_SIZE2_1 = ((8000 + silenceDuration) * notesLength2) / 2; // Assuming each note duration includes the silence duration
    int BUFFER_SIZE2_2 = ((8000 + silenceDuration) * bassLength2) / 2;

    short int buffer2_1[BUFFER_SIZE2_1];
    short int buffer2_2[BUFFER_SIZE2_2];
    //int idx = 0, idy = 0;
	
    // Initialize buffers
    memset(buffer2_1, 0, sizeof(short int) * BUFFER_SIZE2_1);
    memset(buffer2_2, 0, sizeof(short int) * BUFFER_SIZE2_2);

    //int noteDurationIndex = 0;
    
	
	while (1) {
		PS2_data = *(PS2_ptr);	
		RVALID = (PS2_data & 0x8000);	
		if (RVALID != 0)
		{
			//byte1 = byte2;
			//byte2 = byte3;
			byte3 = PS2_data & 0xFF;
			// Handle key press
		}
		    // Fill the buffers with notes and silences
		for (int noteIdx = 0; noteIdx < notesLength1; noteIdx++) {
			double freq1 = song1_treble[noteIdx];
			int period1 = 8000 / freq1;
			double freq2 = song1_bass[noteIdx % bassLength1];
			int period2 = 8000 / freq2;

			int noteDuration1 = swingDurations[noteDurationIndex % (sizeof(swingDurations) / sizeof(swingDurations[0]))];
			noteDurationIndex++;

			// Fill melody and silence
			for (int i = 0; i < noteDuration1; i++) {
				if (idx < BUFFER_SIZE1_1) {
					double sample1 = volume_1 * sin(2 * M_PI * i / period1);
					buffer1_1[idx++] = (short)(sample1);
				}
			}
			for (int i = 0; i < silenceDuration; i++) {
				if (idx < BUFFER_SIZE1_1) buffer1_1[idx++] = 0;
			}

			// Fill bass and silence
			for (int i = 0; i < noteDuration1; i++) {
				if (idy < BUFFER_SIZE1_2) {
					double sample2 = volume_2 * sin(2 * M_PI * i / period2);
					buffer1_2[idy++] = (short)(sample2);
				}
			}
			for (int i = 0; i < silenceDuration; i++) {
				if (idy < BUFFER_SIZE1_2) buffer1_2[idy++] = 0;
			}
		}
		
		
		// Fill the buffers with notes and silences
		for (int noteIdx = 0; noteIdx < notesLength2; noteIdx++) {
			double freq1 = song2_treble[noteIdx];
			int period1 = 8000 / freq1;
			double freq2 = song2_bass[noteIdx % bassLength2];
			int period2 = 8000 / freq2;

			int noteDuration2 = swingDurations[noteDurationIndex % (sizeof(swingDurations) / sizeof(swingDurations[0]))];
			noteDurationIndex++;

			// Fill melody and silence
			for (int i = 0; i < noteDuration2; i++) {
				if (idx < BUFFER_SIZE2_1) {
					double sample1 = volume_1 * sin(2 * M_PI * i / period1);
					buffer2_1[idx++] = (short)(sample1);
				}
			}
			for (int i = 0; i < silenceDuration; i++) {
				if (idx < BUFFER_SIZE2_1) buffer2_1[idx++] = 0;
			}

			// Fill bass and silence
			for (int i = 0; i < noteDuration2; i++) {
				if (idy < BUFFER_SIZE2_2) {
					double sample2 = volume_2 * sin(2 * M_PI * i / period2);
					buffer2_2[idy++] = (short)(sample2);
				}
			}
			for (int i = 0; i < silenceDuration; i++) {
				if (idy < BUFFER_SIZE2_2) buffer2_2[idy++] = 0;
			}
		}
	
		
		switch (byte3) {
				// Key '1'
			case 0x16:
				*LED_ptr = 0b0000000001; //LEDS off

				buffer_ptr_1 = 0;
				buffer_ptr_2 = 0;
				
				int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
				int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

				// Melody output (left speaker)
				if (out_fifo_1 >= 128) {
					for (int j = 0; j < 128; j++) {
						*(audio_ptr + 3) = ((int)buffer1_1[buffer_ptr_1]) << 16;
						buffer_ptr_1 = (buffer_ptr_1 + 1) % BUFFER_SIZE1_1;
					}
				}

				// Bass output (right speaker)
				if (out_fifo_2 >= 128) {
					for (int j = 0; j < 128; j++) {
						*(audio_ptr + 2) = ((int)buffer1_2[buffer_ptr_2]) << 16;
						buffer_ptr_2 = (buffer_ptr_2 + 1) % BUFFER_SIZE1_2;
					}
				}

				*(audio_ptr + 1) = *(audio_ptr + 1) | 0x00000002; // Set FIFO reset bit to 1
        		*(audio_ptr + 1) = *(audio_ptr + 1) & (~0x00000002); // Then clear it back to 0
				
				break;

				// Key '2'
			case 0x1E: 
				*LED_ptr = 0b0000000010; //LEDS off
				//int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
				//int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

				buffer_ptr_1 = 0;
				buffer_ptr_2 = 0;
				
					// Melody output (left speaker)
					if (out_fifo_1 >= 128) {
						for (int j = 0; j < 128; j++) {
							*(audio_ptr + 3) = ((int)buffer2_1[buffer_ptr_1]) << 16;
							buffer_ptr_1 = (buffer_ptr_1 + 1) % BUFFER_SIZE2_1;
						}
					}

					// Bass output (right speaker)
					if (out_fifo_2 >= 128) {
						for (int j = 0; j < 128; j++) {
							*(audio_ptr + 2) = ((int)buffer2_2[buffer_ptr_2]) << 16;
							buffer_ptr_2 = (buffer_ptr_2 + 1) % BUFFER_SIZE2_2;
						}
					}
				*(audio_ptr + 1) = *(audio_ptr + 1) | 0x00000002; // Set FIFO reset bit to 1
        		*(audio_ptr + 1) = *(audio_ptr + 1) & (~0x00000002); // Then clear it back to 0
					
				break;
				// Key '3'
			case 0x26: 
				*LED_ptr = 0b0000000100; //LEDS off
				break;
				// Key '4'
			case 0x25:
				*LED_ptr = 0b0000001000; //LEDS off
				break;
				// Key 'Space'
			case 0x2E: 
				break;
				// Key 'Enter'
			case 0x5A: 
				*LED_ptr = 0b1111111111; //LEDS on
				//play_song();
				break;
				// Key 'a'
			case 0x1C: 
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
