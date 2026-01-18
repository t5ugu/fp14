#include "shape.h"

#ifndef IMG_H
#define IMG_H

/* --------------------------------------------------
   アニメーション用パーツ描画関数 (Wrapper)
   -------------------------------------------------- */

/* 木を描く */
void drawTree(CANVAS c, struct atlas a);

/* ゴールセット（穴・棒・旗）を描く */
/* t: アニメーション用の時刻（フレーム数など） */
void drawGoal(CANVAS c, struct atlas a, int t);

/* つぶれたボールを描く */
void drawSquashedBall(CANVAS c, int x, int y);

#endif