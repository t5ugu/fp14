#include "animate.h"
// #include "shape.h" // animate.h で既に include 済みのため。
#include <stdio.h>
#include <stdlib.h>

static char fname[100];
static int frame_count = 0;

struct atlas* mkAtlas(int x, int y, int w, int h) {
  struct atlas* a = malloc(sizeof(struct atlas));
  a->x = x; a->y = y; a->w = w; a->h = h;
  return a;
}

CANVAS createCanvas() {
    CANVAS canvas = malloc(sizeof(struct color*) * CANVAS_WIDTH);
    for (int i = 0; i < CANVAS_WIDTH; i++) {
        canvas[i] = malloc(sizeof(struct color) * CANVAS_HEIGHT);
    }
    return canvas;
}

void writeCanvas(CANVAS c) {
  sprintf(fname, "frame_%04d.ppm", frame_count++);
  FILE *f = fopen(fname, "wb");
  if(f == NULL) { fprintf(stderr, "can't open %s\n", fname); exit(1); }
  fprintf(f, "P6\n%d %d\n255\n", CANVAS_WIDTH, CANVAS_HEIGHT);
  unsigned char buf[CANVAS_WIDTH][CANVAS_HEIGHT][3];
  for (int x = 0; x < CANVAS_WIDTH; x++) {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
      buf[x][y][0] = c[x][y].r;
      buf[x][y][1] = c[x][y].g;
      buf[x][y][2] = c[x][y].b;
    }
  }

  fwrite(buf, sizeof(buf), 1, f);
  fclose(f);
}

int main(void) {
  FRAME_t crashTime = 3, bounceTime = 14;
  int ballHeight = 300;

  for (int i = 0; i < 3; i++) {
    for (FRAME_t frame = 0; frame < (crashTime + bounceTime + crashTime); frame++) {
      CANVAS c = createCanvas();
      animateBall(c, crashTime, bounceTime, frame, (struct atlas){-100 + 200 * i, 300 - ballHeight, 200, ballHeight});
      writeCanvas(c);
    }
    ballHeight = (int)(ballHeight*0.8);
  }

  return 0;
}