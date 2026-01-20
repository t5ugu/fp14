#include "animate.h"

/*
  ボールを「つぶれ → 放物運動 → つぶれ」で描画する関数
  cs        : キャンバス配列（1要素 = 1フレーム）
  asZero    : このアニメーションの開始フレーム
  crashTime : 地面でつぶれているフレーム数
  bounceTime: 跳ね上がっているフレーム数
  a         : ボールが動く範囲（左下基準）
  ballColor : ボールの色
*/
void animateBall(
    CANVAS* cs,
    FRAME_t asZero,
    FRAME_t crashTime,
    FRAME_t bounceTime,
    struct atlas a,
    struct color ballColor
) {
    FRAME_t stroke = crashTime + bounceTime + crashTime;
    double x, y, h;

    for (FRAME_t t = 0; t < stroke; t++) {
        FRAME_t frame = asZero + t;

        /* ① 地面でつぶれている状態 */
        if (t <= crashTime) {
            h = t * (1 - MAX_CRASH_RATE) / crashTime + MAX_CRASH_RATE;
            drawEllipse(cs[frame], (struct atlas){a.x - BALL_RAD, a.y - BALL_RAD, BALL_RAD * 2, (int)(2 * BALL_RAD * h)}, ballColor);
        }
        /* ② 放物運動で跳ね上がる状態 */
        else if (t < crashTime + bounceTime) {
            double r = (double)(t - crashTime) / bounceTime;

            /* xは等速、yは二次関数 */
            x = a.x + a.w * r;
            y = a.y + 4 * a.h * r * (1 - r);

            drawCircle(cs[frame], x, y, BALL_RAD, ballColor);
        }
        /* ③ 着地後につぶれる状態 */
        else {
            h = (double)(stroke - t) * (1 - MAX_CRASH_RATE) / crashTime + MAX_CRASH_RATE;
            drawEllipse(cs[frame], (struct atlas){a.x + a.w - BALL_RAD, a.y - BALL_RAD, BALL_RAD * 2, (int)(2 * BALL_RAD * h)}, ballColor);
        }
    }
}