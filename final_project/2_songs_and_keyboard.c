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

// Frequency definitions for musical notes
// ... Definitions for C1, C3, Cs3, D3, etc. ...

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

void generateNoteWaveform(double frequency, double duration, int volume, short int* buffer, int* bufferIdx) {
    int samples = (int)(SAMPLE_RATE * duration);
    double phaseIncrement = 2 * PI * frequency / SAMPLE_RATE;
    double phase = 0.0;

    for (int i = 0; i < samples; ++i) {
        buffer[*bufferIdx + i] = (short)(volume * getSineFromTable(phase));
        phase += phaseIncrement;
        if (phase >= 2 * PI) phase -= 2 * PI;
    }
    *bufferIdx += samples;
}

void generateBassWaveform(double frequency1, double frequency2,
                          double frequency3, double duration, int volume,
                          short int* buffer, int* bufferIdx) {
  int samples = (int)(SAMPLE_RATE * duration);
  double phase1 = 0.0;
  double phase2 = 0.0;
  double phase3 = 0.0;
  double value = 0.0;
  double phaseIncrement1 = 2 * PI * frequency1 / SAMPLE_RATE;
  double phaseIncrement2 = 2 * PI * frequency2 / SAMPLE_RATE;
  double phaseIncrement3 = 2 * PI * frequency3 / SAMPLE_RATE;

  for (int i = 0; i < samples; ++i) {
    value = volume / 3 *
            (getSineFromTable(phase1) + getSineFromTable(phase2) +
             getSineFromTable(phase3));

    buffer[*bufferIdx + i] = (short)(value);
    phase1 += phaseIncrement1;
    phase2 += phaseIncrement2;
    phase3 += phaseIncrement3;
    if (phase1 >= 2 * PI) phase1 -= 2 * PI;
    if (phase2 >= 2 * PI) phase2 -= 2 * PI;
    if (phase3 >= 2 * PI) phase3 -= 2 * PI;
  }
  *bufferIdx += samples;
}

short int* generateMelodyBuffer(double melody[][2], int melodySize, int volume, int* melodyBufferSize) {
    *melodyBufferSize = 0;
    for (int i = 0; i < melodySize; ++i) {
        *melodyBufferSize += (int)(SAMPLE_RATE * melody[i][1]);
    }

    short int* buffer = (short int*)malloc(*melodyBufferSize * sizeof(short int));
    if (!buffer) {
        printf("Failed to allocate memory for the audio buffer\n");
        exit(1);
    }

    int bufferIdx = 0;
    for (int i = 0; i < melodySize; ++i) {
        generateNoteWaveform(melody[i][0], melody[i][1], volume, buffer, &bufferIdx);
    }

    return buffer;
}

short int* generateBassBuffer(double bassLine[][4], int bassLineSize, int volume, int* bassBufferSize) {
    *bassBufferSize = 0;
    for (int i = 0; i < bassLineSize; ++i) {
        *bassBufferSize += (int)(SAMPLE_RATE * bassLine[i][3]);
    }

    short int* buffer = (short int*)malloc(*bassBufferSize * sizeof(short int));
    if (!buffer) {
        printf("Failed to allocate memory for the audio buffer\n");
        exit(1);
    }

    int bufferIdx = 0;
	for (int i = 0; i < bassLineSize; ++i) {
		generateBassWaveform(bassLine[i][0], bassLine[i][1], bassLine[i][2],
							 bassLine[i][3], volume, buffer, &bufferIdx);
	}

    return buffer;
}

// Simple delay function
void delay(volatile int count) {
    while (count > 0) {
        --count;
    }
}

int main() {
    fillSineLookupTable();

    int volume = 3000; // Example volume level

	int mel1, mel2, mel3, mel4;
	int bass1, bass2, bass3, bass4;
	mel1 = 12;
	mel2 = 16;
	mel3 = 19;
	mel4 = 17;
	bass1 = 0;
	bass2 = 4;
	bass3 = 8;
	bass4 = 5;
  
	double noteFrequencies[37] = {C3,  Cs3, D3,  Ds3, E3,  F3,  Fs3, G3,  Gs3, A3,
                              As3, B3,  C4,  Cs4, D4,  Ds4, E4,  F4,  Fs4, G4,
                              Gs4, A4,  As4, B4,  C5,  Cs5, D5,  Ds5, E5,  F5,
                              Fs5, G5,  Gs5, A5,  As5, B5,  C6};

	double song1_melody[23][2] = {
		// First Measure 5 Notes
		{noteFrequencies[mel1], 0.5},
		{noteFrequencies[mel1 + 7], 0.5},
		{noteFrequencies[mel1 + 4], 0.5},
		{noteFrequencies[mel1], 0.5},
		{noteFrequencies[mel1 + 2], 2},
		// Second Measure 5 Notes
		{noteFrequencies[mel2 + 4], 0.5},
		{noteFrequencies[mel2 + 4], 0.5},
		{noteFrequencies[mel2 + 7], 0.5},
		{noteFrequencies[mel2], 0.5},
		{noteFrequencies[mel2 + 2], 2},
		// Third Measure 8 Notes
		{noteFrequencies[mel3 + 7], 0.5},
		{noteFrequencies[mel3 + 5], 0.5},
		{noteFrequencies[mel3 + 3], 0.5},
		{noteFrequencies[mel3 + 3], 0.5},
		{noteFrequencies[mel4 + 5], 0.5},
		{noteFrequencies[mel4 + 3], 0.5},
		{noteFrequencies[mel4 + 2], 0.5},
		{noteFrequencies[mel4 + 9], 0.5},
		// Fourth Measure 5 Notes
		{noteFrequencies[mel1 + 7], 0.5},
		{noteFrequencies[mel1 + 4], 0.5},
		{noteFrequencies[mel1 + 7], 0.5},
		{noteFrequencies[mel1 + 12], 0.5},
		{noteFrequencies[mel1], 2},
	};

	int melodySize1 = sizeof(song1_melody) / sizeof(song1_melody[0]);
	
	double song1_bass[18][4] = {
		// First Measure 4 Notes
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 1},
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 1},
		{noteFrequencies[bass1], noteFrequencies[bass1], noteFrequencies[bass1],
		 0.5},
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 1.5},
		// Second Measure 4 Notes
		{noteFrequencies[bass2], noteFrequencies[bass2 + 4],
		 noteFrequencies[bass2 + 7], 1},
		{noteFrequencies[bass2], noteFrequencies[bass2 + 4],
		 noteFrequencies[bass2 + 7], 1},
		{noteFrequencies[bass2], noteFrequencies[bass2], noteFrequencies[bass2],
		 0.5},
		{noteFrequencies[bass2], noteFrequencies[bass2 + 4],
		 noteFrequencies[bass2 + 7], 1.5},
		// Third Measure 5 Notes
		{noteFrequencies[bass3], noteFrequencies[bass3 + 3],
		 noteFrequencies[bass3 + 7], 1},
		{noteFrequencies[bass3], noteFrequencies[bass3 + 3],
		 noteFrequencies[bass3 + 7], 1},
		{noteFrequencies[bass4], noteFrequencies[bass4], noteFrequencies[bass4],
		 0.5},
		{noteFrequencies[bass4], noteFrequencies[bass4], noteFrequencies[bass4],
		 0.5},
		{noteFrequencies[bass4], noteFrequencies[bass4 + 3],
		 noteFrequencies[bass4 + 7], 1},
		// Fourth Measure 5 Notes
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 0.5},
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 1},
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 0.5},
		{noteFrequencies[bass1], noteFrequencies[bass1 + 4],
		 noteFrequencies[bass1 + 7], 1},
		{noteFrequencies[bass1], noteFrequencies[bass1], noteFrequencies[bass1],
		 1},
	};
	
	int bassSize1 = sizeof(song1_bass) / sizeof(song1_bass[0]);
	
	double song2_melody[38][2] = {// Pre Measure 3 Notes
		{C1, 2},
		{C4, 0.66},
		{C4, 0.34},
		// First Measure 9 Notes
		{D4, 0.66},
		{D4, 0.34},
		{C4, 0.66},
		{C4, 0.34},
		{F4, 0.66},
		{F4, 0.34},
		{E4, 2},
		{C4, 0.66},
		{C4, 0.34},
		// Second Measure 9 Notes
		{D4, 0.66},
		{D4, 0.34},
		{C4, 0.66},
		{C4, 0.34},
		{G4, 0.66},
		{G4, 0.34},
		{F4, 2},
		{C4, 0.5},
		{C4, 0.5},
		// Third Measure 10 Notes
		{C5, 1},
		{A4, 0.66},
		{A4, 0.34},
		{F4, 0.66},
		{F4, 0.34},
		{E4, 0.66},
		{E4, 0.34},
		{D4, 1},
		{As4, 0.66},
		{As4, 0.34},
		// Fourth Measure 7 Notes
		{A4, 0.66},
		{A4, 0.34},
		{F4, 0.66},
		{F4, 0.34},
		{G4, 0.66},
		{G4, 0.34},
		{F4, 3}};
	
    int melodySize2 = sizeof(song2_melody) / sizeof(song2_melody[0]);

	double song2_bass[24][4] = {// Pre 1 Note
		{C1, C1, C1, 3},
		// First Measure 6 Notes
		{C3, E3, G3, 1},
		{C3, E3, G3, 1},
		{C3, E3, G3, 1},
		{D3, G3, B3, 2},
		{D3, G3, B3, 0.5},
		{D3, G3, B3, 0.5},
		// Second Measure 6 Notes
		{D3, G3, B3, 1},
		{D3, G3, B3, 1},
		{D3, G3, B3, 1},
		{C3, E3, G3, 2},
		{C3, E3, G3, 0.5},
		{C3, E3, G3, 0.5},
		// Third Measure 7 Notes
		{E3, G3, C4, 1},
		{E3, G3, C4, 1},
		{E3, G3, C4, 1},
		{F3, A3, C4, 1},
		{E3, A3, C4, 1},
		{E3, A3, C4, 0.5},
		{E3, A3, C4, 0.5},
		// Fourth Measure 4 Notes
		{C3, E3, G3, 1},
		{C3, E3, G3, 1},
		{D3, G3, B3, 1},
		{E3, G3, C4, 3}};
	
	int bassSize2 = sizeof(song2_bass) / sizeof(song2_bass[0]);

	int melodyBufferSize1;
	int bassBufferSize1;
	
    short int* melody1Buffer = generateMelodyBuffer(song1_melody, melodySize1, volume, &melodyBufferSize1);
	short int* bass1Buffer = generateBassBuffer(song1_bass, bassSize1, volume * 6, &bassBufferSize1);
	
    int melodyBufferSize2;
	int bassBufferSize2;
	
    short int* melody2Buffer = generateMelodyBuffer(song2_melody, melodySize2, volume, &melodyBufferSize2);
	short int* bass2Buffer = generateBassBuffer(song2_bass, bassSize2, volume * 6, &bassBufferSize2);
	
    volatile int *PS2_ptr = (int *)0xFF200100; // PS/2 port address
    unsigned char byte3 = 0;
    int PS2_data, RVALID;

	
    while (1) {
        PS2_data = *(PS2_ptr);
        RVALID = (PS2_data & 0x8000);
        if (RVALID) {
            byte3 = PS2_data & 0xFF;

			switch(byte3) {
				case 0x16:
					*LED_ptr = 0b1111111111; // Indicate playback

					int melody2_ptr = 0, bass2_ptr = 0;
					while (melody2_ptr < melodyBufferSize2 || bass2_ptr < bassBufferSize2) {
						int canWriteLeft2 = ((*(audio_ptr + 1) & 0x00FF0000) >> 16) > 0;
						int canWriteRight2 = ((*(audio_ptr + 1) & 0xFF000000) >> 24) > 0;

						// Only write when both channels are ready to accept data
						if (canWriteLeft2 && canWriteRight2) {
							if (melody2_ptr < melodyBufferSize2) {
								*(audio_ptr + 2) = melody2Buffer[melody2_ptr++] << 16; // Left channel
							}
							if (bass2_ptr < bassBufferSize2) {
								*(audio_ptr + 3) = bass2Buffer[bass2_ptr++] << 16; // Right channel
							}

							// Introduce a controlled delay to allow FIFO to consume data
							delay(50); // Fine-tune this delay based on your system
						}
					}
					*LED_ptr = 0; // Turn off LEDs after playback
					
					break;
					
				case 0x1E:
					*LED_ptr = 0b1111111111; // Indicate playback

					int melody1_ptr = 0, bass1_ptr = 0;
					while (melody1_ptr < melodyBufferSize1 || bass1_ptr < bassBufferSize1) {
						int canWriteLeft1 = ((*(audio_ptr + 1) & 0x00FF0000) >> 16) > 0;
						int canWriteRight1 = ((*(audio_ptr + 1) & 0xFF000000) >> 24) > 0;

						// Only write when both channels are ready to accept data
						if (canWriteLeft1 && canWriteRight1) {
							if (melody1_ptr < melodyBufferSize1) {
								*(audio_ptr + 2) = melody1Buffer[melody1_ptr++] << 16; // Left channel
							}
							if (bass1_ptr < bassBufferSize1) {
								*(audio_ptr + 3) = bass1Buffer[bass1_ptr++] << 16; // Right channel
							}

							// Introduce a controlled delay to allow FIFO to consume data
							delay(50); // Fine-tune this delay based on your system
						}
					}
					*LED_ptr = 0; // Turn off LEDs after playback
					break;
					
				default:
					break;
			}
        }
    }

    free(melody2Buffer); // Free the buffer
    free(bass2Buffer);
    free(melody1Buffer); // Free the buffer
    free(bass1Buffer);
	
    return 0;
}