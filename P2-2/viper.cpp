// ===========================================
// The Viper class file
//
// Copyright 2022 Georgia Tech. All rights reserved.
// The materials provided by the instructor in this course are for
// the use of the students currently enrolled in the course.
// Copyrighted course materials may not be further disseminated.
// This file must NOT be made publicly available anywhere.
//==================================================================


#include "viper.h"

void viper_init (Viper * v)
{
    //1. Set starting coordinates for your viper head and previous
    v -> head_x = 5;
    v -> head_y = 5;
    v -> head_px = (v -> head_x - 1);
    v -> head_py = (v -> head_y);
    
    //2. Initialize all location for your maximum viper body (loop through the viper)
    

    //3. Set starting length of your viper
    v -> length = 3;

    //4. Set starting coordinates for the rest of viper body from starting length
    for (int i = 0; i < (v -> length); i++) {
        v -> locations[i].x = (v -> head_x - i);
        v -> locations[i].y = (v -> head_y);   
    }

    //5. Initialize viper status
    v -> score = 0;
    v -> life = 3;
    v -> speed = 7;

}
