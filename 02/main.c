#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>


typedef struct
{
    uint16_t min;
    uint16_t max;
    uint16_t nof_occurences;
    char letter;
    char pswd[50];
} password;

static password passwords[1000];
static uint16_t count = 0;


static void _parse_line(char * line)
{
    char *token = NULL;

    token = strtok(line, "-");
    passwords[count].min = atoi(token);

    token = strtok(NULL, "-");
    passwords[count].max = atoi(token);

    token = strtok(token, " ");
    token = strtok(NULL, " ");
    passwords[count].letter = token[0];

    token = strtok(NULL, " ");
    strncpy(passwords[count].pswd, token, sizeof(passwords[0].pswd) - 1);

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

    uint16_t nof_correct_pswds_1 = 0;
    uint16_t nof_correct_pswds_2 = 0;

    for(uint16_t i = 0; i < count; i++)
    {
        password *password = &passwords[i];
        password->nof_occurences = 0;

        for(uint8_t j = 0; j < strlen(password->pswd); j++)
        {
            (password->pswd[j] == password->letter) ? password->nof_occurences++ : 0;
        }

        if((password->nof_occurences >= password->min) &&
           (password->nof_occurences <= password->max))
        {
            nof_correct_pswds_1++;
        }

         if( ((password->pswd[password->min - 1] == password->letter) ||
              (password->pswd[password->max - 1] == password->letter) ) &&
              (password->pswd[password->min - 1] != password->pswd[password->max - 1]))
        {
            nof_correct_pswds_2++;
        }
    }

    printf("ans1 = %d\n", nof_correct_pswds_1);
    printf("ans2 = %d\n", nof_correct_pswds_2);

    return EXIT_SUCCESS;
}