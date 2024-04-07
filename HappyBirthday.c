#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
#define BREAK 0
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
volatile int *audio_ptr = (int *)AUDIO_BASE;

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
  double notesFrequencies[26][2] = {// Pre Measure 3 Notes
                                    {C1, 2},
                                    {C4, 0.5},
                                    {C4, 0.5},
                                    // First Measure 6 Notes
                                    {D4, 1},
                                    {C4, 1},
                                    {F4, 1},
                                    {E4, 2},
                                    {C4, 0.5},
                                    {C4, 0.5},
                                    // Second Measure 6 Notes
                                    {D4, 1},
                                    {C4, 1},
                                    {G4, 1},
                                    {F4, 2},
                                    {C4, 0.5},
                                    {C4, 0.5},
                                    // Third Measure 7 Notes
                                    {C5, 1},
                                    {A4, 1},
                                    {F4, 1},
                                    {E4, 1},
                                    {D4, 1},
                                    {As4, 0.5},
                                    {As4, 0.5},
                                    // Fourth Measure 4 Notes
                                    {A4, 1},
                                    {F4, 1},
                                    {G4, 1},
                                    {F4, 3}};
  double bassLineFrequencies[24][4] = {// Pre 1 Note
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

  // int notesLength = 21;
  // int bassLength = 21;
  // int swingDurations[] = {2800, 2800};
  int volume_1 = 3000;
  int volume_2 = 12000;

  // Define a silence duration. Let's say 5% of your swing duration for
  // simplicity.
  int silenceDuration = 400;
  int tempo = 2.0;
  double totalDurationSeconds_1 = 0;
  double totalDurationSeconds_2 = 0;
  for (int noteIdx = 0; noteIdx < 26; noteIdx++) {
    totalDurationSeconds_1 +=
        notesFrequencies[noteIdx][1] / tempo +
        (silenceDuration / 8000.0);  // Adding note duration and silence
  }
  for (int noteIdx = 0; noteIdx < 24; noteIdx++) {
    totalDurationSeconds_2 +=
        bassLineFrequencies[noteIdx][3] / tempo +
        (silenceDuration / 8000.0);  // Adding note duration and silence
  }
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

  // MELODY
  double freq1, period1, sample1;
  for (int noteIdx = 0; noteIdx < 26; noteIdx++) {
    freq1 = notesFrequencies[noteIdx][0];
    period1 = 8000 / freq1;

    // int noteDuration =tempo * swingDurations[noteDurationIndex %
    // (sizeof(swingDurations) / sizeof(swingDurations[0]))];
    int noteDuration = 8000 * notesFrequencies[noteIdx][1] / tempo;
    noteDurationIndex++;

    // Fill melody and silence
    for (i = 0; i < noteDuration; i++) {
      if (idx < BUFFER_SIZE_1) {
        sample1 = volume_1 * sin(2 * M_PI * i / period1);
        buffer_1[idx++] = (short)(sample1);
      }
    }
    for (i = 0; i < silenceDuration; i++) {
      if (idx < BUFFER_SIZE_1) buffer_1[idx++] = 0;
    }
  }

  // BASS
  double freq2_1, freq2_2, freq2_3, period2_1, period2_2, period2_3, sample2_1,
      sample2_2, sample2_3;
  for (int noteIdx = 0; noteIdx < 24; noteIdx++) {
    freq2_1 = bassLineFrequencies[noteIdx][0];
    freq2_2 = bassLineFrequencies[noteIdx][1];
    freq2_3 = bassLineFrequencies[noteIdx][2];

    period2_1 = 8000 / freq2_1;
    period2_2 = 8000 / freq2_2;
    period2_3 = 8000 / freq2_3;

    // int noteDuration =tempo * swingDurations[noteDurationIndex %
    // (sizeof(swingDurations) / sizeof(swingDurations[0]))];
    int noteDuration = 8000 * bassLineFrequencies[noteIdx][3] / tempo;
    noteDurationIndex++;

    // Fill bass and silence
    for (i = 0; i < noteDuration; i++) {
      if (idy < BUFFER_SIZE_2) {
        sample2_1 = volume_2 * sin(2 * M_PI * i / period2_1);
        sample2_2 = volume_2 * sin(2 * M_PI * i / period2_2);
        sample2_3 = volume_2 * sin(2 * M_PI * i / period2_3);
        buffer_2[idy++] = (short)((sample2_1 + sample2_2 + sample2_3) / 3);
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