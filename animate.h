#include "shape.h"
// 内部では、秒は用いず、フレーム単位で扱うこと
#define FPS 20
// 何フレームか
#define FRAME_t int
// drawCircle には、このまま入れてよい
#define BALL_RAD 10 //px

// 0  <= t <= crashTime: つぶれている; 中心は (a.x, a.y+a.h)
// cT <= t <= bounceTime: 上に凸の山を描いている; 最高点 (a.x + a.w/2, a.y)
// bT <= t <= bT+cT: つぶれている; 中心は (a.x+a.w, a.y+a.h)
void animateBall(CANVAS c, FRAME_t crashTime, FRAME_t bounceTime, FRAME_t t, struct atlas a);
