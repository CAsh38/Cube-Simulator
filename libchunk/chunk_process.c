#include "chunk.h"

void free_chunk_process(char*** chunk, int width, int height)
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

char*** chunk_duplicate(
    char*** chunk, int width, int height, int depth){
    
    char*** new_chunk = malloc(width * sizeof *new_chunk);
    for (int x = 0; x < width; x++) {
        new_chunk[x] = malloc(height * sizeof **new_chunk);
        for (int y = height-1; y >= 0; y--) {
            new_chunk[x][y] = malloc(depth * sizeof ***new_chunk);

            for (int z = 0; z < depth; z++)
                new_chunk[x][y][z] = chunk[x][y][z];
        }
    }
    return new_chunk;
}

char*** chunk_shell(
    char*** chunk, int width, int height, int depth,
    char target_block, char shell_block) {

    char*** new_chunk = chunk_duplicate(chunk, width, height, depth);
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < depth; k++)
            {
                if(chunk[i][j][k] != target_block) continue;
                else {
                    new_chunk[i][j][k] = target_block;
                    if (i-1 >= 0) if(chunk[i-1][j][k] != target_block)
                        new_chunk[i-1][j][k] = shell_block;
                    if (i+1 <= width-1) if(chunk[i+1][j][k] != target_block)
                        new_chunk[i+1][j][k] = shell_block;
                    if (j-1 >= 0) if(chunk[i][j-1][k] != target_block)
                        new_chunk[i][j-1][k] = shell_block;
                    if (j+1 <= height-1) if(chunk[i][j+1][k] != target_block)
                        new_chunk[i][j+1][k] = shell_block;
                    if (k-1 >= 0) if(chunk[i][j][k-1] != target_block)
                        new_chunk[i][j][k-1] = shell_block;
                    if (k+1 <= depth-1) if(chunk[i][j][k+1] != target_block)
                        new_chunk[i][j][k+1] = shell_block;
                    if (i-1 >=0 && j-1 >= 0) 
                        if(chunk[i-1][j-1][k] != target_block)
                            new_chunk[i-1][j-1][k] = shell_block;
                    if (i-1 >=0 && j+1 <= height-1)
                         if(chunk[i-1][j+1][k] != target_block)
                            new_chunk[i-1][j+1][k] = shell_block;
                    if (i+1 <=width-1 && j-1 >= 0)
                        if(chunk[i+1][j-1][k] != target_block)
                            new_chunk[i+1][j-1][k] = shell_block;
                    if (i+1 <=width-1 && j+1 <= height-1)
                        if(chunk[i+1][j+1][k] != target_block)
                            new_chunk[i+1][j+1][k] = shell_block;
                    if (i-1 >=0 && k-1 >= 0)
                        if(chunk[i-1][j][k-1] != target_block)
                            new_chunk[i-1][j][k-1] = shell_block;
                    if (i-1 >=0 && k+1 <= depth-1)
                        if(chunk[i-1][j][k+1] != target_block)
                            new_chunk[i-1][j][k+1] = shell_block;
                    if (i+1 <=width-1 && k-1 >= 0)
                        if(chunk[i+1][j][k-1] != target_block)
                            new_chunk[i+1][j][k-1] = shell_block;
                    if (i+1 <=width-1 && k+1 <= depth-1)
                        if(chunk[i+1][j][k+1] != target_block)
                            new_chunk[i+1][j][k+1] = shell_block;
                    if (j-1 >=0 && k-1 >= 0)
                        if(chunk[i][j-1][k-1] != target_block)
                            new_chunk[i][j-1][k-1] = shell_block;
                    if (j-1 >=0 && k+1 <= depth-1)
                        if(chunk[i][j-1][k+1] != target_block)
                            new_chunk[i][j-1][k+1] = shell_block;
                    if (j+1 <=height-1 && k-1 >= 0)
                        if(chunk[i][j+1][k-1] != target_block)
                            new_chunk[i][j+1][k-1] = shell_block;
                    if (j+1 <=height-1 && k+1 <= depth-1)
                        if(chunk[i][j+1][k+1] != target_block)
                            new_chunk[i][j+1][k+1] = shell_block;
                }
            }
        }
    }
    free_chunk_process(chunk, width, height);
    return new_chunk;
}

char*** chunk_fill_xz_special(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block, char** planXOZ)
    {
        // for(int i = 0; i < width; i++)
        // {
        //     for(int k = 0; k < depth; k++)
        //     {
        //         printf("%d ", planXOZ[i][k]);
        //     }
        //     printf("\n");
        // }
        if(z+1 <= depth-1)
        {
            if(planXOZ[x][z+1] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y][z+1]){
                planXOZ[x][z] = block;
                chunk = chunk_fill_xz_special(
                    chunk, width, height, depth,
                    x, y, z+1, block, planXOZ);
                }
            }
        }
        if(z-1 >= 0)
        {
            if(planXOZ[x][z-1] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y][z-1]){
                planXOZ[x][z] = block;
                
                chunk = chunk_fill_xz_special(
                    chunk, width, height, depth,
                    x, y, z-1, block, planXOZ);
                }
            }
        }
        if(x-1 >= 0)
        {
            if(planXOZ[x-1][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x-1][y][z]){
                planXOZ[x][z] = block;

                chunk = chunk_fill_xz_special(
                    chunk, width, height, depth,
                    x-1, y, z, block, planXOZ);
                }
            }
        }
        if(x+1 <= width-1)
        {
            if(planXOZ[x+1][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x+1][y][z]){
                planXOZ[x][z] = block;
                chunk = chunk_fill_xz_special(
                    chunk, width, height, depth,
                    x+1, y, z, block, planXOZ);
                }
            }
        }
    planXOZ[x][z] = block;
    return chunk;
}

char*** chunk_fill_xz(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        char** planXOZ = malloc(width * sizeof(**chunk));
        for(int i = 0; i < width; i++)
        {
            planXOZ[i] = calloc(depth,sizeof(***chunk));
            for(int k = 0; k < depth; k++)
            {
                planXOZ[i][k] = chunk[i][y][k];
                // printf("%d ", planXOZ[i][k]);
            }
            // printf("\n");
        }
        chunk = chunk_fill_xz_special(
            chunk, width, height, depth, x, y, z ,block, planXOZ);
        for(int i = 0; i < width; i++)
        {
            for(int k = 0; k < depth; k++)
            {
                chunk[i][y][k] = planXOZ[i][k];
            }
        }
        for(int i = width-1; i >= 0; i--)
        {
            free(planXOZ[i]);
        }
        free(planXOZ);
    return chunk;
}

char*** chunk_fill_special(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block, char*** dup_chunk){
    
        if(x+1 <= width-1)
        {
            if(dup_chunk[x+1][y][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x+1][y][z]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x+1, y, z, block, dup_chunk);
                }
            }
        }
        if(x-1 >= 0)
        {
            if(dup_chunk[x-1][y][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x-1][y][z]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x-1, y, z, block, dup_chunk);
                }
            }
        }
        if(y+1 <= height-1)
        {
            if(dup_chunk[x][y+1][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y+1][z]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x, y+1, z, block, dup_chunk);
                }
            }
        }
        if(y-1 >= 0)
        {
            if(dup_chunk[x][y-1][z] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y-1][z]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x, y-1, z, block, dup_chunk);
                }
            }
        }
        if(z+1 <= depth-1)
        {
            if(dup_chunk[x][y][z+1] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y][z+1]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x, y, z+1, block, dup_chunk);
                }
            }
        }
        if(z-1 >= 0)
        {
            if(dup_chunk[x][y][z-1] == chunk[x][y][z])
            {
                if(chunk[x][y][z] == chunk[x][y][z-1]){
                dup_chunk[x][y][z] = block;
                chunk = chunk_fill_special(
                    chunk, width, height, depth,
                    x, y, z-1, block, dup_chunk);
                }
            }
        }
    dup_chunk[x][y][z] = block;
    return chunk; 
}

char*** chunk_fill(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
    char*** dup_chunk = chunk_duplicate(chunk, width, height, depth);
    chunk = chunk_fill_special(chunk, width, height, depth,
                                x, y, z, block, dup_chunk);

    free_chunk_process(chunk, width, height);
    return dup_chunk;
}
