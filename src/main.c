#include <stdio.h>

#include "../libchunk/chunk_gen.c" 
#include "../libchunk/chunk_process.c"
#include "../libchunk/chunk_transform.c"
#include "../libchunk/chunk_compress.c"

void print_chunk(char*** chunk, int width, int height, int depth) {
    printf("%d\n%d\n%d\n", width, height, depth);

    for (int x = 0; x < width; x++) { 
        for (int y = height-1; y >= 0; y--) {
            for (int z = 0; z < depth; z++)
                printf("%d ", chunk[x][y][z]);
            printf("\n");
        }
        printf("\n");
    }
}

char*** read_chunk(int* width, int* height, int* depth) {
    scanf("%d%d%d", width, height, depth);
    
    char*** chunk = malloc(*width * sizeof *chunk);
    
    for (int x = 0; x < *width; x++) {
        chunk[x] = malloc(*height * sizeof **chunk);
        for (int y = *height-1; y >= 0; y--) {
            chunk[x][y] = malloc(*depth * sizeof ***chunk);

            for (int z = 0; z < *depth; z++)
                scanf("%hhd", &chunk[x][y][z]);
        }
    }

    return chunk;
}

int main(void) {
    int width=0, height=0, depth=0;
    char*** chunk = read_chunk(&width, &height, &depth);

    // Procesare chunk
    // chunk = chunk_place_block(chunk, width, height, depth, 3, 2, 3, 2);
    // chunk = chunk_fill_cuboid(chunk, width, height, depth, 2, -1, 3, -3, 1, -2, 0);
    // chunk = chunk_fill_sphere(chunk, width, height, depth, 7, 3, 7, 6.5, 2);
    // chunk = chunk_shell(chunk, width, height, depth, 1, 1);
    // chunk = chunk_fill_xz(chunk, width, height, depth, 2, 1, 3, 3);
    // chunk = chunk_fill(chunk, width, height, depth, 1, 1, 1, 3);
    // chunk = chunk_rotate_y(chunk, width, height, depth);

    // int new_height = height; 
    // chunk = chunk_apply_gravity(chunk, width, height, depth, &new_height);

    // int copy_width = width;
    // width = depth;
    // depth = copy_width;

    int length;
    unsigned char* vector = chunk_encode(chunk, width, height, depth, &length);
    // chunk = chunk_decode(vector, width, height, depth);
    free(vector);

    print_chunk(chunk, width, height, depth);
    free_chunk_compress(chunk, width, height);
    // every chunk, and any use of calloc or malloc must be freed(every single one)
    
    return 0;
}
