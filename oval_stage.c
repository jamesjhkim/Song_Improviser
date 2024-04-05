#define NUM_SQUARE 4
#define R_CIRCLE 10
#define HEIGHT_LIMIT 150
#define SPEED 5
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

volatile int pixel_buffer_start;  // global variable
volatile int *pixel_ctrl_ptr = (int *)0xFF203020;
volatile int *status_register = (int *)0xFF20302C;

short int Buffer1[240][512];  // 240 rows, 512 (320 + padding) columns
short int Buffer2[240][512];

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
  int square_arr[NUM_SQUARE][3];     // xplot_top, yplot_top, colour
  int direction_arr[NUM_SQUARE][2];  // xdirec, ydirec

  // Initialize Square Location
  for (int i = 0; i < NUM_SQUARE; i++) {
    // circle_top = rand() % (319 - 2 * R_CIRCLE) + R_CIRCLE
    square_arr[i][0] = rand() % (319 - 4 * R_CIRCLE) + 2 * R_CIRCLE;
    square_arr[i][1] =
        (rand() % ((239 - HEIGHT_LIMIT) - 2 * R_CIRCLE)) + HEIGHT_LIMIT;
    square_arr[i][2] = 0x07E0;
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
    for (int count = 0; count < NUM_SQUARE; count++) {
      x_plot = square_arr[count][0];
      y_plot = square_arr[count][1];
      for (int i = x_plot - 2 * R_CIRCLE; i < x_plot + 2 * R_CIRCLE; i++) {
        draw_line(319 / 2, 0, i, y_plot - R_CIRCLE, (short)0xF81F);
      }
    }

    // Draw Circle
    for (int count = 0; count < NUM_SQUARE; count++) {
      int centre_x = square_arr[count][0];
      int centre_y = square_arr[count][1] - R_CIRCLE;
      for (int x_cnt = centre_x - (2 * R_CIRCLE);
           x_cnt < centre_x + (2 * R_CIRCLE); x_cnt++) {
        for (int y_cnt = centre_y - R_CIRCLE; y_cnt < centre_y + R_CIRCLE;
             y_cnt++) {
           if (((x_cnt - centre_x) * (x_cnt - centre_x) / (float)(4 * R_CIRCLE * R_CIRCLE)) +
                ((y_cnt - centre_y) * (y_cnt - centre_y) / (float)(R_CIRCLE * R_CIRCLE)) <= 1) {
                plot_pixel(x_cnt, y_cnt, (short)square_arr[count][2]);
            }
        }
      }
    }

    bool switch_x, switch_y;

    for (int count = 0; count < NUM_SQUARE; count++) {
      switch_x = false;
      switch_y = false;
      if (square_arr[count][0] <= 0 + R_CIRCLE ||
          square_arr[count][0] >= 319 - R_CIRCLE) {
        switch_x = true;
      }
      if (square_arr[count][1] <= 0 + HEIGHT_LIMIT ||
          square_arr[count][1] >= 239 - 2 * R_CIRCLE) {
        switch_y = true;
      }
      if (switch_x) {
        direction_arr[count][0] *= -1;
      }
      if (switch_y) {
        direction_arr[count][1] *= -1;
      }
      square_arr[count][0] += SPEED * direction_arr[count][0];
      square_arr[count][1] += SPEED * direction_arr[count][1];
    }

    wait_for_vsync();
    pixel_buffer_start = *(pixel_ctrl_ptr + 1);
  }
}