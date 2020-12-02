#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


static uint16_t input[1000];
static uint16_t count;


static void _parse_line(char * line)
{
    input[count] = atoi(line);
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


int main(void)
{
    _parse_input();

    for(uint16_t i = 0; i < count; i++){
        for(uint16_t j = 0; j < count; j++){
            if(input[i] + input[j] == 2020){
                printf("ans1 = %d\n", input[i] * input[j]);
            }

            for(uint16_t h = 0; h < count ; h++){
                if(input[i] + input[j] + input[h] == 2020){
                    printf("ans2 = %d\n", input[i] * input[j] * input[h]);
                    return EXIT_SUCCESS;
                }
            }
        }
    }

   return EXIT_SUCCESS;
}