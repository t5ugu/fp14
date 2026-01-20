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

    for (FRAME_t t = 0; t < stroke; t++) {
        FRAME_t frame = asZero + t;
        double x, y;

        /* ① 地面でつぶれている状態 */
        if (t < crashTime) {
            x = a.x;
            y = a.y + a.h;
            drawSquashedBall(cs[frame], (int)x, (int)y);
        }
        /* ② 放物運動で跳ね上がる状態 */
        else if (t < crashTime + bounceTime) {
            FRAME_t tb = t - crashTime;
            double r = (double)tb / bounceTime;

            /* xは等速、yは二次関数 */
            x = a.x + a.w * r;
            y = a.y + 4 * a.h * r * (1 - r);

            drawCircle(cs[frame], x, y, BALL_RAD, ballColor);
        }
        /* ③ 着地後につぶれる状態 */
        else {
            x = a.x + a.w;
            y = a.y + a.h;
            drawSquashedBall(cs[frame], (int)x, (int)y);
        }
    }
}