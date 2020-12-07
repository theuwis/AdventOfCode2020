#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>


#define MAX_LEN_NAME    (30)
#define MAX_NOF_SUBBAGS (10)

typedef struct bag
{
    char name[MAX_LEN_NAME + 1];                      /*<! Name of the bag.                                */
    uint16_t subbag_amount[MAX_NOF_SUBBAGS];          /*<! Number of each subbag.                          */
    char subbags[MAX_NOF_SUBBAGS][MAX_LEN_NAME + 1];  /*<! Subbags strings.                                */
    struct bag *subbags_p[MAX_NOF_SUBBAGS];           /*<! Subbags pointers.                               */
    uint16_t nof_subbags;                             /*<! Number of subbags in the bag.                   */
    bool contains_shiny_gold;                         /*<! True if the bag contains a shiny bag somewhere. */
    uint32_t nof_total_bags;                          /*<! Total number of bags in the bag.                */
} bag;

static bag bags[1000];
static uint16_t nof_bags = 0;
static bag * _shiny_bag = NULL;


static void _parse_line(char *const line)
{
    char segments[100][MAX_LEN_NAME];
    uint16_t nof_segments = 0;

    char *token = NULL;

    token = strtok(line, " ");

    while(token != NULL)
    {
        strncpy(segments[nof_segments], token, MAX_LEN_NAME);
        token = strtok(NULL, " ");
        nof_segments++;
    }

    bag *bag = &bags[nof_bags];
    bag->nof_subbags = 0;
    bag->contains_shiny_gold = false;
    bag->nof_total_bags = 0;
    snprintf(bag->name, MAX_LEN_NAME, "%s %s", segments[0], segments[1]);

    if(strncmp(bag->name, "shiny gold", sizeof(bags[0].name)) == 0)
    {
        _shiny_bag = bag;
    }

    for(uint16_t i = 4; i < nof_segments; i += 4)
    {
        if((bag->subbag_amount[bag->nof_subbags] = atoi(segments[i])) != 0)
        {
            snprintf(bag->subbags[bag->nof_subbags], MAX_LEN_NAME, "%s %s", segments[i + 1], segments[i + 2]);
            bag->nof_subbags++;
        }
    }

    nof_bags++;
}


static uint16_t _find_bag(char *bag_name)
{
    uint16_t idx = 0;

    while(strncmp(bags[idx].name, bag_name, MAX_LEN_NAME) != 0)
    {
        idx++;
    }

    return idx;
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



static void _get_subbag_pointers(void)
{
    bag * bag;

    for(uint16_t i = 0; i < nof_bags; i++)
    {
        bag = &bags[i];

        for(uint16_t j = 0; j < bag->nof_subbags; j++)
        {
            bag->subbags_p[j] = &bags[_find_bag(bag->subbags[j])];
        }
    }
}


int main(void)
{
    _parse_input();
    _get_subbag_pointers();
    uint16_t ans1 = 0;

    /* Initial check for shiny gold. */
    for(uint16_t i = 0; i < nof_bags; i++)
    {
        for(uint16_t j = 0; j < bags[i].nof_subbags; j++)
        {
            if( (bags[i].subbags_p[j] == _shiny_bag) && (&bags[i] != _shiny_bag) )
            {
                ans1++;
                bags[i].contains_shiny_gold = true;
                break;
            }
        }
    }

    bool end = false;
    while(!end)
    {
        end = true;

        for(uint16_t i = 0; i < nof_bags; i++)
        {
            for(uint16_t j = 0; j < bags[i].nof_subbags; j++)
            {
                if(bags[i].subbags_p[j]->contains_shiny_gold)
                {
                    if(!bags[i].contains_shiny_gold)
                    {
                        ans1++;
                        bags[i].contains_shiny_gold = true;
                        end = false;
                    }
                    break;
                }
            }

            uint32_t total_bags = 0;
            for(uint16_t j = 0; j < bags[i].nof_subbags; j++){
                total_bags += bags[i].subbag_amount[j];
                total_bags += (bags[i].subbags_p[j]->nof_total_bags * bags[i].subbag_amount[j]);
            }

            if(bags[i].nof_total_bags != total_bags)
            {
                bags[i].nof_total_bags = total_bags;
                end = false;
            }
        }
    }

    printf("ans1 = %d\n", ans1);
    printf("ans2 = %d\n", _shiny_bag->nof_total_bags);

    return EXIT_SUCCESS;
}