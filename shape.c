#include "shape.h"
#include <math.h>

/* ---------------------------------------------------------
   shape.c
   shape.h で宣言された関数の「実体（中身）」を書くファイル
   --------------------------------------------------------- */

/* 点を打つ関数 */
void drawPixel(CANVAS c, int x, int y, struct color clr) {
    if (x < 0 || x >= CANVAS_WIDTH || y < 0 || y >= CANVAS_HEIGHT) {
        return;
    }
    /* CANVASは struct color** なので、配列のようにアクセスできる */
    c[x][y] = clr;
}

/* 長方形を描く関数 */
void drawRectangle(CANVAS c, struct atlas a, struct color clr) {
    /* h=横幅, w=高さ としてループを回す */
    for(int j = 0; j < a.w; j++) {
        for(int i = 0; i < a.h; i++) {
            drawPixel(c, a.x + i, a.y + j, clr);
        }
    }
}

/* 楕円を描く関数 */
void drawEllipse(CANVAS c, struct atlas a, struct color clr) {
    double rx = a.h / 2.0;
    double ry = a.w / 2.0;
    double cx = a.x + rx;
    double cy = a.y + ry;

    for(int j = 0; j < a.w; j++) {
        for(int i = 0; i < a.h; i++) {
            double dx = (a.x + i - cx);
            double dy = (a.y + j - cy);
            /* 楕円の方程式: (x^2/a^2) + (y^2/b^2) <= 1 */
            if ((dx*dx)/(rx*rx) + (dy*dy)/(ry*ry) <= 1.0) {
                drawPixel(c, a.x + i, a.y + j, clr);
            }
        }
    }
}

/* 二等辺三角形を描く関数 */
void drawIsocelesTriangle(CANVAS c, struct atlas a, struct color clr) {
    /* 底辺の中央のX座標 */
    double top_x = a.x + a.h / 2.0;
    
    for(int j = 0; j < a.w; j++) {
        for(int i = 0; i < a.h; i++) {
            int px = a.x + i;
            int py = a.y + j;
            
            /* 上に行くほど幅が狭くなる計算 */
            double progress = (double)j / a.w; 
            double current_w = progress * a.h; 
            
            if (px >= top_x - current_w/2.0 && px <= top_x + current_w/2.0) {
                drawPixel(c, px, py, clr);
            }
        }
    }
}

/* 波（旗のなびき）を描く関数 */
void drawWave(CANVAS c, int x, int y, int length, int amplitude, int omega, int theta) {
    /* theta は時間tとして扱われる */
    for(int i = 0; i < length; i++) {
        /* sin波の高さを計算 */
        double val = amplitude * sin(0.2 * i * omega + theta);
        drawPixel(c, x + i, y + (int)val, (struct color){255, 0, 0}); 
    }
}

/* 円を描く関数（楕円のラッパー） */
void drawCircle(CANVAS c, int x, int y, int radius, struct color clr) {
    struct atlas a = {x - radius, y - radius, radius * 2, radius * 2};
    drawEllipse(c, a, clr);
}