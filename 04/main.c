#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum {
    UNIT_NONE,
    UNIT_CM,
    UNIT_IN
} UNITS;

typedef struct
{
    int32_t byr;
    int32_t iyr;
    int32_t eyr;
    int32_t hgt;
    UNITS hgt_unit;
    char hcl[30];
    char ecl[30];
    char pid[30];
    char cid[30];
} passport;

static passport passports[1000];

static uint16_t count = 0;
static uint16_t nof_passports = 0;


static void _parse_line(char * line)
{
    char * p;

    if((p = strstr(line, "byr")) != NULL)
    {
        passports[nof_passports].byr = atoi(&line[p - line + 4]);
    }

    if((p = strstr(line, "iyr")) != NULL)
    {
        passports[nof_passports].iyr = atoi(&line[p - line + 4]);
    }

    if((p = strstr(line, "eyr")) != NULL)
    {
        passports[nof_passports].eyr = atoi(&line[p - line + 4]);
    }

    if((p = strstr(line, "hgt")) != NULL)
    {
        passports[nof_passports].hgt = atoi(&line[p - line + 4]);
    }

    if((p = strstr(line, "in")) != NULL)
    {
        passports[nof_passports].hgt_unit = UNIT_IN;
    }

    if((p = strstr(line, "cm")) != NULL)
    {
        passports[nof_passports].hgt_unit = UNIT_CM;
    }

    if((p = strstr(line, "hcl")) != NULL)
    {
        char * l = &line[p - line + 4];
        char * b = passports[nof_passports].hcl;

        while( (*l != ' ') && (*l != '\0') )
        {
            *b++ = *l++;
        }
        b = '\0';
    }

    if((p = strstr(line, "ecl")) != NULL)
    {
        char * l = &line[p - line + 4];
        char * b = passports[nof_passports].ecl;

        while( (*l != ' ') && (*l != '\0') )
        {
            *b++ = *l++;
        }
        b = '\0';
    }

    if((p = strstr(line, "pid")) != NULL)
    {
        char * l = &line[p - line + 4];
        char * b = passports[nof_passports].pid;

        while( (*l != ' ') && (*l != '\0') )
        {
            *b++ = *l++;
        }
        b = '\0';
    }

    if((p = strstr(line, "cid")) != NULL)
    {
        char * l = &line[p - line + 4];
        char * b = passports[nof_passports].cid;

        while( (*l != ' ') && (*l != '\0') )
        {
            *b++ = *l++;
        }
        b = '\0';
    }

    if(strlen(line) == 0)
    {
        nof_passports++;
    }

    count++;
}


static void _parse_input(void)
{
    for(uint16_t i = 0; i < 1000; i++)
    {
        passports[i].byr = -1;
        passports[i].iyr = -1;
        passports[i].eyr = -1;
        passports[i].hgt = -1;
        passports[i].hgt_unit = UNIT_NONE;
        passports[i].hcl[0] = '\0';
        passports[i].ecl[0] = '\0';
        passports[i].pid[0] = '\0';
        passports[i].cid[0] = '\0';
    }


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

    nof_passports++;

    free(data_read);
    fclose(fp);
}


bool _check_byr(int32_t byr)
{
    if( (byr < 1920) || (byr > 2002) )
    {
        return false;
    }

    return true;
}


bool _check_iyr(int32_t iyr)
{
    if( (iyr < 2010) || (iyr > 2020) )
    {
        return false;
    }

    return true;
}


bool _check_eyr(int32_t eyr)
{
    if( (eyr < 2020) || (eyr > 2030) )
    {
        return false;
    }

    return true;
}


bool _check_hgt(int32_t hgt, UNITS unit)
{
    if(unit == UNIT_NONE)
    {
        return false;
    }

    if( (unit == UNIT_CM) && ((hgt < 150) || (hgt > 193)) )
    {
        return false;
    }

    if( (unit == UNIT_IN) && ((hgt < 59) || (hgt > 76)) )
    {
        return false;
    }

    return true;
}


bool _check_hcl(char *const hcl)
{
    char * c = hcl;

    if(strlen(hcl) != 7)
    {
        return false;
    }

    if(*c != '#')
    {
        return false;
    }

    c++;

    while(*c != '\0')
    {
        if(!isxdigit(*c))
        {
            return false;
        }

        c++;
    }


    return true;
}


bool _check_ecl(char *const ecl)
{
    if(strlen(ecl) != 3)
    {
        return false;
    }

    if((strstr(ecl, "amb") == NULL)  &&
        (strstr(ecl, "blu") == NULL)  &&
        (strstr(ecl, "brn") == NULL)  &&
        (strstr(ecl, "gry") == NULL)  &&
        (strstr(ecl, "grn") == NULL)  &&
        (strstr(ecl, "hzl") == NULL)  &&
        (strstr(ecl, "oth") == NULL))
    {
        return false;
    }

    return true;
}


bool _check_pid(char *const pid)
{
    char * c = pid;

    if(strlen(pid) != 9)
    {
        return false;
    }

    while(*c != '\0')
    {
        if(!isdigit(*c))
        {
            return false;
        }

        c++;
    }

    return true;
}


int main(void)
{
    _parse_input();

    uint16_t ans1 = 0;
    uint16_t ans2 = 0;
    for(uint16_t i = 0; i < nof_passports; i++)
    {
        passport passport = passports[i];

        if((passport.byr != -1) &&
            (passport.iyr != -1) &&
            (passport.eyr != -1) &&
            (passport.hgt != -1) &&
            (passport.hcl[0] != '\0') &&
            (passport.ecl[0] != '\0') &&
            (passport.pid[0] != '\0')
           )
        {
            ans1++;
        }

        if(_check_byr(passport.byr)  &&
            _check_iyr(passport.iyr) &&
            _check_eyr(passport.eyr) &&
            _check_hgt(passport.hgt, passport.hgt_unit) &&
            _check_hcl(passport.hcl) &&
            _check_ecl(passport.ecl) &&
            _check_pid(passport.pid)
           )
        {
            ans2++;
        }
    }

    printf("ans1 = %d\n", ans1);
    printf("ans2 = %d\n", ans2);

    return EXIT_SUCCESS;
}