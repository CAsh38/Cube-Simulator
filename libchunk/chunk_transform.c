#include "chunk.h"
#include <stdlib.h>

#define Nr_Objects 300

void free_chunk_transform(char*** chunk, int width, int height)
{
    for(int i = width-1; i >= 0; i--)
    {
        for(int j = height-1; j >= 0; j--)
        {
            free(chunk[i][j]);
        }
        free(chunk[i]);
    }
    free(chunk);
}

char*** chunk_duplicate_air(
    int width, int height, int depth){
    
    char*** new_chunk = malloc(width * sizeof *new_chunk);
    for (int x = 0; x < width; x++) {
        new_chunk[x] = malloc(height * sizeof **new_chunk);
        for (int y = 0; y < height; y++) {
            new_chunk[x][y] = calloc(depth, sizeof ***new_chunk);
        }
    }
    return new_chunk;
}

char*** chunk_rotate_y(
    char*** chunk, int width, int height, int depth) {
    
    char*** new_chunk = malloc(depth * sizeof(*chunk));
    for(int i = 0; i < depth; i++)
    {
        new_chunk[i] = malloc(height * sizeof(**chunk));
        for(int j = 0; j < height; j++)
        {
            new_chunk[i][j] = malloc(width * sizeof(***chunk));
        }
    }
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < depth; k++)
            {
                new_chunk[depth - 1 - k][j][i] = chunk[i][j][k];
            }
        }
    }
    // char*** dup_new_chunk = chunk_duplicate(new_chunk, depth, height, width);
    // for(int j = 0; j < height; j++)
    // {
    //     for(int i = 0; i < depth; i++)
    //     {
    //         for(int k = 0; k < width; k++)
    //         {
    //             new_chunk[i][j][k] = dup_new_chunk[depth - 1][j][width - k];
    //         }
    //     }
    // }
    // free(chunk);
    free_chunk_transform(chunk, width, height);
    return new_chunk;
}

char*** find_object(char*** temp_chunk, char*** space_object,
                    char*** chunk, int width, int height, int depth,
                    int x, int y, int z, char block){
    
    temp_chunk[x][y][z] = block;
    space_object[x][y][z] = block;
    chunk[x][y][z] = BLOCK_AIR;
    if(z - 1 >= 0)
    {
        if(chunk[x][y][z-1] == block && temp_chunk[x][y][z-1] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x, y, z-1, block);
        }
    }
    if(z + 1 <= depth - 1)
    {
        if(chunk[x][y][z+1] == block && temp_chunk[x][y][z+1] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x, y, z+1, block);
        }
    }
    if(x - 1 >= 0)
    {
        if(chunk[x-1][y][z] == block && temp_chunk[x-1][y][z] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x-1, y, z, block);
        }
    }
    if(x + 1 <= width - 1)
    {
        if(chunk[x+1][y][z] == block && temp_chunk[x+1][y][z] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x+1, y, z, block);
        }
    }
    if(y - 1 >= 0)
    {
        if(chunk[x][y-1][z] == block && temp_chunk[x][y-1][z] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x, y-1, z, block);
        }
    }
    if(y + 1 <= height - 1)
    {
        if(chunk[x][y+1][z] == block && temp_chunk[x][y+1][z] != block)
        {
            temp_chunk = find_object(temp_chunk, space_object, chunk, width,
            height, depth, x, y+1, z, block);
        }
    }
    return temp_chunk;
}

char*** fall_down_1block(char*** object, char*** temp_chunk,
        int width, int height, int depth){
        
        for(int i = 0; i < width; i++)
        {
            for(int j = 0; j < height; j++)
            {
                for(int k = 0; k < depth; k++)
                {
                    if(object[i][j][k] != BLOCK_AIR)
                    {
                        temp_chunk[i][j][k] = BLOCK_AIR;
                        temp_chunk[i][j-1][k] = object[i][j][k];
                        object[i][j-1][k] = object[i][j][k];
                        object[i][j][k] = BLOCK_AIR; 
                    }
                }
            }
        }
    return temp_chunk;
}

char**** check_for_joined_objects(char**** mother_of_chunks, int* object_counter,
    int width, int height, int depth) {
    
    int original = *object_counter;
    for(int s = 0; s < *object_counter; s++)
    {
        for(int p = s+1; p < *object_counter; p++)
        {
            int joined = 0;
            for(int i = 0; i < width; i++)
            {
                for(int k = 0; k < depth; k++)
                {
                    unsigned char stick[height];
                    for(int t = 0; t < height; t++) stick[t] = BLOCK_AIR;
                    for(int j = 0; j < height; j++)
                    {
                        
                        if(mother_of_chunks[s][i][j][k] != BLOCK_AIR)
                        {
                            stick[j] = mother_of_chunks[s][i][j][k];
                        }
                        else if(mother_of_chunks[p][i][j][k] != BLOCK_AIR)
                        {
                            stick[j] = mother_of_chunks[p][i][j][k];
                        }
                    }
                    int joined_sides = 0;
                    for(int j = 1; j < height; j++)
                    {
                        if(stick[j] != BLOCK_AIR)
                        {
                            if(stick[j] != stick[j-1]) joined_sides++;
                            if(joined_sides >= 3) joined = 1;
                        }
                        else joined_sides = 0;
                    }
                }
            }
            if(joined)
            {
                char*** merged_chunk = chunk_duplicate_air(width, height, depth);
                for(int i = 0; i < width; i++)
                {
                    for(int j = 0; j < height; j++)
                    {
                        for(int k = 0; k < depth; k++)
                        {
                            if(mother_of_chunks[s][i][j][k] != BLOCK_AIR)
                            {
                                merged_chunk[i][j][k] = mother_of_chunks[s][i][j][k];
                            }
                            if(mother_of_chunks[p][i][j][k] != BLOCK_AIR)
                            {
                                merged_chunk[i][j][k] = mother_of_chunks[p][i][j][k];
                            }
                        }
                    }
                }
                char**** new_mother = malloc(Nr_Objects * sizeof(mother_of_chunks[0]));
                int counter = 0;
                for(int l = 0; l < *object_counter; l++)
                {
                    if (l != s && l != p)
                    {
                        new_mother[counter] = mother_of_chunks[l];
                        counter++;
                    }
                    else free_chunk_transform(mother_of_chunks[l], width, height);
                }
                new_mother[counter] = merged_chunk;
                free(mother_of_chunks);
                mother_of_chunks = new_mother;
                *object_counter-=1;
            }
            if(original != *object_counter) break;
        }
        if(original != *object_counter) break;
    }
    return mother_of_chunks;
}

char*** chunk_apply_gravity(
    char*** chunk, int width, int height, int depth, int* new_height) {

    int object_counter = 0;
    char**** TheGangOfObjects = malloc(Nr_Objects * sizeof(chunk));

    char*** temp_chunk = chunk_duplicate_air(width, height, depth);
    for(int j = 0; j < height; j++)
    {
        for(int i = 0; i < width; i++)
        {
            for(int k = 0; k < depth; k++)
            {
                // printf("d ", chunk[i][j][k]);
                if(chunk[i][j][k] != BLOCK_AIR) 
                {
                    if (temp_chunk[i][j][k] == 0 || object_counter == 0)
                    {
                        char*** space_object = chunk_duplicate_air(width, height, depth);
                        temp_chunk = find_object(temp_chunk, space_object,
                        chunk, width, height, depth, i, j, k, chunk[i][j][k]);
                        TheGangOfObjects[object_counter] = space_object;

                        // printf("\n");
                        // for(int j = 0; j < height; j++)
                        // {
                        //     for(int i = 0; i < width; i++)
                        //     {
                        //         for(int k = 0; k < depth; k++)
                        //         {
                        //             printf("%d ", TheGangOfObjects[object_counter][i][j][k]);
                        //         }
                        //         printf("\n");
                        //     }
                        //     printf("\n");
                        // }
                        
                        object_counter++;
                    }
                }
            }
        }
    }
    TheGangOfObjects = check_for_joined_objects(TheGangOfObjects, &object_counter,
    width, height, depth);
    int changes = 1; free_chunk_transform(chunk, width, height);
    // return TheGangOfObjects[2];
    while(changes != 0)
    {
        changes = 0;
        for(int s = 0; s < object_counter; s++)
        {
            int support = 0;
            // printf("\n\n");
            for(int j = 0; j < height; j++)
            {
                for(int i = 0; i < width; i++)
                {
                    for(int k = 0; k < depth; k++)
                    {
                        // printf("%d ", TheGangOfObjects[s][i][j][k]);
                        if(TheGangOfObjects[s][i][j][k] != 0)
                        {
                            if(j == 0)
                            {
                                support = 1; 
                                break; break; break;
                            }
                            else if(TheGangOfObjects[s][i][j-1][k] == BLOCK_AIR
                                    && temp_chunk[i][j-1][k] != BLOCK_AIR)
                            {
                                support = 1;
                            }
                        }
                    }
                    // printf("\n");
                }
                // printf("\n");
            }
            if(!support)
            {
                temp_chunk = fall_down_1block(TheGangOfObjects[s],
                    temp_chunk, width, height, depth);
                changes++;
            }
        }
    }
    int floor = height;
    for(int i = 0; i < object_counter; i++)
    {
        free_chunk_transform(TheGangOfObjects[i], width, height);
    }
    free(TheGangOfObjects);
    for(int j = height-1; j >= 0; j--)
    {
        for(int i  = 0; i < width; i++)
        {
            for(int k = 0; k < depth; k++)
            {
                if(temp_chunk[i][j][k] == BLOCK_AIR) continue;
                else {floor = j+1; goto A1;}
            }
        }
    }
    A1:*new_height = floor;
    return temp_chunk;
}

