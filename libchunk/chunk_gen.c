#include <stdlib.h>
#include <math.h>
#include "chunk.h"

char*** chunk_place_block(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, char block) {
        if( x >= width || x < 0 || 
            y >= height || y < 0 ||
            z >= depth || z < 0) return chunk;
        else chunk[x][y][z] = block;
        return chunk;
}

char*** chunk_fill_cuboid(
    char*** chunk, int width, int height, int depth,
    int x0, int y0, int z0, int x1, int y1, int z1, char block) {

    int direction_X = 0, direction_Y = 0, direction_Z = 0;
    if (x0 > width) x0 = width;
    if (x0 < 0) x0 = 0;
    if (x1 > width) x1 = width;
    if (x1 < 0) x1 = 0;
    if (y0 > height) y0 = height;
    if (y0 < 0) y0 = 0;
    if (y1 > height) y1 = height;
    if (y1 < 0) y1 = 0;
    if (z0 > depth) z0 = depth;
    if (z0 < 0) z0 = 0;
    if (z1 > depth) z1 = depth;
    if (z1 < 0) z1 = 0;
    if (x0 > x1) direction_X = x1; else direction_X = x0;
    if (y0 > y1) direction_Y = y1; else direction_Y = y0;
    if (z0 > z1) direction_Z = z1; else direction_Z = z0;
    for(int i = 0; i <= abs(x0 - x1); i++)
    {
        for(int j = 0; j <= abs(y0 - y1); j++)
        {
            for(int k = 0; k <= abs(z0 - z1); k++)
            {
                // if( i >= width || i < 0 ||
                //     j >= height || j < 0 ||
                //     k >= depth || k < 0) continue;
                // else chunk[direction_X + i][direction_Y + j][direction_Z + k] = block;
                chunk[direction_X + i][direction_Y + j][direction_Z + k] = block;
            }
        }
    }
    return chunk;
}

char*** chunk_fill_sphere(
    char*** chunk, int width, int height, int depth,
    int x, int y, int z, double radius, char block) {
    
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            for(int k = 0; k < depth; k++)
            {
                if(sqrt(pow(abs(i - x),2) +
                pow(abs(j - y), 2) +
                pow(abs(k - z), 2)) > radius) continue;
                else chunk[i][j][k] = block;
            }
        }
    }
    return chunk;
}
