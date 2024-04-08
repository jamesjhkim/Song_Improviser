#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
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

volatile int *audio_ptr = (int *)AUDIO_BASE;
double sineLookupTable[SINE_TABLE_SIZE];

double noteFrequencies[37] = {C3,  Cs3, D3,  Ds3, E3,  F3,  Fs3, G3,  Gs3, A3,
                              As3, B3,  C4,  Cs4, D4,  Ds4, E4,  F4,  Fs4, G4,
                              Gs4, A4,  As4, B4,  C5,  Cs5, D5,  Ds5, E5,  F5,
                              Fs5, G5,  Gs5, A5,  As5, B5,  C6};

void fillSineLookupTable() {
  for (int i = 0; i < SINE_TABLE_SIZE; ++i) {
    sineLookupTable[i] = sin(2 * PI * i / SINE_TABLE_SIZE);
  }
}
double getSineFromTable(double phase) {
  // Assuming phase is given in radians and normalized to the range [0, 2π)
  int index = (int)(phase * SINE_TABLE_SIZE / (2 * PI)) % SINE_TABLE_SIZE;
  return (double)sineLookupTable[index];
}

// 2:25
int main() {
  int i = 0;
  int buffer_ptr_1 = 0;
  int buffer_ptr_2 = 0;

  // double notesFrequencies[] = {
  //     261.63, 261.63, 293.66, 261.63, 349.23, 329.63,
  //     261.63, 261.63, 293.66, 261.63, 392.00, 349.23,
  //     261.63, 261.63, 523.25, 440.00, 349.23, 329.63, 293.66,
  //     466.16, 466.16, 440.00, 349.23, 392.00, 349.23
  // };
  // double bassLineFrequencies[] = {
  //     130.81, 130.81, 130.81, 130.81, 130.81, 130.81,
  //     196.00, 196.00, 196.00, 196.00, 196.00, 196.00,
  //     130.81, 130.81, 174.61, 174.61, 130.81, 130.81, 130.81,
  //     196.00, 196.00, 196.00, 130.81, 196.00, 130.81
  // };
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
  

  double notesFrequencies[23][2] = {
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

  double bassLineFrequencies[18][4] = {
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

  // int notesLength = 21;
  // int bassLength = 21;
  // int swingDurations[] = {2800, 2800};
  int volume_1 = 3000;
  int volume_2 = 12000;

  // Define a silence duration. Let's say 5% of your swing duration for
  // simplicity.
  int silenceDuration = 100;
  int tempo = 2.0;
  double totalDurationSeconds_1 = 16;
  double totalDurationSeconds_2 = 16;
  // for (int noteIdx = 0; noteIdx < 38; noteIdx++) {
  //   totalDurationSeconds_1 +=
  //       notesFrequencies[noteIdx][1] / tempo +
  //       (silenceDuration / 8000.0);  // Adding note duration and silence
  // }
  // for (int noteIdx = 0; noteIdx < 24; noteIdx++) {
  //   totalDurationSeconds_2 +=
  //       bassLineFrequencies[noteIdx][3] / tempo +
  //       (silenceDuration / 8000.0);  // Adding note duration and silence
  // }
  int BUFFER_SIZE_1 =
      totalDurationSeconds_1 * 8000;  // Convert total duration to samples
  int BUFFER_SIZE_2 =
      totalDurationSeconds_2 * 8000;  // Convert total duration to samples

  // Adjust buffer size calculation to account for additional silence
  // int BUFFER_SIZE_1 = ((8000 + silenceDuration) * notesLength / tempo);  //
  // Assuming each note duration includes the silence duration int BUFFER_SIZE_2
  // = ((8000 + silenceDuration) * bassLength / tempo);

  short int buffer_1[BUFFER_SIZE_1];
  short int buffer_2[BUFFER_SIZE_2];
  int idx = 0, idy = 0;

  // Initialize buffers
  memset(buffer_1, 0, sizeof(short int) * BUFFER_SIZE_1);
  memset(buffer_2, 0, sizeof(short int) * BUFFER_SIZE_2);

  int noteDurationIndex = 0;

  fillSineLookupTable();

  // MELODY
  double freq1, period1;
  // for (int noteIdx = 0; noteIdx < 38; noteIdx++) {
  //   freq1 = notesFrequencies[noteIdx][0];
  //   period1 = 8000 / freq1;

  //   // int noteDuration =tempo * swingDurations[noteDurationIndex %
  //   // (sizeof(swingDurations) / sizeof(swingDurations[0]))];
  //   int noteDuration = 8000 * notesFrequencies[noteIdx][1] / tempo;
  //   noteDurationIndex++;

  //   // Fill melody and silence
  //   for (i = 0; i < noteDuration - silenceDuration; i++) {
  //     sample1 = volume_1 * sin(2 * PI * i / period1);
  //     buffer_1[idx++] = (short)(sample1);
  //   }
  //   for (i = 0; i < silenceDuration; i++) {
  //     buffer_1[idx++] = 0;
  //   }
  // }
  double sineValue, phase;
  for (int noteIdx = 0; noteIdx < 23; noteIdx++) {
    phase = 0.0;
    freq1 = notesFrequencies[noteIdx][0];
    period1 = 8000 / freq1;
    double phaseIncrement = 2 * PI / period1;
    int noteDuration = 8000 * notesFrequencies[noteIdx][1] / tempo;
    for (i = 0; i < noteDuration - silenceDuration; i++) {
      sineValue = volume_1 * getSineFromTable(phase);
      phase += phaseIncrement;
      buffer_1[idx++] = (short)(sineValue);
      if (phase >= 2 * PI) phase -= 2 * PI;
    }
    for (i = 0; i < silenceDuration; i++) {
      buffer_1[idx++] = 0;
    }
  }

  // BASS
  // double freq2_1, freq2_2, freq2_3, period2_1, period2_2, period2_3,
  // sample2_1,
  //     sample2_2, sample2_3, phase2_1, phase2_2, phase2_3;
  double phase2_1, phase2_2, phase2_3, sample2_1, sample2_2, sample2_3;
  for (int noteIdx = 0; noteIdx < 18; noteIdx++) {
    // freq2_1 = bassLineFrequencies[noteIdx][0];
    // freq2_2 = bassLineFrequencies[noteIdx][1];
    // freq2_3 = bassLineFrequencies[noteIdx][2];
    phase2_1 = 0.0;
    phase2_2 = 0.0;
    phase2_3 = 0.0;

    // int noteDuration =tempo * swingDurations[noteDurationIndex %
    // (sizeof(swingDurations) / sizeof(swingDurations[0]))];
    int noteDuration = 8000 * bassLineFrequencies[noteIdx][3] / tempo;
    noteDurationIndex++;

    // Fill bass and silence
    for (i = 0; i < noteDuration - silenceDuration; i++) {
      sample2_1 = volume_2 * getSineFromTable(phase2_1);
      sample2_2 = volume_2 * getSineFromTable(phase2_2);
      sample2_3 = volume_2 * getSineFromTable(phase2_3);
      phase2_1 += 2 * PI * bassLineFrequencies[noteIdx][0] / 8000;
      phase2_2 += 2 * PI * bassLineFrequencies[noteIdx][1] / 8000;
      phase2_3 += 2 * PI * bassLineFrequencies[noteIdx][2] / 8000;
      buffer_2[idy++] = (short)((sample2_1 + sample2_2 + sample2_3) / 3);
      if (phase2_1 >= 2 * PI) phase -= 2 * PI;
      if (phase2_2 >= 2 * PI) phase -= 2 * PI;
      if (phase2_3 >= 2 * PI) phase -= 2 * PI;
    }
    for (i = 0; i < silenceDuration; i++) {
      buffer_2[idy++] = 0;
    }
    // for (i = 0; i < noteDuration - silenceDuration; i++) {
    //   sample2_1 = volume_2 * sin(2 * PI * i / period2_1);
    //   sample2_2 = volume_2 * sin(2 * PI * i / period2_2);
    //   sample2_3 = volume_2 * sin(2 * PI * i / period2_3);
    //   buffer_2[idy++] = (short)((sample2_1 + sample2_2 + sample2_3) / 3);
    // }
    // for (i = 0; i < silenceDuration; i++) {
    //   buffer_2[idy++] = 0;
    // }
  }

  // Playback loop
  while (1) {
    int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
    int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

    // Melody output (left speaker)
    if (out_fifo_1 >= 128) {
      for (int j = 0; j < 128; j++) {
        *(audio_ptr + 3) = ((int)buffer_1[buffer_ptr_1]) << 16;
        buffer_ptr_1 = (buffer_ptr_1 + 1);
      }
    }

    // Bass output (right speaker)
    if (out_fifo_2 >= 128) {
      for (int j = 0; j < 128; j++) {
        *(audio_ptr + 2) = ((int)buffer_2[buffer_ptr_2]) << 16;
        buffer_ptr_2 = (buffer_ptr_2 + 1);
      }
    }
    if (buffer_ptr_1 >= BUFFER_SIZE_1 && buffer_ptr_2 >= BUFFER_SIZE_2) {
      buffer_ptr_1 = 0;
      buffer_ptr_2 = 0;
    }
  }
  return 0;
}
