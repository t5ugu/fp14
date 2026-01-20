#ifndef SHAPE_H
#define SHAPE_H

struct color { unsigned char r, g, b; };

#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 300
#define CANVAS struct color**

// 図形を描く区間
// 左から x px, 上から y px, 右向きに w px, 下向きに h px
struct atlas { int x, y, w, h; };

void drawPixel(CANVAS c, int x, int y, struct color clr);

// 中心点(x, y)、半径 radius の円
void drawCircle(CANVAS c, int x, int y, int radius, struct color clr);

// 長方形
void drawRectangle(CANVAS c, struct atlas a, struct color clr);

// 傾きのない楕円
void drawEllipse(CANVAS c, struct atlas a, struct color clr);

// 二等辺三角形
void drawIsocelesTriangle(CANVAS c, struct atlas a, struct color clr);

// 点(x, y) を原点とする波。
// A sin(ωt + θ)  (0 <= t <= length)
void drawWave(CANVAS c, int x, int y, int length, int amplitude, int omega, int theta);

#endif
