#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define AUDIO_BASE 0xFF203040
// #define BUFFER_SIZE 1000000  // Define buffer size

// int buffer_ptr = 0;  // Initialize buffer pointer

int main() {
  int buffer_ptr1 = 0;
  int buffer_ptr2 = 0;
  // get_chord_wave(chord, duration, fs, buffer);

  // Output waveform to audio interface
  volatile int *audio_ptr = (int *)AUDIO_BASE;
  //   int period1 = 8000 / 440;
  //   int period2 = 8000 / 659;
  //   int period3 = 8000 / 554;
  int freq1 = 55 * 4;
  int freq2 = 69 * 4;
  int freq3 = 82 * 4;
  int freq4 = 73 * 4;
  int freq5 = 93 * 4;
  int freq6 = 440;
  int sampling_rate = 8000;
  int period1 = sampling_rate / freq1;
  int period2 = sampling_rate / freq2;
  int period3 = sampling_rate / freq3;
  int period4 = sampling_rate / freq4;
  int period5 = sampling_rate / freq5;

  int period_total1 = period1 + period2 + period3;
  int period_total2 = period1 + period4 + period5;
  double sample1, sample2, sample3, sample4, sample5;
  // short sample;
  int multiple = 100;
  int volume = 10000;
  int i = 0;
  int BUFFER_SIZE1 = multiple * (period_total1);
  int BUFFER_SIZE2 = multiple * (period_total2);
  short int buffer1[BUFFER_SIZE1];
  short int buffer2[BUFFER_SIZE2];
  for (int i = 0; i < BUFFER_SIZE1; i++) {
    buffer1[i] = 0.0;
  }
  for (int i = 0; i < BUFFER_SIZE2; i++) {
    buffer2[i] = 0.0;
  }
  // Generate waveform for chord
  for (i = 0; i < period1 * multiple; i++) {
    sample1 = volume * cos(2 * M_PI * i / period1);
    buffer1[i] = (short)(sample1);
  }
  for (i = period1 * multiple; i < period1 * multiple + period2 * multiple;
       i++) {
    sample2 = volume * cos(2 * M_PI * i / period2);
    buffer1[i] = (short)(sample2);
  }
  for (i = period1 * multiple + period2 * multiple;
       i < period_total1 * multiple; i++) {
    sample3 = volume * cos(2 * M_PI * i / period3);
    buffer1[i] = (short)(sample3);
  }

  for (i = 0; i < period1 * multiple; i++) {
    sample1 = volume * cos(2 * M_PI * i / period1);
    buffer2[i] = (short)(sample1);
  }
  for (i = period1 * multiple; i < period1 * multiple + period4 * multiple;
       i++) {
    sample4 = volume * cos(2 * M_PI * i / period4);
    buffer2[i] = (short)(sample4);
  }
  for (i = period1 * multiple + period4 * multiple;
       i < period_total2 * multiple; i++) {
    sample5 = volume * cos(2 * M_PI * i / period5);
    buffer2[i] = (short)(sample5);
  }

  int chord_num = 1;
  int chord_counter = 0;
  int mult2 = 100;
  int max_chord_counter1 = period_total1 * mult2;
  int max_chord_counter2 = period_total2 * mult2;
  while (1) {
    int out_fifo = (*(audio_ptr + 1) & 0xFF0000) >> 16;
    if (out_fifo >= period_total1 / 30 && chord_num == 1) {
      for (int j = 0; j < period_total1 / 30; j++) {
        *(audio_ptr + 2) =
            buffer1[buffer_ptr1] * 0xFFFF;  // Write left channel sample
        *(audio_ptr + 3) =
            buffer1[buffer_ptr1] * 0xFFFF;  // Write right channel sample
        buffer_ptr1 = (buffer_ptr1 + 1) % BUFFER_SIZE1;
      }
      chord_counter += 1;
      if (chord_counter >= max_chord_counter1) {
        chord_counter = 0;
        chord_num = 2;
      }
    }
    if (out_fifo >= period_total2 / 30 && chord_num == 2) {
      for (int j = 0; j < period_total2 / 30; j++) {
        *(audio_ptr + 2) =
            buffer2[buffer_ptr2] * 0xFFFF;  // Write left channel sample
        *(audio_ptr + 3) =
            buffer2[buffer_ptr2] * 0xFFFF;  // Write right channel sample
        buffer_ptr2 = (buffer_ptr2 + 1) % BUFFER_SIZE2;
      }
      chord_counter += 1;
      if (chord_counter >= max_chord_counter2) {
        chord_counter = 0;
        chord_num = 1;
      }
    }
  }
  return 0;
}