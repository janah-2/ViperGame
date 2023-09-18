// ============================================
// The Map class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "map.h"

#include "globals.h"
#include "graphics.h"

#define MHF_NBUCKETS 97

/**
 * The Map structure. This holds a HashTable for all the MapItems, along with
 * values for the width and height of the Map.
 */
struct Map {
    HashTable* items;
    int w, h;
};

#define NUM_MAPS 1

static Map maps[NUM_MAPS];
static int active_map;

static const MapItem CLEAR_SENTINEL = {
    .type = CLEAR,
    .draw = draw_nothing
};

/**
 * The first step in HashTable access for the map is turning the two-dimensional
 * key information (x, y) into a one-dimensional unsigned integer.
 * This function should uniquely map (x,y) onto the space of unsigned integers.
 */
static unsigned XY_KEY(int X, int Y) {
    // 1. Return the 1-D xy key
    unsigned xy_key = X + map_width() * Y;
    return xy_key;
}

/**
 * This is the hash function actually passed into createHashTable. It takes an
 * unsigned key (the output of XY_KEY) and turns it into a hash value (some
 * small non-negative integer).
 */
unsigned map_hash(unsigned key)
{
    return key % MHF_NBUCKETS;
}

/**
 * Initializes the map, using a hash_table, setting the width and height.
 */
void maps_init()
{
    // TODO: Implement
    // 1. Loop through all map items and and initialize hash table to hold the buckets
    // 2. Set width & height for any maps 
    // 3. Set the first map to be active
    Map *activeMap = get_active_map();
    HashTable *activeMapHashTable = createHashTable(map_hash, MHF_NBUCKETS);
    activeMap -> w = WIDTH;
    activeMap -> h = HEIGHT;
    activeMap -> items = activeMapHashTable;
}

Map* get_active_map()
{
    return &maps[active_map];
}

Map* set_active_map(int m)
{
    active_map = m;
    return &maps[active_map];
}

void print_map()
{
    char lookup[] = {'W', 'D', 'P', 'A', 'K', 'C', 'N',' ','S'};
    Map* map = get_active_map();
    for(int j = 0; j < map->h; j++)
    {
        for (int i = 0; i < map->w; i++)
        {
            MapItem* item = (MapItem*)getItem(map->items, XY_KEY(i, j));
            if (item) pc.printf("%c", lookup[item->type]);
            else pc.printf(" ");
        }
        pc.printf("\r\n");
    }
}

/**
 * Returns width of active map
 */
int map_width()
{
    Map *activeMap = get_active_map();
    return activeMap -> w;
}

/**
 * Returns height of active map
 */
int map_height()
{
    Map *activeMap = get_active_map();
    return activeMap -> h;
}

/**
 * Returns the area of the active map
 */
int map_area()
{
    return map_height() * map_width();
}

/**
 * Returns MapItem at current coordinate location
 */
MapItem* get_current(int x, int y)
{
    // 1. Get map item
    Map *activeMap = get_active_map();
    int key = XY_KEY(x, y);
    MapItem *item = (MapItem*) getItem(activeMap -> items, key);
    // 2. Check and remove item if item exist and is clear type
    if (item -> type == CLEAR) {
        deleteItem(activeMap -> items, key);
    }
    // 3. Return the item
    return item;
}

/**
 * Returns the MapItem immediately above the given location.
 */
MapItem* get_north(int x, int y)
{
    return get_current(x, y - 1);
}

/**
 * Returns the MapItem immediately below the given location.
 */
MapItem* get_south(int x, int y)
{
    return get_current(x, y + 1);
}

/**
 * Returns the MapItem immediately right the given location.
 */
MapItem* get_east(int x, int y)
{
    return get_current(x + 1, y);
}

/**
 * Returns the MapItem immediately left the given location.
 */
MapItem* get_west(int x, int y)
{
    return get_current(x - 1, y);
}

/**
 * Returns the MapItem at current coordinate location
 */
 MapItem* get_here(int x, int y)
 {
     // 1. Get map item
    Map *activeMap = get_active_map();
    int key = XY_KEY(x, y);
    MapItem *item = (MapItem*) getItem(activeMap -> items, key);
    // 2. Check and remove item if item exist and is clear type
    if (item -> type == CLEAR) {
        deleteItem(activeMap -> items, key);
    }
    // 3. Return the item
    return item;
 }

/**
 * Erase current location map item
 */
void map_erase(int x, int y)
{
    //1. Insert clear value to the map item at current location
    Map *activeMap = get_active_map();
    int key = XY_KEY(x, y);
    MapItem *item = (MapItem*) getItem(activeMap -> items, key);
    //2. Free current location map item if it exists
    if (item != NULL) {
        *item = CLEAR_SENTINEL;
        free(item);
    }
}

void add_wall(int x, int y, int dir, int len)
{
    for(int i = 0; i < len; i++)
    {
        MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
        w1->type = WALL;
        w1->draw = draw_wall;
        w1->walkable = false;
        w1->data = NULL;
        unsigned key = (dir == HORIZONTAL) ? XY_KEY(x+i, y) : XY_KEY(x, y+i);
        void* val = insertItem(get_active_map()->items, key, w1);
        if (val) free(val); // If something is already there, free it
    }
}

void add_plant(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = PLANT;
    w1->draw = draw_plant;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_chest(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CHEST;
    w1->draw = draw_chest;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_chest(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = CLEAR;
    w1->draw = draw_nothing;
    w1->walkable = true;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_body(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_body;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_viper_body(int x, int y)
{
    MapItem* s1 = (MapItem*) malloc(sizeof(MapItem));
    s1->type = CLEAR;
    s1->draw = draw_nothing;
    s1->walkable = true;
    s1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), s1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_head(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_head;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_viper_head(int x, int y)
{
    MapItem* s1 = (MapItem*) malloc(sizeof(MapItem));
    s1->type = CLEAR;
    s1->draw = draw_nothing;
    s1->walkable = true;
    s1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), s1);
    if (val) free(val); // If something is already there, free it
}

void add_viper_tail(int x, int y)
{
    MapItem* w1 = (MapItem*) malloc(sizeof(MapItem));
    w1->type = VIPER_BODY;
    w1->draw = draw_viper_tail;
    w1->walkable = false;
    w1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), w1);
    if (val) free(val); // If something is already there, free it
}

void remove_viper_tail(int x, int y)
{
    MapItem* s1 = (MapItem*) malloc(sizeof(MapItem));
    s1->type = CLEAR;
    s1->draw = draw_nothing;
    s1->walkable = true;
    s1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), s1);
    if (val) free(val); // If something is already there, free it
}

void add_increase_speed(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = INCREASESPEED;
    b1->draw = draw_increase_speed;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_increase_speed(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_life_off(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = LIFEOFF;
    b1->draw = draw_life_off;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_life_off(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_decrease_speed(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = DECREASESPEED;
    b1->draw = draw_decrease_speed;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_decrease_speed(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_stop_game_for_five(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = STOPGAMEFORFIVE;
    b1->draw = draw_stop_game_for_five;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_stop_game_for_five(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_increase_length_only(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = INCREASELENGTHONLY;
    b1->draw = draw_increase_length_only;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_increase_length_only(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_increase_score_only(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = INCREASESCOREONLY;
    b1->draw = draw_increase_score_only;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_increase_score_only(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_increase_life(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = INCREASELIFE;
    b1->draw = draw_increase_life;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_increase_life(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void add_doubles_score(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = DOUBLESSCORE;
    b1->draw = draw_doubles_score;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}

void remove_doubles_score(int x, int y)
{
    MapItem* b1 = (MapItem*) malloc(sizeof(MapItem));
    b1->type = CLEAR;
    b1->draw = draw_nothing;
    b1->walkable = true;
    b1->data = NULL;
    void* val = insertItem(get_active_map()->items, XY_KEY(x, y), b1);
    if (val) free(val); // If something is already there, free it
}
