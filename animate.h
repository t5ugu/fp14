#include "shape.h"

#ifndef ANIMATE_H
#define ANIMATE_H

// 内部では、秒は用いず、フレーム単位で扱うこと
#define FPS 20
// 何フレームか
#define FRAME_t int
// drawCircle には、このまま入れてよい
#define BALL_RAD 10 //px
#define GROUND_Y (CANVAS_HEIGHT - 50) //px

// c: crashTime + bounceTime + crashTime 枚のキャンバス
// asZero  <= t <= aZ + crashTime: つぶれている; 中心は (a.x, a.y+a.h)
// aZ + cT <= t <= aZ + bounceTime: 上に凸の山を描いている; 最高点 (a.x + a.w/2, a.y)
// aZ + bT <= t <= aZ + bT + cT: つぶれている; 中心は (a.x+a.w, a.y+a.h)
void animateBall(CANVAS* c, FRAME_t asZero, FRAME_t crashTime, FRAME_t bounceTime, struct atlas a, struct color ballColor);

#endif
