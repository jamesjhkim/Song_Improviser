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

volatile int *audio_ptr = (int *) AUDIO_BASE;
volatile int *LED_ptr = (int *) RLEDs;

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
short int* generateMelodyBuffer(double melody[][2], int melodySize, int volume, int* melodyBufferSize) {
    *melodyBufferSize = 0;
    for (int i = 0; i < melodySize; ++i) {
        *melodyBufferSize += (int)(SAMPLE_RATE * melody[i][1]);
    }

    short int* buffer = (short int*)malloc(*melodyBufferSize * sizeof(short int));
    if (!buffer) {
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
        exit(1);
    }

    int bufferIdx = 0;
    for (int i = 0; i < bassLineSize; ++i) {
        generateNoteWaveform(bassLine[i][0], bassLine[i][3], volume, buffer, &bufferIdx);
    }

    return buffer;
}

void delay(volatile int count) {
    while (count > 0) {
        --count;
    }
}

int main() {
  fillSineLookupTable();

  int volume = 3000;

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
    
    int melodyBufferSize1;
    int bassBufferSize1;
    
    short int* melody1Buffer = generateMelodyBuffer(song1_melody, melodySize1, volume, &melodyBufferSize1);
    short int* bass1Buffer = generateBassBuffer(song1_bass, bassSize1, volume, &bassBufferSize1);
	
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
    					*LED_ptr = 0b1111111111;
    
    					int melody1_ptr = 0, bass1_ptr = 0;
    					while (melody1_ptr < melodyBufferSize1 || bass1_ptr < bassBufferSize1) {
    						int canWriteLeft1 = ((*(audio_ptr + 1) & 0x00FF0000) >> 16) > 0;
    						int canWriteRight1 = ((*(audio_ptr + 1) & 0xFF000000) >> 24) > 0;
    
    						if (canWriteLeft1 && canWriteRight1) {
    							if (melody1_ptr < melodyBufferSize1) {
    								*(audio_ptr + 2) = melody1Buffer[melody1_ptr++] << 16; // Left channel
    							}
    							if (bass1_ptr < bassBufferSize1) {
    								*(audio_ptr + 3) = bass1Buffer[bass1_ptr++] << 16; // Right channel
    							}
    
    							delay(50); // Fine-tune this delay based on your system
    						}
    					}
    					*LED_ptr = 0;
    					break;
    					
    				default:
    					break;
    			}
        }
    }

	free(melody1Buffer);
	free(bass1Buffer);
	
  return 0;
}
