#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


static uint16_t nof_groups = 0;
static uint16_t groups[1000][27]; /* groups[x][26] == nof persons in the group */

static void _parse_line(char * line)
{
    char * c = line;

    if(*c == '\0')
    {
        nof_groups++;
    }
    else
    {
        groups[nof_groups][26]++;
    }

    while(*c != '\0')
    {
        groups[nof_groups][*c - 'a']++;
        c++;
    }
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

    nof_groups++;

    free(data_read);
    fclose(fp);
}


int main(void)
{
    memset(groups, 0, sizeof(groups));
    _parse_input();

    uint32_t ans1 = 0;
    uint32_t ans2 = 0;
    for(uint16_t group = 0; group < nof_groups; group++)
    {
        for(uint16_t answer = 0; answer < 26; answer++)
        {
            if(groups[group][answer] != 0)
            {
                ans1++;
            }

            if(groups[group][answer] == groups[group][26])
            {
                ans2++;
            }
        }
    }

    printf("ans1 = %d\n", ans1);
    printf("ans2 = %d\n", ans2);

    return EXIT_SUCCESS;
}