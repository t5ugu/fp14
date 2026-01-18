#include "img.h"
#include <math.h>

/* --- 木を描画する関数 --- */
/* a: 木全体を表示するエリア (x, y, h=幅, w=高さ) */
void drawTree(CANVAS c, struct atlas a) {
    struct color trunk_col = {101, 67, 33};  /* 茶色 */
    struct color leaf_col = {34, 139, 34};   /* 緑色 */

    /* 1. 幹を描く (長方形) */
    /* 幅はエリアの1/4, 高さはエリアの1/3 */
    int trunk_w = a.h / 4;  /* 幹の幅 */
    int trunk_h = a.w / 3;  /* 幹の高さ */
    
    struct atlas trunk_area;
    trunk_area.x = a.x + (a.h - trunk_w) / 2; /* 中央揃え */
    trunk_area.y = a.y + (a.w - trunk_h);     /* 一番下 */
    trunk_area.h = trunk_w;                   /* atlasの定義通り h=横幅 */
    trunk_area.w = trunk_h;                   /* atlasの定義通り w=縦幅 */
    
    drawRectangle(c, trunk_area, trunk_col);

    /* 2. 葉を描く (三角形3つ) */
    /* 幹の上からエリア上部にかけて3つ重ねる */
    struct atlas tri_area;
    int tri_h = (a.w - trunk_h) / 2 + 10; /* 三角形1つの高さ(少し重なるように) */

    /* 下の段 */
    tri_area.x = a.x;
    tri_area.y = a.y + trunk_h - 5;
    tri_area.h = a.h;           /* 横幅いっぱい */
    tri_area.w = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);

    /* 中の段 */
    tri_area.x = a.x + a.h/6;
    tri_area.y = a.y + trunk_h - 20;
    tri_area.h = a.h * 2/3;     /* 少し小さく */
    tri_area.w = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);

    /* 上の段 */
    tri_area.x = a.x + a.h/4;
    tri_area.y = a.y;           /* 一番上 */
    tri_area.h = a.h / 2;       /* もっと小さく */
    tri_area.w = tri_h;
    drawIsocelesTriangle(c, tri_area, leaf_col);
}

/* --- ゴールセットを描画する関数 --- */
void drawGoal(CANVAS c, struct atlas a, int t) {
    struct color black = {0, 0, 0};          
    struct color pole_col = {192, 192, 192}; 
    struct color flag_col = {255, 0, 0};     

    /* 1. 穴を描く (黒い楕円) */
    struct atlas hole_area;
    hole_area.x = a.x + a.h/4;
    hole_area.y = a.y + a.w - 8;
    hole_area.h = a.h / 2;
    hole_area.w = 8;
    drawEllipse(c, hole_area, black);

    /* 2. 棒を描く (長方形) */
    struct atlas pole_area;
    pole_area.x = a.x + a.h/2 - 2; /* 中央 */
    pole_area.y = a.y + 20;        /* 旗の分あける */
    pole_area.h = 4;               /* 棒の太さ */
    pole_area.w = a.w - 20;        /* 長さ */
    drawRectangle(c, pole_area, pole_col);

    /* 3. 旗を描く (波) */
    /* 棒のてっぺんから右へ */
    int wave_x = a.x + a.h/2;
    int wave_y = a.y + 20;
    int length = 35; /* 旗の長さ */
    int amp = 4;     /* 波の高さ */
    int omega = 1;   /* 波の数 */
    
    /* 旗を塗りつぶすために、少しずつYをずらして複数回線を引く */
    for(int i = 0; i < 20; i++) {
        /* theta(位相)に t を渡して動かす */
        drawWave(c, wave_x, wave_y + i, length, amp, omega, t); 
    }
}

/* --- つぶれたボールを描画する関数 --- */
void drawSquashedBall(CANVAS c, int x, int y) {
    struct color white = {255, 255, 255};
    struct atlas ball_area;
    
    /* 通常半径10 -> つぶれて横13, 縦7の楕円とする */
    /* 中心(x,y)から左上の座標を計算 */
    ball_area.x = x - 13;
    ball_area.y = y - 7;
    ball_area.h = 26; /* 横幅 (半径13*2) */
    ball_area.w = 14; /* 縦幅 (半径7*2) */
    
    drawEllipse(c, ball_area, white);
}
