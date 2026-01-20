#include "animate.h"
#include "img.h"
// #include "shape.h" // animate.h で既に include 済み
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

static char fname[100];
static int frame_count = 0;
static struct color bgColor = {0, 127, 255};
static struct color ballColor = {192, 192, 192};
static struct color holeColor = {32, 32, 32};
static FRAME_t crashTime = 7, bounceTime = 26;

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

void clearCanvas(CANVAS c) {
  for (int x = 0; x < CANVAS_WIDTH; x++) {
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
      c[x][y] = bgColor;
    }
  }
}

void writeCanvas(CANVAS c) {
  sprintf(fname, "out/frame_%04d.ppm", frame_count++);
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

void writeConvases(CANVAS* cs, FRAME_t start, FRAME_t end) {
  for (int i = start; i < end; i++) {
    writeCanvas(cs[i]);
  }
}

void renderScene1(void) {
  int ballHeight = 180;
  FRAME_t stroke = crashTime + bounceTime + crashTime;
  FRAME_t halfStroke = stroke / 2;
  
  // ボールが3回、跳ねながら左から右へ。
  CANVAS cs[stroke*3];
  for (int i = 0; i < stroke*3; i++) {
    cs[i] = createCanvas();
    clearCanvas(cs[i]);
  }
  for (int i = 0; i < 3; i++) {
    animateBall(cs, stroke * i, crashTime, bounceTime, (struct atlas){-100 + 200 * i, GROUND_Y - ballHeight, 200, ballHeight}, ballColor);
    ballHeight = (int)(ballHeight*0.6);
  }
  writeConvases(cs, halfStroke, halfStroke + stroke*2);
}

void renderScene2(void) {
  struct atlas tree = {150, 70, 120, 180};
  struct atlas goal = {320, 100, 50, 150};

  CANVAS cs[bounceTime + 30];
  for (int i = 0; i < bounceTime + 30; i++) {
    cs[i] = createCanvas();
    clearCanvas(cs[i]);
    drawTree(cs[i], tree);
    drawGoal(cs[i], goal, i);
    drawEllipse(cs[i], (struct atlas){300, GROUND_Y - 7, 40, 14}, holeColor);
  }
  
  // 跳ねて入ってきたボール
  animateBall(cs, 0, 0, bounceTime, (struct atlas){-100, GROUND_Y - 65, 200, 65}, ballColor);
  
  // 徐々に減速して消える(ゴールに入るつもり。判定は無いので要調整)
  int x = 100;
  int velocity = 200 / bounceTime;
  // magic number
  for (int i = 0; i < 20; i++) {
    x += velocity;
    drawCircle(cs[bounceTime + i], x, 50, BALL_RAD, ballColor);
    velocity -= 1;
  }
  writeConvases(cs, 0, bounceTime + 30);
}

void renderScene3(void) {
  double yoffset = 1;
  double scale = 1.1;
  int frames = 10 + (int)(log10(CANVAS_HEIGHT) / log10(scale));
  // カメラが下方にパン。つぎのループへ。
  CANVAS cs[frames];
  for (int i = 0; i < frames; i++) {
    cs[i] = createCanvas();
    clearCanvas(cs[i]);
    drawTree(cs[i], (struct atlas){150, 70 - (int)yoffset, 120, 180});
    drawGoal(cs[i], (struct atlas){320, 100 - (int)yoffset, 50, 150}, bounceTime + 29 - i);
    drawEllipse(cs[i], (struct atlas){300, GROUND_Y - 7 - (int)yoffset, 40, 14}, holeColor);
    yoffset = yoffset * scale;
  }
  writeConvases(cs, 0, frames);
}

int main(void) {
  renderScene1();
  renderScene2();
  renderScene3();
  return 0;
}
