// ============================================
// The Graphics class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "graphics.h"

#include "globals.h"

void draw_nothing(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'Y') colors[i] = 0xFFFF00;   // Yellow
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'D') colors[i] = 0xD2691E;   // "Dirt"
        else if (img[i] == '5') colors[i] = LGREY;      // 50% grey
        else if (img[i] == '3') colors[i] = DGREY;
        else if (img[i] == 'B') colors[i] = 0x00FFFF;   // Blue
        else if (img[i] == 'L') colors[i] = 0xF5B536;   // Gold
        else if (img[i] == 'K') colors[i] = BLACK;
        else if (img[i] == '6') colors[i] = 0xAA4A44;   // Brick-red
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_wall(int u, int v)
{
    const char* wall = "666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666666";
    draw_img(u, v, wall);
}

void draw_plant(int u, int v)
{
    const char* plant = "KKKKGGGKKKKKKGGGGGGGKKKGGGGGGGGGKGGGGGGGGGGGGGGGGGGGGGGKGGDGGGDGGKKKKGDDDGKKKKKKKDDDKKKKKKKKDDDKKKKKKKKDDDKKKKKKKDDDDDKKK";
    draw_img(u, v, plant);
}

void draw_chest(int u, int v)
{
    const char* chest ="KKLLLLLLLKKKLLDDDDDLLKLLDDDDDDDLLLDDDDLDDDDLLDDDLLLDDDLLLLLLLLLLLLLDDDGKGDDDLLDDDLLLDDDLLDDDDDDDDDLLDDDDDDDDDLLLLLLLLLLLL";
    draw_img(u, v, chest);
}

void draw_viper_body(int u, int v)
{
    const char* viper_body = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    draw_img(u, v, viper_body);
}

void draw_viper_head(int u, int v)
{
    const char* viper_head = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBRRRBBBBBBBBRRRBBBBBBBBRRRBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    draw_img(u, v, viper_head);
}

void draw_viper_tail(int u, int v)
{
    const char* viper_tail = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBYYYBBBBBBBBYYYBBBBBBBBYYYBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    draw_img(u, v, viper_tail);
}

void draw_increase_speed(int u, int v) // speed up: lightning bolt (boost up)
{
    const char* increase_speed ="KKKYYYYYKKKKKYYYYYKKKKKKYYYYKKKKKKYYYYYYYYKKKYYYYYYYYKKYYYYYYYYKKKYYYYYYYKKKKKKYYYYKKKKKKYYYYKKKKKKYYYYKKKKKKKYYYKKKKKKKK";
    draw_img(u, v, increase_speed);   
}

void draw_decrease_speed(int u, int v) // speed down: turtle (boost down)
{
    const char* decrease_speed ="KKKKKKKKKKKKKKKKKKKKKKKKKKGGKKKKKKKKGGGGK66KKKGGGGGG6K6KKGGGGGG666K6GGGGGG66KKK66KK66KKKKK66KK66KKKKKKKKKKKKKKKKKKKKKKKKK";
    draw_img(u, v, decrease_speed);   
}

void draw_life_off(int u, int v) // life -1: warning sign (boost down)
{
    const char* life_off = "KKKKKYKKKKKKKKKYYYKKKKKKKYYKYYKKKKKKYYKYYKKKKKYYYKYYYKKKKYYYKYYYKKKYYYYKYYYYKYYYYYYYYYYYYYYYYKYYYYYYYYYYYYYYYYKKKKKKKKKKK";
    draw_img(u, v, life_off);
}

void draw_stop_game_for_five(int u, int v) // stops game for 5 seconds: exclamation mark (boost down)
{
    const char* stop_game_for_five  = "KKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKKKKKKKKKKKKRRRKKKKKKKKRRRKKKKKKKKRRRKKKK";
    draw_img(u, v, stop_game_for_five);
}

void draw_increase_length_only(int u, int v) // length +1: briefcase (boost up)
{
    const char* increase_length_only  = "KKKKKKKKKKKKDDDDDDDDDKKDDDDDDDDDKKDDDDDDDDDKKDDDDKDDDDKKKKKKKKKKKKKDDDDKDDDDKKDDDDDDDDDKKDDDDDDDDDKKDDDDDDDDDKKKKKKKKKKKK";
    draw_img(u, v, increase_length_only);
}

void draw_increase_score_only(int u, int v) // score +1: green arrow up (boost up)
{
    const char* increase_score_only  = "KKKKKGKKKKKKKKKDDDKKKKKKGGGGGGGKKKGGGGGGGGGKGGGGGGGGGGGKKKKGGGKKKKKKKKGGGKKKKKKKKGGGKKKKKKKKGGGKKKKKKKKGGGKKKKKKKKGGGKKKK";
    draw_img(u, v, increase_score_only);
}

void draw_increase_life(int u, int v) // life +1: heart (boost up)
{
    const char* increase_life  = "KKRRKKKRRKKKRRRRKRRRRKKRRRRRRRRRKRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRKRRRRRRRRRKKKRRRRRRRKKKKKRRRRRKKKKKKKRRRKKKKKKKKKRKKKKK";
    draw_img(u, v, increase_life);
}

void draw_doubles_score(int u, int v) // score x 2: diagonal arrow (boost up)
{
    const char* doubles_score  = "KKKKKBBBBBBKKKKKKKKKBBKKKKKKKKBKBKKKKKKKBKKBKKKKKKBKKKBKKKKKBKKKKBKKKKBKKKKKKKKKBKKKKKKKKKBKKKKKKKKKBKKKKKKKKKBKKKKKKKKKK";
    draw_img(u, v, doubles_score);
}

