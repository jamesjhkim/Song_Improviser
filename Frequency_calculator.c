#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Assume MAX_SAMPLES and your audio sample array are defined as before
// #define MAX_SAMPLES 44100
// #define SAMPLE_RATE 44100
#define SAMPLE_RATE 8000
#define AUDIO_BASE 0xFF203040
#define DAMPING_FACTOR 0.3  // Damping factor M: 0 < M < 1
#define PI 3.14159265358979323846
#define SIZE_ARRAY (int)(8000 * 1.0)

volatile int *audio_ptr = (int *)AUDIO_BASE;

struct audio_t {
  volatile unsigned int control;
  volatile unsigned char rarc;
  volatile unsigned char ralc;
  volatile unsigned char wsrc;
  volatile unsigned char wslc;
  volatile unsigned int ldata;
  volatile unsigned int rdata;
};

struct audio_t *const audiop = (struct audio_t *)AUDIO_BASE;

// Output(t): stored in echo_array
int sound_array[SIZE_ARRAY];
int sound_cursor = 0;

int value;  // input(t) for both left and right

// Function to estimate frequency using zero-crossing
float estimateFrequencyByZeroCrossing(int *samples, int numSamples) {
  int zeroCrossings = 0;
  for (int i = 1; i < numSamples; i++) {
    if ((samples[i - 1] < 0 && samples[i] > 0) ||
        (samples[i - 1] > 0 && samples[i] < 0)) {
      zeroCrossings++;
    }
  }

  // Estimate frequency
  float durationInSeconds = (float)numSamples / SAMPLE_RATE;
  float frequency = zeroCrossings / (2 * durationInSeconds);
  return frequency;
}
// Part 4

// Part 2 but with echo

int main(void) {
  // Set array values to 0 to begin
  for (int count = 0; count < SIZE_ARRAY; count++) {
    sound_array[count] = 0;
  }
  while (1) {
    if (audiop->rarc > 0) {
      value = audiop->ldata;
      sound_array[sound_cursor] = value;
    }
    sound_cursor += 1;
    if (sound_cursor >= SIZE_ARRAY) break;
  }

  float estimatedFrequency =
      estimateFrequencyByZeroCrossing(sound_array, SIZE_ARRAY);

  int sampling_freq = 8000;
  int BUFFER_SIZE = 10 * sampling_freq;
  short int buffer[BUFFER_SIZE];
  memset(buffer, 0, sizeof(short int) * BUFFER_SIZE);

  double freq, period, sample;
  int idx = 0;
  freq = (double)estimatedFrequency;
  period = sampling_freq / freq;
  for (int i = 0; i < BUFFER_SIZE - 1000; i++) {
    sample = 300 * sin(2 * PI * i / period);
    buffer[idx++] = (short)(sample);
  }
  for (int i = 0; i < 1000; i++) {
    buffer[idx++] = 0;
  }
  idx = 0;
  int buffer_ptr = 0;
  while (1) {
    int out_fifo_1 = (*(audio_ptr + 1) & 0xFF0000) >> 16;
    if (out_fifo_1 >= 128) {
      for (int j = 0; j < 128; j++) {
        *(audio_ptr + 3) = ((int)buffer[buffer_ptr]) << 16;
      }
    }
    // if (buffer_ptr >= BUFFER_SIZE) {
    //   buffer_ptr = 0;
    // }
    if (buffer_ptr >= BUFFER_SIZE) {  // Check if all data has been written
      break;                          // Exit loop
    }
    return 0;
  }
  // while (1) {
  //     if (audiop->rarc > 0) { // Check for data to read
  //             //Input FIFO
  //         value = audiop->ldata;
  //         // Update input with damping factor for echoed sound
  //         value += DAMPING_FACTOR * echo_array[echo_position];
  //         // Store the new echoed value back into the array
  //         echo_array[echo_position] = value;
  //         //Output FIFO
  //         audiop->ldata = value;
  //         audiop->rdata = value;
  //         // Move to the next position in the array
  //         echo_position = (echo_position + 1) % SIZE_ARRAY;
  //     }
  // }
}

// int main() {
//     // Example: a simple array with your audio samples
//     int audioSamples[MAX_SAMPLES]; // This should be filled with your actual
//     audio data

//     // Estimate the frequency
//     float estimatedFrequency = estimateFrequencyByZeroCrossing(audioSamples,
//     MAX_SAMPLES); printf("Estimated Frequency: %f Hz\n", estimatedFrequency);

//     return 0;
// }
