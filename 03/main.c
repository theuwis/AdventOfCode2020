#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


static bool map[1000][1000];    /* True = open, false = tree. */
static uint16_t count = 0;
static uint16_t width = 0;


static void _parse_line(char * line)
{
    width = strlen(line);

    for(uint16_t i = 0; i < width; i++){
        (line[i] == '.') ? (map[count][i] = true) : (map[count][i] = false);
    }

    count++;
}


static void _parse_input(void)
{
    char *data_read = NULL;
    size_t len;
    ssize_t read;
    FILE *fp;

    if(!(fp = fopen("input.txt", "r")))
    {
        fprintf(stderr, "Error opening file\n");
        exit(EXIT_FAILURE);
    }

    while((read = getline(&data_read, &len, fp)) != -1)
    {
        data_read[strcspn(data_read, "\r\n")] = '\0';
        _parse_line(data_read);
    }

    free(data_read);
    fclose(fp);
}


uint16_t _traverse_map(uint16_t x_step, uint16_t y_step)
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t x_max = width;
    uint16_t y_max = count;
    uint16_t nof_trees = 0;

    while(y < (y_max - 1))
    {
        x += x_step;
        y += y_step;

        x %= x_max;

        if(!map[y][x])
        {
            nof_trees++;
        }
    }

    return nof_trees;
}


int main(void)
{
    _parse_input();

    uint16_t ans1 = _traverse_map(3, 1);

    uint64_t ans2 = _traverse_map(1, 1);
    ans2 *= _traverse_map(3, 1);
    ans2 *= _traverse_map(5, 1);
    ans2 *= _traverse_map(7, 1);
    ans2 *= _traverse_map(1, 2);

    printf("ans1 = %d\n", ans1);
    printf("ans2 = %ld\n", ans2);

    return EXIT_SUCCESS;
}