#define NUM_CIRCLE 2
#define CIRCLE_R1 20
#define CIRCLE_R2 15
#define HEIGHT_LIMIT 180
#define SPEED 5

#include <inttypes.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>

const uint16_t test[40][40] = {
    {0, 0, 0, 0,  0, 0, 0, 0, 0, 0,    0,     0,     0,     0,
     0, 0, 0, 0,  0, 0, 0, 0, 0, 6339, 23243, 40179, 16904, 0,
     0, 0, 0, 00, 0, 0, 0, 0, 0, 0,    0,     0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,     0,     0,     0,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 8452, 31695, 48663, 31695, 6339,
     0, 0, 0, 0, 0, 0, 0, 0, 0, 0,    0,     0},
    {0,    0, 0, 0, 0, 0, 0, 0, 0, 0,     0,     0,     0,     0,
     0,    0, 0, 0, 0, 0, 0, 0, 0, 10597, 40211, 59196, 48631, 19049,
     2081, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0,     0},
    {0,     0, 0, 0, 0, 0, 0, 0, 0, 0,     0,     0,     0,     0,
     0,     0, 0, 0, 0, 0, 0, 0, 0, 14791, 48599, 63422, 61277, 35985,
     10565, 0, 0, 0, 0, 0, 0, 0, 0, 0,     0,     0},
    {0,     0,    0, 0, 0, 0, 0, 0, 0, 0,     0,     0,     0,     0,
     0,     0,    0, 0, 0, 0, 0, 0, 0, 16904, 54938, 63454, 65535, 52825,
     25356, 4194, 0, 0, 0, 0, 0, 0, 0, 0,     0,     0},
    {0,     0,     0,  0, 0, 0, 0,  0, 0, 0,     0,     0,     0,     0,
     0,     0,     0,  0, 0, 0, 00, 0, 0, 21130, 59228, 65503, 65535, 61309,
     42260, 14791, 00, 0, 0, 0, 0,  0, 0, 0,     0,     0},
    {0,    0, 0,    0,     0,     0,     0,     0,     0,     0,
     0,    0, 0,    0,     0,     0,     0,     0,     0,     0,
     00,   0, 2145, 27469, 63422, 65503, 65535, 65503, 57051, 31727,
     8420, 0, 0,    0,     0,     0,     0,     0,     0,     0},
    {0,     0,    0,    0,     0,     0,     0,     0,     0,     0,
     0,     0,    0,    0,     0,     0,     0,     0,     0,     0,
     0,     0,    6339, 33808, 63422, 65503, 65535, 65535, 65535, 46486,
     14791, 2113, 0,    0,     0,     0,     0,     0,     0,     0},
    {0,     0,    0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,    0,     0,     0,     0,     0,     00,    0,     00,
     0,     0,    14791, 38066, 63454, 65535, 65503, 65535, 65535, 59164,
     38034, 8484, 0,     2081,  0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     00,
     0,     0,     16936, 44373, 63454, 65535, 63422, 63422, 65503, 65535,
     50712, 23243, 2145,  0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     21162, 52857, 63422, 65535, 65535, 65535, 65535, 65535,
     61277, 42260, 12678, 0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     00,    25388, 59196, 63422, 65503, 63422, 65503, 63454, 65503,
     63454, 54970, 27501, 6339,  0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     4194,  29614, 63422, 65535, 63454, 52857, 46518, 50712, 59228,
     65535, 63422, 46518, 19017, 0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     8452,  35921, 65503, 65535, 65535, 42292, 21162, 25356, 44373,
     61309, 65535, 59196, 35921, 8420,  0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     0,     0,     0,    0,    0,
     0,     0,     0,     0,     0,     0,     0,     0,    0,    0,
     00,    14791, 40179, 65535, 63422, 63454, 35953, 8452, 6307, 19017,
     38066, 52857, 59196, 50744, 25324, 4226,  0,     0,    0,    0},
    {0,     0,     0,     0,     0,     0,     0,     0,    0,  0,
     0,     0,     0,     0,     0,     0,     0,     0,    0,  0,
     2081,  19017, 44405, 65535, 65535, 61309, 31695, 6371, 00, 0,
     12710, 33840, 54970, 61309, 40147, 8452,  0,     0,    0,  0},
    {0,    0,     0,     0,     0,     0,     0,     0, 0,    0,
     0,    0,     0,     0,     0,     0,     0,     0, 0,    0,
     0,    25388, 54938, 63422, 65535, 61341, 23243, 0, 0,    0,
     2081, 4226,  25356, 44373, 50744, 29614, 4226,  0, 2081, 0},
    {0,    0,     0,     0,     0,     0,     0,     0, 0, 0,
     0,    0,     0,     0,     0,     0,     0,     0, 0, 0,
     2113, 29582, 57083, 63454, 65535, 59164, 21162, 0, 0, 0,
     0,    2113,  6339,  21162, 31727, 25324, 8420,  0, 0, 0},
    {0, 0, 0, 0, 0, 0,    0,     0,     0,     0,     0,     0,     0,     0,
     0, 0, 0, 0, 0, 0,    4258,  35921, 61309, 65535, 63422, 52825, 19017, 0,
     0, 0, 0, 0, 0, 4194, 10565, 12646, 6307,  0,     0,     0},
    {0, 0, 0, 0, 0,    0, 0,    0,     0,     0,     0,     0,     0,     0,
     0, 0, 0, 0, 0,    0, 8452, 40179, 63454, 65535, 61309, 44405, 14791, 0,
     0, 0, 0, 0, 2081, 0, 0,    2081,  00,    0,     0,     0},
    {0, 0, 0, 0, 0, 0, 0,     0,     0,     0,     0,     0,     0,     0,
     0, 0, 0, 0, 0, 0, 12678, 44405, 65535, 65535, 61309, 38066, 12646, 0,
     0, 0, 0, 0, 0, 0, 0,     0,     0,     0,     0,     0},
    {0, 0, 0, 0, 0, 0,    0,     0,     0,     0,     0,     0,     0,    0,
     0, 0, 0, 0, 0, 2113, 19017, 48631, 65535, 65535, 61277, 31695, 8452, 0,
     0, 0, 0, 0, 0, 0,    0,     0,     0,     0,     0,     0},
    {0, 0, 0, 0, 0,  0,    0,     0,     0,     0,     0,     0,     0,    0,
     0, 0, 0, 0, 0,  4226, 25356, 52857, 65535, 65503, 57115, 25356, 4258, 0,
     0, 0, 0, 0, 00, 0,    0,     0,     0,     0,     0,     0},
    {0, 0, 0, 0, 0, 0,    0,     0,     0,     0,     0,     0,     0,    0,
     0, 0, 0, 0, 0, 6339, 29582, 54970, 65535, 65503, 54970, 19049, 2113, 0,
     0, 0, 0, 0, 0, 0,    0,     0,     0,     0,     0,     0},
    {0, 0, 00, 0,  0, 0,    0,     0,     0,     0,     0,     0,     0, 0,
     0, 0, 0,  00, 0, 4226, 40211, 57083, 63422, 65535, 54938, 12646, 0, 0,
     0, 0, 0,  0,  0, 0,    0,     0,     0,     0,     0,     0},
    {00,    0,     0,     0,     0,     0,     0, 0, 0, 0,
     00,    2113,  2113,  2113,  2081,  2081,  0, 0, 0, 8420,
     46518, 63422, 65535, 63422, 48631, 10565, 0, 0, 0, 0,
     0,     0,     0,     0,     0,     0,     0, 0, 0, 0},
    {0,     0,     0,     0,     0,     0,     0,     0,    0,    2145,
     8484,  14823, 19049, 21130, 19049, 19049, 12678, 6371, 2145, 12646,
     48631, 65503, 65535, 59196, 42292, 8452,  0,     0,    0,    0,
     0,     0,     0,     0,     0,     0,     0,     0,    0,    0},
    {0,     0,     00,    00,    0,     0,     2113,  6371,  21162, 27501,
     35985, 44405, 48631, 50712, 48631, 48599, 42292, 33840, 27469, 29614,
     55002, 65535, 65535, 59164, 35921, 6339,  0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     00,    0,     4194,  16904, 31695, 52857, 57051,
     61309, 65503, 65535, 65535, 65503, 63454, 65535, 61277, 55002, 52857,
     63454, 65535, 65503, 54970, 27501, 4258,  0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     4194,  19049, 40211, 57083, 63422, 65503,
     65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 63390,
     65535, 63454, 63422, 52857, 21130, 2145,  0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     2081,  0,     0,     14823, 40179, 59228, 65535, 63454, 65535,
     65535, 65535, 65503, 65535, 65535, 65535, 63422, 63422, 65535, 63422,
     65535, 65535, 65535, 50744, 14791, 00,    0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {00,    0,     0,     6339,  29582, 54938, 65535, 65503, 65535, 65535,
     65535, 65535, 65535, 65535, 65535, 65535, 65535, 65503, 65535, 63454,
     65535, 65535, 65503, 46486, 8484,  0,     00,    0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     12678, 40179, 61309, 65535, 65535, 65535, 65535,
     65535, 65535, 65535, 65535, 65535, 65535, 65535, 65503, 65535, 65535,
     63454, 65535, 61309, 42292, 2113,  00,    0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     12710, 42292, 63390, 65503, 63454, 65503, 65503,
     65503, 65503, 65503, 65535, 65535, 65535, 65535, 65503, 65503, 65535,
     63422, 65503, 57115, 38034, 2113,  00,    0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     10565, 40147, 61341, 65535, 63454, 65535, 65535,
     65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535, 65535,
     65535, 65503, 52825, 27501, 2081,  0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     00,    0,     4258,  27501, 52889, 65503, 65535, 65535, 65535,
     65535, 65535, 65535, 65535, 65535, 65535, 65535, 65503, 63454, 65535,
     65535, 63422, 42292, 14791, 00,    0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     12678, 35921, 55002, 65535, 65503, 65535,
     65503, 63454, 65503, 65535, 65535, 65535, 65535, 65503, 65535, 65535,
     63390, 50712, 27437, 2113,  0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {00,    0,     0,     0,     2081,  16904, 38034, 52857, 63454, 65535,
     65535, 65535, 65503, 65535, 65535, 65535, 65535, 65535, 65535, 61277,
     46518, 27469, 8484,  0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     0,     4194,  14791, 27469, 44405, 52825,
     59196, 61341, 63422, 65535, 63454, 61341, 61277, 55002, 48599, 38034,
     23243, 8420,  0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},
    {0,     0,     0,     0,     00,    0,     0,     4258,  14791, 27501,
     44373, 54938, 59228, 63422, 61341, 59164, 50712, 38066, 21162, 8452,
     2113,  0,     0,     00,    0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0},

};

volatile int pixel_buffer_start;  // global variable
volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int *status_register = (int *)0xFF20302C;

short int Buffer1[240][512];  // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];
int colour_disco[19] = {
    0xF800, 0xE00F, 0xFAC0, 0xF8B2, 0xFB7F, 0x001F, 0x1F9D, 0x07FF,
    0x67E0, 0xAFE6, 0x07D3, 0x07EF, 0xF8F9, 0xFFA,  0xF7C7,
};

void wait_for_vsync() {
  *pixel_ctrl_ptr = 1;
  while ((*status_register) & 0x01) {
    // Wait for 1/60th of a second (wait for status register S bit to go to 0)
  }
}

// code for subroutines (not shown)
void plot_pixel(int x, int y, short int line_colour) {
  volatile short int *one_pixel_address;

  one_pixel_address = pixel_buffer_start + (y << 10) + (x << 1);

  *one_pixel_address = line_colour;
}

// void claer_screen2() {}

void clear_screen() {
  int x, y = 0;
  short int colour = 0;

  for (x = 0; x < 320; x++) {
    for (y = 0; y < 240; y++) {
      plot_pixel(x, y, colour);
    }
  }
}

void swap(int *x, int *y) {
  int temp = *x;
  *x = *y;
  *y = temp;
}

void draw_line(int x0, int y0, int x1, int y1, short int line_colour) {
  bool is_steep = abs(y1 - y0) > abs(x1 - x0);
  if (is_steep) {
    swap(&x0, &y0);
    swap(&x1, &y1);
  }
  if (x0 > x1) {
    swap(&x0, &x1);
    swap(&y0, &y1);
  }

  int deltax = x1 - x0;
  int deltay = abs(y1 - y0);

  int error = -(deltax / 2);
  int y = y0;
  int y_step;

  if (y0 < y1) {
    y_step = 1;
  } else {
    y_step = -1;
  }

  for (int x = x0; x <= x1; x++) {
    if (is_steep) {
      plot_pixel(y, x, line_colour);
    } else {
      plot_pixel(x, y, line_colour);
    }
    error = error + deltay;

    if (error > 0) {
      y = y + y_step;
      error = error - deltax;
    }
  }
}

int main(void) {
  srand(time(NULL));
  int square_arr[NUM_CIRCLE][3];     // xplot_top, yplot_top, colour
  int direction_arr[NUM_CIRCLE][2];  // xdirec, ydirec

  // Initialize Square Location
  for (int i = 0; i < NUM_CIRCLE; i++) {
    // circle_top = rand() % (319 - 2 * CIRCLE_R1) + CIRCLE_R1
    square_arr[i][0] = rand() % (319 - 4 * CIRCLE_R1) + 2 * CIRCLE_R1;
    square_arr[i][1] =
        (rand() % ((239 - HEIGHT_LIMIT) - 2 * CIRCLE_R1)) + HEIGHT_LIMIT;
    square_arr[i][2] = 0xDD4C;
    direction_arr[i][0] = ((rand() % 2) * 2) - 1;
    direction_arr[i][1] = ((rand() % 2) * 2) - 1;
  }

  *(pixel_ctrl_ptr + 1) = (int)&Buffer1;
  wait_for_vsync();

  pixel_buffer_start = *pixel_ctrl_ptr;
  clear_screen();

  *(pixel_ctrl_ptr + 1) = (int)&Buffer2;
  pixel_buffer_start = *(pixel_ctrl_ptr + 1);

  while (1) {
    clear_screen();
    int x_plot, y_plot;

    // Draw Triangle
    for (int count = 0; count < NUM_CIRCLE; count++) {
      x_plot = square_arr[count][0];
      y_plot = square_arr[count][1];
      for (int i = x_plot - 2 * CIRCLE_R1; i < x_plot + 2 * CIRCLE_R1; i++) {
        if (count % 2 == 0)
          draw_line(0, 0, i, y_plot - CIRCLE_R1, (short)0xFFFB);
        else
          draw_line(319, 0, i, y_plot - CIRCLE_R1, (short)0xFFFB);
      }
    }

    // Draw Circle
    // for (int count = 0; count < NUM_CIRCLE; count++) {
    //   int centre_x = square_arr[count][0];
    //   int centre_y = square_arr[count][1] - CIRCLE_R1;
    //   int diameter = 2 * CIRCLE_R1;
    //   for (int x_cnt = centre_x - diameter; x_cnt < centre_x + diameter;
    //        x_cnt++) {
    //     for (int y_cnt = centre_y - CIRCLE_R1; y_cnt < centre_y + CIRCLE_R1;
    //          y_cnt++) {
    //       if ((pow(x_cnt - centre_x, 2) / (float)pow(diameter, 2)) +
    //               (pow(y_cnt - centre_y, 2) / (float)pow(CIRCLE_R1, 2)) <=
    //           1) {
    //         plot_pixel(x_cnt, y_cnt, (short)square_arr[count][2]);
    //       }
    //     }
    //   }
    // }

    for (int count = 0; count < NUM_CIRCLE; count++) {
      int centre_x = square_arr[count][0];
      int centre_y = square_arr[count][1] - CIRCLE_R1;
      int diameter = 2 * CIRCLE_R1;
      for (int x_cnt = centre_x - diameter; x_cnt < centre_x + diameter;
           x_cnt++) {
        for (int y_cnt = centre_y - CIRCLE_R1; y_cnt < centre_y + CIRCLE_R1;
             y_cnt++) {
          if ((pow(x_cnt - centre_x, 2) / (float)pow(diameter, 2)) +
                  (pow(y_cnt - centre_y, 2) / (float)pow(CIRCLE_R1, 2)) <=
              1) {
            plot_pixel(x_cnt, y_cnt, (short)square_arr[count][2]);
          }
        }
      }
    }

    for (int count = 0; count < NUM_CIRCLE; count++) {
      int x_i = square_arr[count][0] - 20;
      int y_i = square_arr[count][1] - 40;
      int drawing_size = 40;
      for (int i = 0; i < 40; i++) {
        for (int j = 0; j < 40; j++) {
          if (test[j][i] != 0) {
            plot_pixel(x_i + i, y_i + j, (short)0xF2B2);
          }
        }
      }
    }

    // Draw Disco Ball
    // int disco_x = 50;
    // int disco_r = 20;
    // int disco_y = disco_r;
    // for (int x_cnt = disco_x - disco_r; x_cnt < disco_x + disco_r; x_cnt++) {
    //   for (int y_cnt = disco_y - disco_r; y_cnt = disco_y + disco_r; y_cnt++)
    //   {
    //   }
    // }

    bool switch_x, switch_y;

    for (int count = 0; count < NUM_CIRCLE; count++) {
      switch_x = false;
      switch_y = false;
      if (square_arr[count][0] <= 0 + CIRCLE_R1 ||
          square_arr[count][0] >= 319 - CIRCLE_R1) {
        switch_x = true;
      }
      if (square_arr[count][1] <= 0 + HEIGHT_LIMIT ||
          square_arr[count][1] >= 239 - 2 * CIRCLE_R1) {
        switch_y = true;
      }
      if (switch_x) {
        direction_arr[count][0] *= -1;
      }
      if (switch_y) {
        direction_arr[count][1] *= -1;
      }
      square_arr[count][0] += SPEED * direction_arr[count][0];
      square_arr[count][1] += direction_arr[count][1];
    }

    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  }
}
