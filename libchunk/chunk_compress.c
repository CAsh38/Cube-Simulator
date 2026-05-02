#include "chunk.h"
#include <stdlib.h>

#define MINIM_INT 32
#define SHIFT_LEFT_6 6
#define MAXIM_INT 4096
#define MASK_15 15
#define MASK_255 255
#define SHIFT_RIGHT_8 8
#define MASK_5 5

void free_chunk_compress(char*** chunk, int width, int height)
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

unsigned char* chunk_encode(
    char*** chunk, int width, int height, int depth,
    int* length) {

    int* int_storage = calloc(width * height * depth,
            sizeof(int));

    int* char_storage = calloc(width * height * depth,
            sizeof(int));

    unsigned char* vector = calloc(width * height * depth,
            sizeof(unsigned char));

    *length = 0;

    char block = chunk[0][0][0];
    int runs = 0, counter = 0;
    for(int j = 0; j < height; j++)
    {
        for(int k = 0; k < depth; k++)
        {
            for(int i = 0; i < width; i++)
            {
                if(chunk[i][j][k] != block)
                {
                    char_storage[counter] = block;
                    int_storage[counter] = runs;
                    block = chunk[i][j][k];
                    runs = 1, counter++;
                }
                else runs++;
            }
        }
        if(j == height-1)
        {
            char_storage[counter] = block;
            int_storage[counter] = runs;
            block = chunk[width - 1][j][depth - 1];
        }
    }
    // free_chunk_compress(chunk, width, height);
    int char_count = 0;
    for(int i = 0; i <= counter; i++){
        // printf("(%d, %d)\n", char_storage[i], int_storage[i]);
        while(int_storage[i] != 0)
        {
            if(int_storage[i] < MINIM_INT)
            {
                int char_in_bits = char_storage[i] << SHIFT_LEFT_6;
                vector[char_count] |= char_in_bits;
                int int_in_bits = int_storage[i];
                vector[char_count] |= int_in_bits;
                int_storage[i] = 0;
                *length+=1;
            }
            else 
            {
                if(int_storage[i]/MAXIM_INT)
                {
                    int char_in_bits = char_storage[i] << SHIFT_LEFT_6;
                    vector[char_count] |= char_in_bits;
                    int additional_bit = 1 << MASK_5;
                    vector[char_count] |= additional_bit;
                    vector[char_count] |= MASK_15;
                    vector[char_count + 1] |= MASK_255;
                    int_storage[i] -= ((MAXIM_INT) - 1);
                    // printf("%d ", vector[char_count]);
                    char_count++;
                    *length+=2;
                }
                else 
                {
                    int char_in_bits = char_storage[i] << SHIFT_LEFT_6;
                    vector[char_count] |= char_in_bits;
                    int int_in_bits_first = int_storage[i] >> SHIFT_RIGHT_8;
                    int additional_bit = 1 << MASK_5;
                    vector[char_count] |= additional_bit;
                    vector[char_count] |= int_in_bits_first;
                    vector[char_count + 1] |= int_storage[i];
                    // printf("%d ", vector[char_count]);
                    char_count++;
                    int_storage[i] = 0;
                    *length+=2;
                }
            }
            // printf("%d\n", vector[char_count]);
            char_count++;

        }
    }
    free(int_storage); free(char_storage);
    return vector;
}

char*** chunk_decode(
    unsigned char* code, int width, int height, int depth) {

    int* int_storage = calloc(width * height * depth,
            sizeof(int));

    int* char_storage = calloc(width * height * depth,
            sizeof(int));
    int array_counter = 0;

    int i = 0;
    while(code[i] != 0)
    {
        int dependency_bit = (code[i] & MINIM_INT) >> MASK_5;
        if(dependency_bit) 
        {
            unsigned char block = code[i] >> SHIFT_LEFT_6; // it's right
            char_storage[array_counter] = block;
            int number = code[i] & MASK_15;
            number = number << SHIFT_RIGHT_8; // it's left
            number += code[i+1];
            int_storage[array_counter] += number;
            if(!(code[i+2] >> SHIFT_LEFT_6 == block)) array_counter++;
            i++;
        }
        else
        {
            unsigned char block = code[i] >> SHIFT_LEFT_6;
            char_storage[array_counter] = block;
            int_storage[array_counter] += code[i] & (MINIM_INT - 1);
            array_counter++;
        }
        // printf("(%d, %d)\n", char_storage[array_counter-1], int_storage[array_counter-1]);
        i++;
    }
    
    // for(int i = 0; i < array_counter; i++)
    // {
    //     int_storage[i] = int_storage[i] - ((int_storage[i] >> 6) << 6);
    //     printf("(%d, %d)\n", char_storage[i], int_storage[i]);
    // }

    int char_counter = 0;
    char*** chunk = malloc(width * sizeof *chunk);
    
    for(int i = 0; i < width; i++)
    {
        chunk[i] = malloc(height * sizeof **chunk);
        for(int j = 0; j < height; j++)
        {
            chunk[i][j] = malloc(depth * sizeof ***chunk);
            for(int k = 0; k < depth; k++)
            {
                chunk[i][j][k] = BLOCK_AIR;
            }
        }
    }

    for (int j = 0; j < height; j++) 
    {
        for (int k = 0; k < depth; k++) 
        {
            for (int i = 0; i < width; i++)
            {
                chunk[i][j][k] = (char)char_storage[char_counter];
                int_storage[char_counter]--;
                if(!int_storage[char_counter]) char_counter++;
            }
        }
    }
    free(int_storage); free(char_storage);
    // free(code);
    return chunk;
}

