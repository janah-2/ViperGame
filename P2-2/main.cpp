//=================================================================
// The main program file.
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


// PROJECT INCLUDES
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "viper.h"

#include <math.h>
#include <stdio.h>

// Important definitions


/////////////////////////////////////////////////////////////////////////////////////////

// GLOBAL VARS
Viper viper;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION PROTOTYPES
void playSound(char* wav);
int get_action(GameInputs inputs);
int update_game(int action);
void draw_upper_status();
void draw_lower_status();
void draw_border();
void draw_game(int draw_option);
void init_main_map();
int invincibility = 0; // starts off as turned off
int pause = 0;
bool displayed = false;
int difficulty = 1;

/////////////////////////////////////////////////////////////////////////////////////////

// FUNCTION DEFINITIONS
/**
 * Program entry point! This is where it all begins.
 * This function is for all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */
int main()
{
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    viper_init(&viper);
    
    // intro
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.text_bold(TEXTBOLD);
    uLCD.printf("WELCOME!");
    
    uLCD.color(WHITE);
    uLCD.locate(0,3);
    uLCD.printf("Welcome to the");
    uLCD.locate(0,4);
    uLCD.printf("Viper Game!");
    uLCD.locate(0,7);
    uLCD.printf("By: Janah");
    uLCD.locate(0,8);
    uLCD.printf("Abu Hassan.");
    draw_viper_head(30, 88);
    draw_viper_body(19, 88);
    draw_viper_tail(8, 88);
    uLCD.locate(8, 11);
    uLCD.printf("You");
    wait(5);
    
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.text_bold(TEXTBOLD);
    uLCD.printf("WELCOME!");
    
    uLCD.color(WHITE);
    uLCD.locate(0,3);
    uLCD.printf("Try and avoid \nboostdowns, and \nget as much \nboostups! \nThe game ends \nwhen you get \nto max length, or when you lose \nall your lives.");
    wait(5);
    
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    uLCD.color(RED);
    uLCD.locate(0,0);
    uLCD.text_bold(TEXTBOLD);
    uLCD.printf("WELCOME!");
    
    uLCD.color(WHITE);
    uLCD.locate(0,3);
    uLCD.printf("Button 1 -> \nInvincibility \n\nButton 2 -> \nPause Game \n\nButton 3 -> \nGame Menu \n\nButton 4 -> \nResume Game");
    wait(5);
    
    uLCD.filled_rectangle(0,0,255,255,BLACK);
    wait(0.5);
    
    while (1) {
        uLCD.color(WHITE);
        uLCD.locate(0,1);
        uLCD.printf("Use the Nav Switch to select \ndifficulty:");
        uLCD.locate(3,5);
        uLCD.color(RED);
        uLCD.printf("Up for Hard");
        uLCD.locate(3,11);
        uLCD.color(BLUE);
        uLCD.printf("Down for Easy");
        GameInputs diff = read_inputs();
        
        // set difficulty to hard
        if (diff.ns_up){
            difficulty = 3;
            uLCD.filled_rectangle(0,0,255,255,BLACK);
            break;
        }
        // set difficulty to easy
        if (diff.ns_down){
            difficulty = 1;
            uLCD.filled_rectangle(0,0,255,255,BLACK);
            break;
        }
    }
    
    // 0. Initialize the maps
    // TODO: implement maps_init() and init_main_map() function in map.cpp:
    maps_init();
    init_main_map();

    // Initialize game state
    set_active_map(0);
    viper.head_x = viper.head_y = 5;
    
    // Initial drawing
    draw_game(FULL_DRAW);
    
    // Main game loop
    while(1) {
        // Timer to measure game update speed
        Timer t;
        t.start();

        // 1. Read inputs 
        GameInputs inputs = read_inputs();
        
        // 2. Determine action (move, act, menu, etc.)
        int action = get_action(inputs);
        
        // 3. Update game
        int result = update_game(action);
        
        // 3b. Check for game over based on result
        // and if so, handle game over
        switch (result) {
            case GAME_OVER:
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                char str[1024];
                uLCD.text_height(5);
                snprintf(str,sizeof(str),"GAME OVER!");
                uLCD.text_string(str,4,1,FONT_7X8,RED);
                uLCD.text_height(1);
                snprintf(str,sizeof(str),"Final Score: %d", viper.score);
                uLCD.text_string(str,2,15,FONT_7X8,RED);
                wait(1000);
                break;
            case MAX_LENGTH_REACHED:
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.text_height(5);
                snprintf(str,sizeof(str),"CONGRATULATIONS!");
                uLCD.text_string(str,1,1,FONT_7X8,GREEN);
                uLCD.text_height(1);
                snprintf(str,sizeof(str),"MAX HEIGHT REACHED!");
                uLCD.text_string(str,0,4,FONT_7X8,GREEN);
                snprintf(str,sizeof(str),"Final Score: %d", viper.score);
                uLCD.text_string(str,2,15,FONT_7X8,BLUE);
                wait(1000);
                break;
            case PAUSE_GAME:
                while (pause) {
                    snprintf(str,sizeof(str),"GAME PAUSED");
                    uLCD.text_string(str,3,5,FONT_7X8,BLUE);
                    GameInputs input = read_inputs();
                    if (!input.b4) {
                        pause = 0;
                        break;
                    }
                }
                break;
            case MENU_BUTTON:
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.color(RED);
                uLCD.locate(0,0);
                uLCD.text_bold(TEXTBOLD);
                uLCD.printf("GAME MENU");
                
                uLCD.color(WHITE);
                uLCD.locate(0,3);
                uLCD.printf("Welcome to the");
                uLCD.locate(0,4);
                uLCD.printf("game menu!");
                uLCD.locate(0, 6);
                uLCD.printf("This menu will");
                uLCD.locate(0,7);
                uLCD.printf("help you play");
                uLCD.locate(0,8);
                uLCD.printf("the game.");
                draw_viper_head(30, 88);
                draw_viper_body(19, 88);
                draw_viper_tail(8, 88);
                uLCD.locate(8, 11);
                uLCD.printf("You");
                wait(5);
                
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.color(RED);
                uLCD.locate(0,0);
                uLCD.text_bold(TEXTBOLD);
                uLCD.printf("GAME MENU");
                
                uLCD.color(WHITE);
                uLCD.locate(0,3);
                uLCD.printf("The game ends when");
                uLCD.locate(0,4);
                uLCD.printf("your lives get to");
                uLCD.locate(0, 5);
                uLCD.printf("0, or you get to");
                uLCD.locate(0,6);
                uLCD.printf("max length of 50");
                uLCD.locate(0,8);
                uLCD.printf("You lose a life if you back up onto yourself or hit a:");
                draw_plant(12,100);
                uLCD.locate(4, 13);
                uLCD.printf("Plant");
                draw_wall(70,100);
                uLCD.locate(12, 13);
                uLCD.printf("Wall");
                wait(7);
                
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.color(RED);
                uLCD.locate(0,0);
                uLCD.text_bold(TEXTBOLD);
                uLCD.printf("GAME MENU");
                
                draw_chest(8,25);
                draw_stop_game_for_five(8, 55);
                draw_increase_speed(8,85);
                draw_decrease_speed(8, 105);

                uLCD.color(WHITE);
                uLCD.locate(4,3);
                uLCD.printf("Length, Score \n     +1");
                uLCD.locate(4,7);
                uLCD.printf("Stops game for \n    5 seconds");
                uLCD.locate(4,11);
                uLCD.printf("Speed +1");
                uLCD.locate(4,13);
                uLCD.printf("Speed -1");
                wait(5);
                
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.color(RED);
                uLCD.locate(0,0);
                uLCD.text_bold(TEXTBOLD);
                uLCD.printf("GAME MENU");
                
                draw_increase_length_only(8,25);
                draw_increase_score_only(8,45);
                draw_increase_life(8,65);
                draw_life_off(8,85);
                draw_doubles_score(8, 105);
                
                uLCD.color(WHITE);
                uLCD.locate(4,3);
                uLCD.printf("Length +1");
                uLCD.locate(4,6);
                uLCD.printf("Score +1");
                uLCD.locate(4,8);
                uLCD.printf("Lives +1");
                uLCD.locate(4,11);
                uLCD.printf("Lives -1");
                uLCD.locate(4,13);
                uLCD.printf("Score x2");
                wait(5);
                
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                uLCD.color(RED);
                uLCD.locate(0,0);
                uLCD.text_bold(TEXTBOLD);
                uLCD.printf("GAME MENU");
                
                uLCD.color(WHITE);
                uLCD.locate(0,3);
                uLCD.printf("Button 1 -> \nInvincibility \n\nButton 2 -> \nPause Game \n\nButton 3 -> \nGame Menu \n\nButton 4 -> \nResume Game");
                wait(5);
                
                uLCD.filled_rectangle(0,0,255,255,BLACK);
                draw_upper_status();
                draw_lower_status();
                uLCD.locate(0,2);
                uLCD.printf("Upper bar shows"); 
                uLCD.locate(0, 3);
                uLCD.printf("current position");
                uLCD.locate(0, 4);
                uLCD.printf("and score."); 
                uLCD.locate(0,10);
                uLCD.printf("Lower bar shows");
                uLCD.locate(0, 11);
                uLCD.printf("current number of"); 
                uLCD.locate(0,12);
                uLCD.printf("lives and speed.");
                wait(5);
                
                draw_border();
                break;
            default:
                break;
        }
                
        // 4. Draw screen -- provided.
        draw_game(result);
        
        // Compute update time
        t.stop();
        int dt = t.read_ms();

        // Display and wait
        // NOTE: Text is 8 pixels tall
        
        // if speed increases, viper becomes faster (waits 100ms * speed less)
        if ((viper.speed * 100) <= 1000) {
            wait_ms(1000 - (viper.speed * 100));
        }
    }
}

/**
 * Plays a .wav file
 */
void playSound(char* wav)
{
    // OPTIONAL: Implement
    
}

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possible return values are defined below.
 * Get Actions from User (pushbuttons, and nav_switch)
 * Based on push button and navigation switch inputs, determine which action
 * needs to be performed (may be no action).
 */
int get_action(GameInputs inputs)
{
    
    // 1. Check your action and menu button inputs and return the corresponding action value
    if (!inputs.b1) {
        invincibility = !invincibility;   // turn on/off if button 1 is pressed
        if (invincibility) {
            char str[1024];
            snprintf(str,sizeof(str),"INVINCIBLE!");
            uLCD.text_string(str,3,5,FONT_7X8,BLUE);
            wait(0.5);
        } else {
            char str[1024];
            snprintf(str,sizeof(str),"NOT INVINCIBLE!");
            uLCD.text_string(str,1,5,FONT_7X8,BLUE);
            wait(0.5);
        }
    }
    if (!inputs.b2) {
        pause = 1;
        return PAUSE_GAME;   
    }
    if (!inputs.b3) {
        return MENU_BUTTON;   
    }
    
    // 2. Check for your navigation switch inputs and return the corresponding action value
    if (inputs.ns_up){
        return GO_UP;
        }
    if (inputs.ns_down){
        return GO_DOWN;
        }
    if (inputs.ns_left){
        return GO_LEFT;
        }
    if (inputs.ns_right){
        return GO_RIGHT;
        }
        
    // If length get to max length, game over
    if (viper.length >= 50) {
        return MAX_LENGTH_REACHED;   
    }
    
    // if lives get to 0, return game over
    if (viper.life <= 0){
        return GAME_OVER;   
    }
    
    // If no button is pressed, just return no action value
    return NO_ACTION;
}

/**
 * This function is called by update game to check when the snake 
 * gets an object (in other words, when a snake eats an object). We 
 * update the snake and game conditions as needed based on the object type
 */
int get_object(){
    
    // 1. Get item at current head location
    MapItem *item = get_current(viper.head_x, viper.head_y);
    
    // 2. Initialize a return value variable to denote the item type we got
    int itemType = 0;
    
    // 3. Check if item is valid and is a chest
    if (item -> type == CHEST) {
        // 3a. if so, increment the score, extend the length of the viper
        viper.score += 1;
        viper.length += 1;
                
        // 3b. Remove the chest
        remove_chest(viper.head_x, viper.head_y);
        
        // 3c. set the return value variable to GOT_LENGTH
        itemType = GOT_LENGTH;
    }   
        
    // 4. Else, check if the item is valid a boost-up/boost-down
    //    4a. if so, set the ability for the viper based on the type of item
    //    4b. Remove the boost-up/down item
    //    4c. Set the return value variable to GOT_OBJ
    switch(item -> type) {
        case INCREASESPEED:
            // increase speed if speed < 10
            if (viper.speed < 10) {
                viper.speed += 1;
            } else {
                // let player know that max speed has been reached
                // if speed is at 10, and user has not been informed already
                if ((viper.speed == 10) && (!displayed)) {
                    displayed = true;
                    char str[1024];
                    snprintf(str,sizeof(str),"Max speed ");
                    uLCD.text_string(str,3,5,FONT_7X8,BLUE);
                    snprintf(str,sizeof(str),"reached!");
                    uLCD.text_string(str,3,6,FONT_7X8,BLUE);
                    wait(1);
                    draw_border();
                }
            }
            remove_increase_speed(viper.head_x, viper.head_y);
            itemType = GOT_OBJ;
            break;
        case LIFEOFF:
            if (invincibility) {
                // do nothing   
            } else {
                viper.life -= 1;
                remove_life_off(viper.head_x, viper.head_y);
                itemType = GOT_OBJ;
            }
            break;
        case DECREASESPEED:
            if (invincibility) {
                // do nothing   
            } else {
                // decrease speed if speed is > 1
                if (viper.speed > 1) {
                    viper.speed -= 1;
                }
                remove_decrease_speed(viper.head_x, viper.head_y);
                itemType = GOT_OBJ;
            }
            break;
        case STOPGAMEFORFIVE:
            if (invincibility) {
                    // do nothing   
            } else {
                char str[1024];
                snprintf(str,sizeof(str),"OH OH!");
                uLCD.text_string(str,0,5,FONT_7X8,BLUE);
                snprintf(str,sizeof(str),"GAME STOPPED");
                uLCD.text_string(str,0,6,FONT_7X8,BLUE);
                snprintf(str,sizeof(str),"FOR 5 SECONDS.");
                uLCD.text_string(str,0,7,FONT_7X8,BLUE);
                wait(5);
                remove_stop_game_for_five(viper.head_x, viper.head_y);
                itemType = GOT_OBJ;
                draw_border();
            }
            break;
        case INCREASELENGTHONLY:
            viper.length += 1;
            remove_increase_length_only(viper.head_x, viper.head_y);
            itemType = GOT_OBJ;
            break;
        case INCREASESCOREONLY:
            viper.score += 1;
            remove_increase_score_only(viper.head_x, viper.head_y);
            itemType = GOT_OBJ;
            break;
        case INCREASELIFE:
            viper.life += 1;
            remove_increase_life(viper.head_x, viper.head_y);
            itemType = GOT_OBJ;
            break;
        case DOUBLESSCORE:
            viper.score *= 2;
            remove_doubles_score(viper.head_x, viper.head_y);
            itemType = GOT_OBJ;
            break;
    }
    
    // Get new locations
    for (int i = viper.length - 1; i > 0; i--) {
        viper.locations[i] = viper.locations[i - 1];   
    }
    
    // set location[0] to location of head
    viper.locations[0].y = viper.head_y; 
    viper.locations[0].x = viper.head_x; 
    
    // update positions of head and tail
    add_viper_head(viper.locations[0].x, viper.locations[0].y); // add head sprite
    add_viper_body(viper.locations[1].x, viper.locations[1].y); // overwrite old head with body
    for (int i = 2; i < viper.length - 1; i++){
        add_viper_body(viper.locations[i].x, viper.locations[i].y);
    }
    add_viper_tail(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);

}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the viper position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the viper has not moved.
 */
 
 
int update_game(int action)
{
    
    // Update the previous viper head position to the current position
    viper.head_px = viper.head_x;
    viper.head_py = viper.head_y;
    
    switch(action) {
        case GO_UP:
             // get item north of viper head
            MapItem *itemNorth = get_north(viper.head_px, viper.head_py);
            
            // if walkable
            if (itemNorth -> walkable) {
                // update head location
                viper.head_y = viper.head_y - 1;
                remove_viper_body(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
                
                // check what object it is
                get_object();
                
                // return game over if viper is at max length
                if (viper.length >= 50) {
                    return MAX_LENGTH_REACHED;  
                }
            } else {
                // if not walkable, reduce lives and return game over if lives == 0
                if (invincibility) {
                    // do nothing
                } else {
                    viper.life -= 1;
                    if (viper.life == 0) {
                        return GAME_OVER;   
                    }
                }
            }
            return GO_UP;
            break;
        case GO_DOWN:
             // get item south of viper head
            MapItem *itemSouth = get_south(viper.head_px, viper.head_py);
            
            // if walkable
            if (itemSouth -> walkable) {
                // update head location
                viper.head_y = viper.head_y + 1;
                remove_viper_body(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
                
                // check what object it is
                get_object();
                
                // return game over if viper is at max length
                if (viper.length >= 50) {
                    return MAX_LENGTH_REACHED;  
                }
            } else {
                // if not walkable, reduce lives and return game over if lives == 0
                if (invincibility) {
                    //do nothing
                } else {
                    viper.life -= 1;
                    if (viper.life == 0) {
                        return GAME_OVER;   
                    }
                } 
            }
            return GO_DOWN;
            break;
        case GO_LEFT:
            // get item west of viper head
            MapItem *itemWest = get_west(viper.head_px, viper.head_py);
            
            // if walkable
            if (itemWest -> walkable) {
                // update head location
                viper.head_x = viper.head_x - 1;
                remove_viper_body(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);
              
                // check what object it is
                get_object();
                
                // return game over if viper is at max length
                if (viper.length >= 50) {
                    return MAX_LENGTH_REACHED;  
                }
            } else {
                // if not walkable, reduce lives and return game over if lives == 0
                if (invincibility) {
                    // do nothing
                } else {
                    viper.life -= 1;
                    if (viper.life == 0) {
                        return GAME_OVER;   
                    }
                }   
            }
            return GO_LEFT;
            break;
        case GO_RIGHT:
            // get item east of viper head
            MapItem *itemEast = get_east(viper.head_px, viper.head_py);
            
            // if walkable
            if (itemEast -> walkable) {
                // update head location
                viper.head_x = viper.head_x + 1;
                remove_viper_body(viper.locations[viper.length - 1].x, viper.locations[viper.length - 1].y);           

                // check what object it is
                get_object();
                
                // return game over if viper is at max length
                if (viper.length >= 50) {
                    return MAX_LENGTH_REACHED;  
                }
            } else {
                // if not walkable, reduce lives and return game over if lives == 0
                if (invincibility) {
                    // do nothing
                } else {
                    viper.life -= 1;
                    if (viper.life == 0) {
                        return GAME_OVER;   
                    }
                }
            }
            return GO_RIGHT;
            break;    
        case GAME_OVER:
            return GAME_OVER;
        case PAUSE_GAME:
            return PAUSE_GAME;
        case MENU_BUTTON:
            return MENU_BUTTON;
        case MAX_LENGTH_REACHED:
            return MAX_LENGTH_REACHED;
        default:
            break;
    }    
    
    // If no action was given, then we simply return no result
    return NO_RESULT;
}


/**
 * Draw the upper status bar.
 */
void draw_upper_status()
{
    char str[1024];
    snprintf(str,sizeof(str),"Pos:%d,%d   ",viper.head_x,viper.head_y);
    uLCD.text_string(str,0,0,FONT_7X8,RED);
    snprintf(str,sizeof(str),"Score:%d   ",viper.score);
    uLCD.text_string(str,10,0,FONT_7X8,RED);
}

/**
 * Draw the lower status bar.
 */
void draw_lower_status()
{
    char str[1024];
    snprintf(str,sizeof(str),"Lives:%d  ",viper.life);
    uLCD.text_string(str,0,15,FONT_7X8,RED);
    snprintf(str,sizeof(str),"Speed:%d  ",viper.speed);
    uLCD.text_string(str,10,15,FONT_7X8,RED);
}

/**
 * Draw the border for the map.
 */
void draw_border()
{
    uLCD.filled_rectangle(  0,   9, 127,  14, WHITE);   // Top
    uLCD.filled_rectangle(  0,  13,   2, 114, WHITE);   // Left
    uLCD.filled_rectangle(  0, 114, 127, 117, WHITE);   // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE);   // Right
}

/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status
 * bars. Unless init is nonzero, this function will optimize drawing by only
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int draw_option)
{
    // Draw game border first
    if(draw_option == FULL_DRAW) 
    {
        draw_border();
        int u = 58;
        int v = 56;
        draw_viper_head(u, v);
        draw_viper_body(u-11, v);
        draw_viper_tail(u-22, v);
        return;
    }

    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) { // Iterate over columns of tiles
        for (int j = -4; j <= 4; j++) { // Iterate over one column of tiles
            // Here, we have a given (i,j)

            // Compute the current map (x,y) of this tile
            int x = i + viper.head_x;
            int y = j + viper.head_y;

            // Compute the previous map (px, py) of this tile
            int px = i + viper.head_px;
            int py = j + viper.head_py;

            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;

            // Figure out what to draw
            DrawFunc draw = NULL;
            if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) { // Current (i,j) in the map
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (draw_option || curr_item != prev_item) { // Only draw if they're different
                    if (curr_item) { // There's something here! Draw it
                        draw = curr_item->draw;
                    } else { // There used to be something, but now there isn't
                        draw = draw_nothing;
                    }
                } else if (curr_item && curr_item->type == CLEAR) {
                    // This is a special case for erasing things like doors.
                    draw = curr_item->draw; // i.e. draw_nothing
                }
            } else if (draw_option) { // If doing a full draw, but we're out of bounds, draw the walls.
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars
    draw_upper_status();
    draw_lower_status();
}

/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += (39 * difficulty)) {
        add_chest(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    
    for(int i = map_width() + 3; i < map_area(); i += 20) {
        add_plant(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += (65 * difficulty)) {
        add_increase_speed(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += 55) {
        add_life_off(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += 75) {
        add_decrease_speed(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += 78) {
        add_stop_game_for_five(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += (89 * difficulty)) {
        add_increase_length_only(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += (99 * difficulty)) {
        add_increase_score_only(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += (109 * difficulty)) {
        add_increase_life(i % map_width(), i / map_width());
    }
    
    for(int i = map_width() + 3; i < map_area(); i += (80 * difficulty)) {
        add_doubles_score(i % map_width(), i / map_width());
    }
    
    pc.printf("Adding walls!\r\n");
    add_wall(            0,              0, HORIZONTAL, map_width());
    add_wall(            0, map_height()-1, HORIZONTAL, map_width());
    add_wall(            0,              0,   VERTICAL, map_height());
    add_wall(map_width()-1,              0,   VERTICAL, map_height());
    pc.printf("Walls done!\r\n");
    
    add_viper_head(viper.locations[0].x, viper.locations[0].y);
    add_viper_body(viper.locations[1].x, viper.locations[1].y);
    add_viper_tail(viper.locations[2].x, viper.locations[2].y);
    
    pc.printf("Add extra chamber\r\n");
    add_wall(30,  0,   VERTICAL, 10);
    add_wall(30, 10, HORIZONTAL, 10);
    add_wall(39,  0,   VERTICAL, 10);
    pc.printf("Added!\r\n");
    
    print_map();
}