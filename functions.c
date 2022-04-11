#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

#include "functions.h"

/** @file functions.c
 *  @author Radoslaw Rzeczkowski
 *  @date 06.2020
 *  @brief A source code file containing the functions used by the program.
 */

struct list* list_add(struct list* head, unsigned int _id, char _entry, char* _str, unsigned int _key){
    if(!head){
        head = (struct list*)malloc(sizeof(struct list));
        head ->id = _id;
        head ->entry = _entry;
        head ->key = ++_key;
        head ->str = (char*)malloc(strlen(_str) + sizeof(char));
        strcpy(head ->str, _str);
        head ->p_next = NULL;

        return head;
    }
    struct list *cpy = head;
    while(cpy ->p_next){
        cpy = cpy ->p_next;
    }
    cpy ->p_next = (struct list*)malloc(sizeof(struct list));
    cpy = cpy ->p_next;

    cpy ->id = _id;
    cpy ->entry = _entry;
    cpy ->key = ++_key;
    cpy ->str = (char*)malloc(strlen(_str) + sizeof(char));
    strcpy(cpy ->str, _str);
    cpy ->p_next = NULL;

    return cpy;
}

struct list* list_free(struct list* head){
    struct list* cpy = head;
    while(cpy){
        cpy = head ->p_next;
        free(head->str);
        free(head);
        head = cpy;
    }
    return NULL;
};

void list_print(struct list* head){
    while(head){
        printf("%d, %c\t%d\t%s\n", head ->id, head ->entry, head ->key, head ->str);
        head = head ->p_next;
    }
}

struct list* list_search_string(struct list* head, char* _str){
    if(!head)
        return NULL;
    if(!(strcmp(head ->str, _str))){
        return head;
    }
    return list_search_string(head ->p_next, _str);
};

void init_bin(FILE* out_bin, unsigned int count){
    char first = 'L';
    char second = 'Z';
    char code = (char)78;

    fwrite(&first, sizeof(char), 1, out_bin);
    fwrite(&second, sizeof(char), 1, out_bin);
    fwrite(&code, sizeof(char), 1, out_bin);
    fwrite(&count, sizeof(unsigned int), 1, out_bin);
}

void list_write_bin(FILE* out_bin, struct list* head){
    unsigned char uchar_conv;
    unsigned short ushort_conv;

    while(head && head ->key <= UCHAR_MAX){
        uchar_conv = (unsigned char)head ->id;
        fwrite(&uchar_conv, sizeof(unsigned char), 1, out_bin);
        fwrite(&head ->entry, sizeof(char), 1, out_bin);
        head = head ->p_next;
    }
    while(head && head ->key <= USHRT_MAX){
        ushort_conv = (unsigned short)head ->id;
        fwrite(&ushort_conv, sizeof(unsigned short), 1, out_bin);
        fwrite(&head ->entry, sizeof(char), 1, out_bin);
        head = head ->p_next;
    }
    while(head){
        fwrite(&head ->id, sizeof(unsigned int), 1, out_bin);
        fwrite(&head ->entry, sizeof(char), 1, out_bin);
        head = head ->p_next;
    }
};

void compress(char* in_txt_name, char* out_bin_name){
    FILE* in_txt = NULL;
    FILE* out_bin = NULL;

    in_txt = fopen(in_txt_name, "r");
    if(!in_txt){
        printf("Error: Couldn't open input text file \"%s\".\nExiting program.\n", in_txt_name);
        return;
    }
    printf("Compressing text file \"%s\".\n", in_txt_name);

    int c;
    unsigned int count = 0;
    char read[] = {'x', '\0'};
    char* buff = (char*)malloc(225 * sizeof(char));
    buff[0] = '\0';
    struct list* list_head = NULL;
    struct list* list_last = NULL;
    struct list* list_found = NULL;
    struct list* list_found_last = NULL;

    if((c = fgetc(in_txt)) != EOF){
        read[0] = (char) c;
        list_head = list_add(list_head, 0, read[0], read, count);
        list_last = list_head;
        count ++;
    }

    while((c = fgetc(in_txt)) != EOF){
        read[0] = (char) c;
        strcat(buff, read);

        list_found_last = list_found;
        list_found = list_search_string(list_head, buff);

        if(!list_found){
            if(list_found_last)
                list_last = list_add(list_last, list_found_last ->key, read[0], buff, count);
            else
                list_last = list_add(list_last, 0, read[0], read, count);

            count ++;
            buff[0] = '\0';
            list_found = NULL;
        }
    }
    if(list_head){
        if(list_found_last)
            list_last = list_add(list_last, list_found_last ->key, read[0], buff, count);
        else
            list_last = list_add(list_last, 0, read[0], read, count);
        count ++;
    }


    fclose(in_txt);
    free(buff);

    if(count < 1){
        printf("No data to compress.\nExiting program.\n");
        list_head = list_free(list_head);
        return;
    }

    out_bin = fopen(out_bin_name, "wb");
    if(!out_bin){
        printf("Error: Couldn't open output binary file \"%s\".\nExiting program.\n", out_bin_name);
        list_head = list_free(list_head);
        return;
    }

    init_bin(out_bin, count);
    list_write_bin(out_bin, list_head);
    fclose(out_bin);

    printf("File successfully compressed to \"%s\"\n", out_bin_name);

    list_head = list_free(list_head);
    return;
}

unsigned int check_input(FILE* in_bin){
    char read_char;
    unsigned int read_int;
    fread(&read_char, sizeof(char), 1, in_bin);
    if(read_char != 'L')
        return 0;
    fread(&read_char, sizeof(char), 1, in_bin);
    if(read_char != 'Z')
        return 0;
    fread(&read_char, sizeof(char), 1, in_bin);
    if(read_char != 78)
        return 0;
    fread(&read_int, sizeof(unsigned int), 1, in_bin);
    return read_int;
}

struct list* list_search_key(struct list* head, unsigned int _key){
    if(!head)
        return NULL;
    if(head ->key == _key){
        return head;
    }
    return list_search_key(head ->p_next, _key);
};

void list_write_txt(FILE* out_txt, struct list* head){
    while(head){
        fputs(head ->str, out_txt);
        head = head ->p_next;
    }
}

void decompress(char* in_bin_name, char* out_txt_name){
    FILE* in_bin = NULL;
    FILE* out_txt = NULL;

    in_bin = fopen(in_bin_name, "rb");
    if(!in_bin){
        printf("Error: Couldn't open input binary file \"%s\".\nExiting program.\n", in_bin_name);
        return;
    }

    unsigned int records_count = check_input(in_bin);
    if(records_count < 1){
        printf("Error: Wrong binary file format or no data to decompress.\nExiting program.\n");
        return;
    }
    printf("Decompressing text file \"%s\".\n", in_bin_name);

    unsigned int read_id = 0;
    unsigned int i = 0;
    char read_entry[] = {'x', '\0'};
    char* buff = (char*)malloc(255 * sizeof(char));
    struct list* list_head = NULL;
    struct list* list_last = NULL;
    struct list* list_found = NULL;

    fread(&read_id, sizeof(unsigned char), 1, in_bin);
    fread(&read_entry[0], sizeof(char), 1, in_bin);
    list_head = list_add(list_head, read_id, read_entry[0], read_entry, i);
    list_last = list_head;
    i++;

    while(i < records_count && i < UCHAR_MAX){
        buff[0] = '\0';
        fread(&read_id, sizeof(unsigned char), 1, in_bin);
        fread(&read_entry[0], sizeof(char), 1, in_bin);
        if(read_id != 0){
            list_found = list_search_key(list_head, read_id);
            strcat(buff, list_found ->str);
        }
        strcat(buff, read_entry);
        list_last = list_add(list_last, read_id, read_entry[0], buff, i);
        i++;
    }
    while(i < records_count && i < USHRT_MAX){
        buff[0] = '\0';
        fread(&read_id, sizeof(unsigned short), 1, in_bin);
        fread(&read_entry[0], sizeof(char), 1, in_bin);
        if(read_id != 0){
            list_found = list_search_key(list_head, read_id);
            strcat(buff, list_found ->str);
        }
        strcat(buff, read_entry);
        list_last = list_add(list_last, read_id, read_entry[0], buff, i);
        i++;
    }
    while(i < records_count){
        buff[0] = '\0';
        fread(&read_id, sizeof(unsigned int), 1, in_bin);
        fread(&read_entry[0], sizeof(char), 1, in_bin);
        if(read_id != 0){
            list_found = list_search_key(list_head, read_id);
            strcat(buff, list_found ->str);
        }
        strcat(buff, read_entry);
        list_last = list_add(list_last, read_id, read_entry[0], buff, i);
        i++;
    }

    fclose(in_bin);
    list_found = NULL;
    free(buff);

    out_txt = fopen(out_txt_name, "w");
    if(!out_txt){
        printf("Error: Couldn't open output text file \"%s\".\nExiting program.\n", out_txt_name);
        list_free(list_head);
        return;
    }

    list_write_txt(out_txt, list_head);
    fclose(out_txt);
    printf("File successfully decompressed to \"%s\"\n", out_txt_name);

    list_head = list_free(list_head);
    return;
}

int read_cmd(int count, char* args[], char* file_names[]){
    int ret = -1;
    if(count != 6)
        return -1;

    for(int i = 1; i < count; i++){
        if(!strcmp(args[i], "-c"))
            ret = 1;

        else if(!strcmp(args[i], "-d"))
            ret = 2;

        else if((!strcmp(args[i], "-f")) && !file_names[0]){
            i++;
            file_names[0] = (char*)malloc((strlen(args[i]) + 1) * sizeof(char));
            strcpy(file_names[0], args[i]);
        }
        else if((!strcmp(args[i], "-o")) && !file_names[1]){
            i++;
            file_names[1] = (char*)malloc((strlen(args[i]) + 1) * sizeof(char));
            strcpy(file_names[1], args[i]);
        }
        else{
            return -1;
        }
    }
    return ret;
}

void print_help(){
    printf("---LZ78 Compression & Decompression program: Help---\n"
           "This program uses the LZ78 compression algorithm to compress text files\n"
           "and decompress binary files produced by this program.\n\n"

           "Usage:\n"
           "The program has to be always called with three flags (in any order):\n"
           "\t-c or -d : these flags mean compress and decompress respectively\n"
           "\t-f : after this flag the name of the input file is placed.\n"
           "\t\tThis file should be a .txt file for compressing and a .dat file for decompressing.\n"
           "\t\tThe binary file used for decompressing uses a specific format\n"
           "\t\tgenerated by this program. Attempting to decompress files that have not\n"
           "\t\tbeen generated by this program may yield unspecified results.\n"
           "\t-o : after this flag the name of the output file is placed.\n"
           "\t\tThis file should be a .dat file for compressing and a .txt file for decompressing.\n"
           "\t\tWARNING: If the output file already exists in the current directory its\n"
           "\t\tcontent WILL BE OVERWRITTEN AND LOST.\n\n"
           "Calling this program with any flags not specified above will result in\n"
           "this message being displayed and the program not starting.\n");
}
