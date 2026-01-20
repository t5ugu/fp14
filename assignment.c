#include "animate.h"
#include "img.h"
#include "shape.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define GROUND_Y 50

static char fname[100];
static int frame_count = 0;
static const struct color bgColor = {0, 127, 255};
static const struct color ballColor = {192, 192, 192};
static const FRAME_t crashTime = 1, bounceTime = 30;
static const FRAME_t stroke = crashTime + bounceTime + crashTime;
static const FRAME_t halfStroke = stroke / 2;

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

void writeCanvases(CANVAS* cs, FRAME_t start, FRAME_t end) {
  unsigned char buf[CANVAS_HEIGHT][CANVAS_WIDTH][3];
  
  for (int i = start; i < end; i++) {
    sprintf(fname, "out/frame_%04d.ppm", frame_count++);
    FILE *f = fopen(fname, "wb");
    if(f == NULL) { fprintf(stderr, "can't open %s\n", fname); exit(1); }
    fprintf(f, "P6\n%d %d\n255\n", CANVAS_WIDTH, CANVAS_HEIGHT);
    for (int x = 0; x < CANVAS_WIDTH; x++) {
      for (int y = 0; y < CANVAS_HEIGHT; y++) {
        buf[CANVAS_HEIGHT-1-y][x][0] = cs[i][x][y].r;
        buf[CANVAS_HEIGHT-1-y][x][1] = cs[i][x][y].g;
        buf[CANVAS_HEIGHT-1-y][x][2] = cs[i][x][y].b;
      }
    }

    fwrite(buf, sizeof(buf), 1, f);
    fclose(f);
  }
}

void renderScene1(void) {
  int ballHeight = 180;
  
  // ボールが3回、跳ねながら左から右へ。
  CANVAS cs[stroke*3];
  for (int i = 0; i < stroke*3; i++) {
    cs[i] = createCanvas();
    clearCanvas(cs[i]);
  }
  for (int i = 0; i < 3; i++) {
    animateBall(cs, stroke * i, crashTime, bounceTime, (struct atlas){-100 + 200 * i, GROUND_Y, 200, ballHeight}, ballColor);
    ballHeight = (int)(ballHeight*0.6);
  }
  writeCanvases(cs, halfStroke, halfStroke + stroke*2);
}

void renderScene2(void) {
  struct atlas tree = {150, GROUND_Y, 120, 180};
  struct atlas goal = {320, GROUND_Y, 50, 150};

  // v0 on x0  -->  0 on x1 となるような等加速度運動

  int x0 = 100;
  double v0 = 200 / bounceTime;
  int x1 = goal.x + goal.w * 1.5;
  double a = (v0 * v0) / (2 * (x1 - x0));
  double t = v0 / a;
  int rollTime = (int) t-15; // 意味の薄い微調整
  int sceneTime = (int)(t/10 + 2) * 10; // 少し余裕を持たせる

  // 計算ここまで

  CANVAS cs[stroke + sceneTime];
  for (int i = 0; i < stroke + sceneTime; i++) {
    cs[i] = createCanvas();
    clearCanvas(cs[i]);
    drawTree(cs[i], tree);
    drawGoal(cs[i], goal, i);
  }
  
  // 跳ねて入ってきたボール
  animateBall(cs, 0, crashTime, bounceTime, (struct atlas){-100, GROUND_Y, 200, 65}, ballColor);
  
  // ボールが減速する
  for (int i = 0; i < rollTime; i++) {
    v0 -= a;
    x0 += (int)v0;
    drawCircle(cs[stroke + i], x0, GROUND_Y + BALL_RAD, BALL_RAD, ballColor);
  }
  writeCanvases(cs, halfStroke, halfStroke + sceneTime);
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
    drawTree(cs[i], (struct atlas){150, GROUND_Y + (int)yoffset, 120, 180});
    drawGoal(cs[i], (struct atlas){320, GROUND_Y + (int)yoffset, 50, 150}, bounceTime + 29 - i);
    yoffset = yoffset * scale;
  }
  writeCanvases(cs, 0, frames);
}

int main(void) {
  renderScene1();
  renderScene2();
  renderScene3();
  return 0;
}
