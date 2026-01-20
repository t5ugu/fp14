#include "img.h"
#include <math.h>

/* --- 木を描画する関数 --- */
/* a: 木全体を表示するエリア (x, y, h=高さ, w=幅) */
void drawTree(CANVAS c, struct atlas a) {
    struct color trunk_col = {101, 67, 33};  /* 茶色 */
    struct color leaf_col = {34, 139, 34};   /* 緑色 */

    /* 1. 幹を描く (長方形) */
    /* 高さはエリアの1/4, 幅はエリアの1/3 */
    int trunk_h = a.h / 4;
    int trunk_w = a.w / 3;

    drawRectangle(c, (struct atlas){a.x + trunk_w, a.y, trunk_w, trunk_h}, trunk_col);

    /* 2. 葉を描く (三角形3つ) */
    /* 幹の上からエリア上部にかけて3つ重ねる */
    struct atlas tri_area;
    int tri_h = (a.h - trunk_h) / 2 + 10; /* 三角形1つの高さ(少し重なるように) */

    /* 下の段 */
    tri_area.x = a.x;
    tri_area.y = a.y + trunk_h - 5;
    tri_area.w = a.w;           /* 横幅いっぱい */
    tri_area.h = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);

    /* 中の段 */
    tri_area.x = a.x + a.w/6;
    tri_area.y = a.y + trunk_h + 10;
    tri_area.w = a.w * 2/3;     /* 少し小さく */
    tri_area.h = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);

    /* 上の段 */
    tri_area.x = a.x + a.w/4;
    tri_area.y = a.y + trunk_h + 25; /* 一番上 */
    tri_area.w = a.w / 2;       /* もっと小さく */
    tri_area.h = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);
}

/* --- ゴールセットを描画する関数 --- */
void drawGoal(CANVAS c, struct atlas a, int t) {
    struct color black = {0, 0, 0};          
    struct color pole_col = {192, 192, 192}; 
    struct color flag_col = {255, 0, 0};     

    /* 1. 穴を描く (黒い楕円) */
    struct atlas hole_area;
    hole_area.x = a.x + a.w/4;
    hole_area.y = a.y;
    hole_area.w = a.w / 2;
    hole_area.h = 8;
    drawEllipse(c, hole_area, black);

    /* 2. 棒を描く (長方形) */
    struct atlas pole_area;
    pole_area.x = a.x + a.w/2 - 2; /* 中央 */
    pole_area.y = a.y + 4;         
    pole_area.w = 4;               /* 棒の太さ */
    pole_area.h = a.h - 20;        /* 長さ */
    drawRectangle(c, pole_area, pole_col);

    /* 3. 旗を描く (波) */
    /* 棒のてっぺんから右へ */
    int wave_x = a.x + a.w/2;
    int wave_y = a.y + a.h - 40;
    int length = 35; /* 旗の長さ */
    int amp = 4;     /* 波の幅 */
    int omega = 1;   /* 波の数 */
    
    /* 旗を塗りつぶすために、少しずつYをずらして複数回線を引く */
    for(int i = 0; i < 20; i++) {
        /* theta(位相)に t を渡して動かす */
        drawWave(c, wave_x, wave_y + i, length, amp, omega, t); 
    }
}
