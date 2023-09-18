// ============================================
// The header file for Graphics class file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#ifndef GRAPHICS_H
#define GRAPHICS_H

/**
 * Takes a string image and draws it to the screen. The string is 121 characters
 * long, and represents an 11x11 tile in row-major ordering (across, then down,
 * like a regular multi-dimensional array). The available colors are:
 *      R = Red
 *      Y = Yellow
 *      G = Green
 *      D = Brown ("dirt")
 *      5 = Light grey (50%)
 *      3 = Dark grey (30%)
 *      B = Blue
 *      Any other character is black
 * More colors can be easily added by following the pattern already given.
 */
void draw_img(int u, int v, const char* img);

/**
 * DrawFunc functions. 
 * These can be used as the MapItem draw functions.
 */
void draw_nothing(int u, int v);
void draw_plant(int u, int v);
void draw_wall(int u, int v);
void draw_chest(int u, int v);
void draw_viper_body(int u, int v);
void draw_viper_head(int u, int v);
void draw_viper_tail(int u, int v);
void draw_increase_speed(int u, int v); // lightning bolt
void draw_life_off(int u, int v); // warning sign
void draw_stop_game_for_five(int u, int v); // exclamation mark
void draw_decrease_speed(int u, int v); // turtle
void draw_increase_length_only(int u, int v); // briefcase
void draw_increase_score_only(int u, int v); // arrow up
void draw_increase_life(int u, int v); // heart
void draw_doubles_score(int u, int v); // diagonal arrow

#endif // GRAPHICS_H