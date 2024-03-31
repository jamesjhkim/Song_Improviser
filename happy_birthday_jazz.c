#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
volatile int *audio_ptr = (int *)AUDIO_BASE;

int main() {
  int i = 0;
  int buffer_ptr_1 = 0;
  int buffer_ptr_2 = 0;  // Assuming there's a bass line to play

  // Assuming frequencies for "Happy Birthday" as an example melody
  double notesFrequencies[] = {
      261.63, 261.63, 293.66, 261.63, 349.23, 329.63,  // Happy Birthday to you
      261.63, 261.63, 293.66, 261.63, 392.00, 349.23,  // Happy Birthday to you
      261.63, 261.63, 523.25, 440.00, 349.23, 329.63,
      293.66,  // Happy Birthday dear [Name]
      466.16, 466.16, 440.00, 349.23, 392.00, 349.23  // Happy Birthday to you
  };

  double bassLineFrequencies[] = {
      130.81, 130.81, 130.81, 130.81,
      130.81, 130.81,  // "Happy Birthday to you" (C chord)
      196.00, 196.00, 196.00, 196.00,
      196.00, 196.00,  // "Happy Birthday to you" (G7 chord)
      130.81, 130.81, 174.61, 174.61,
      130.81, 130.81, 130.81,  // "Happy Birthday dear [Name]" (C-F-C chords)
      196.00, 196.00, 196.00, 130.81,
      196.00, 130.81  // "Happy Birthday to you" (G7-C chords)
  };

  int notesLength = sizeof(notesFrequencies) / sizeof(notesFrequencies[0]);
  int bassLength = sizeof(bassLineFrequencies) / sizeof(bassLineFrequencies[0]);

  // Swing rhythm adjustment: Assuming 8 beats per note segment for simplicity
  int swingDurations[] = {5600, 2800};  // Adjust these durations for swing feel
  int volume_1 = 3000;
  int volume_2 = 6000;

  // Buffer 1 initialization
  int BUFFER_SIZE_1 = 8000 * notesLength;
  short int buffer_1[BUFFER_SIZE_1];
  int idx = 0;

  for (i = 0; i < BUFFER_SIZE_1; i++) {
    buffer_1[i] = 0;
  }

  // Buffer 2 intialization
  int BUFFER_SIZE_2 = 8000 * bassLength;
  short int buffer_2[BUFFER_SIZE_2];
  int idy = 0;

  for (i = 0; i < BUFFER_SIZE_2; i++) {
    buffer_2[i] = 0;
  }

  int noteDurationIndex = 0;
  // int bassDurationIndex = 0;

  for (int noteIdx = 0; noteIdx < notesLength; noteIdx++) {
    // Melody frequency
    double freq1 = notesFrequencies[noteIdx];
    int period1 = 8000 / freq1;

    // Bass frequency
    double freq2 = bassLineFrequencies[noteIdx % bassLength];
    int period2 = 8000 / freq2;

    int noteDuration =
        swingDurations[noteDurationIndex %
                       (sizeof(swingDurations) / sizeof(swingDurations[0]))];
    noteDurationIndex++;

    for (i = 0; i < noteDuration; i++) {
      // Melody buffer
      double sample1 = volume_1 * sin(2 * M_PI * i / period1);
      if (idx < BUFFER_SIZE_1) {
        buffer_1[idx++] = (short)(sample1);
      }

      // Bass buffer
      double sample2 = volume_2 * sin(2 * M_PI * i / period2);
      if (idy < BUFFER_SIZE_2) {
        buffer_2[idy++] = (short)(sample2);
      }
    }
  }

  // Play to speaker
  while (1) {
    int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
    int out_fifo_2 = (*(audio_ptr + 1) & 0xFF000000) >> 24;

    // Left speaker output (melody with swing rhythm)
    if (out_fifo_1 >= 128) {
      for (int j = 0; j < 128; j++) {
        *(audio_ptr + 3) = ((int)buffer_1[buffer_ptr_1]) << 16;
        buffer_ptr_1 = (buffer_ptr_1 + 1) % BUFFER_SIZE_1;
      }
    }

    // Right speaker output (bass line)
    if (out_fifo_2 >= 128) {
      for (int j = 0; j < 128; j++) {
        *(audio_ptr + 2) = ((int)buffer_2[buffer_ptr_2]) << 16;
        buffer_ptr_2 = (buffer_ptr_2 + 1) % BUFFER_SIZE_2;
      }
    }
  }

  return 0;
}