#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "functions.h"

/** @file main.c
 *  @author Radoslaw Rzeczkowski
 *  @date 06.2020
 *  @brief The main source file of this program.
 */

int main(int argc, char* argv[])
{
    char* file_names[2];
    file_names[0] = NULL;
    file_names[1] = NULL;
    switch(read_cmd(argc, argv, file_names))
    {
    case -1:
        print_help();
        break;
    case 1:
        if(file_names[0] && file_names[1])
            compress(file_names[0], file_names[1]);
        else
            print_help();
        break;
    case 2:
        if(file_names[0] && file_names[1])
            decompress(file_names[0], file_names[1]);
        else
            print_help();
        break;
    }

    free(file_names[0]);
    free(file_names[1]);

    return 0;
}
