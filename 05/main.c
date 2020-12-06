#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#define NOF_ROWS (128)
#define NOF_COLS (8)

int16_t seats[NOF_ROWS][NOF_COLS];

uint8_t _get_seat(uint8_t upper_limit, uint8_t lower_limit, char *const input, uint8_t iterations)
{
    uint8_t upper = upper_limit;
    uint8_t lower = lower_limit;
    char * c = input;

    for(uint8_t i = 0; i < 7; i++)
    {
        double range = ceil((upper - lower) / 2.0);

        if( (*c == 'F') || (*c == 'L') )
        {
            upper -= range;
        }
        else if( (*c == 'B') || (*c = 'R') )
        {
            lower += range;
        }
        else
        {
            fprintf(stderr, "Invalid character found (%c)\n", *c);
            exit(EXIT_FAILURE);
        }

        c++;
    }

    return lower;
}


static void _parse_line(char * line)
{
    uint16_t row = _get_seat(127, 0, line, 7);
    uint16_t col = _get_seat(7, 0, &line[7], 3);

    seats[row][col] = row * 8 + col;
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


int main(void)
{
    memset(seats, -1, sizeof(seats));
    _parse_input();

    uint16_t ans1 = 0;
    for(uint16_t row = 0; row < NOF_ROWS; row++)
    {
        for(uint16_t col = 0; col < NOF_COLS; col++)
        {
            if( (seats[row][col] != -1) && (seats[row][col] > ans1) )
            {
                ans1 = seats[row][col];
            }
        }
    }

    uint16_t ans2 = 0;
    for(uint16_t row = 0; row < NOF_ROWS; row++)
    {
        uint16_t ans2_row = 0;
        uint16_t ans2_col = 0;
        uint8_t nof_empty_seats = 0;

        for(uint16_t col = 0; col < NOF_COLS; col++)
        {
            if(seats[row][col] == -1)
            {
                nof_empty_seats++;

                ans2_row = row;
                ans2_col = col;
            }
        }

        if(nof_empty_seats == 1)
        {
            ans2 = ans2_row * 8 + ans2_col;
        }
    }

    printf("ans1 = %d\n", ans1);
    printf("ans2 = %d\n", ans2);

    return EXIT_SUCCESS;
}