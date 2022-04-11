#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "structures.h"

/** @file functions.h
 *  @author Radoslaw Rzeczkowski
 *  @date 06.2020
 *  @brief A header file containing the functions used by the program.
 */

/** @fn struct list* list_add(struct list* head, unsigned int _id, char _entry, char* _str, unsigned int _key)
 *  @brief Adds an element to a list of list structs.
 *  @param head A pointer to the list.
 *  @param _id The key of the linked entry.
 *  @param _entry The character of the entry.
 *  @param _str The string that has been encoded using this entry.
 *  @param _key The key of this entry.
 *  @return @b Caution. Returns the head pointer of the list only for the first element added.
 *  For all other elements returns a pointer to the current element in order to save time adding new elements.
 *  @warning See Returns.
*/
struct list* list_add(struct list* head, unsigned int _id, char _entry, char* _str, unsigned int _key);

/** @fn struct list* list_free(struct list* head)
 *  @brief Frees the memory allocated to the list.
 *  @param head The head pointer of the list to be freed.
 *  @return A null pointer.
*/
struct list* list_free(struct list* head);

/** @fn void list_print(struct list* head)
 *  @brief Prints the whole list to the console (used for debugging).
 *  @param head The head pointer of the list to be printed.
*/
void list_print(struct list* head);

/** @fn struct list* list_search_string(struct list* head, char* _str)
 *  @brief Searches a list linearly for a given string and
 *  returns a pointer to the element containing the string.
 *  @param head The head pointer of the list to be searched.
 *  @param _str The string to be found.
 *  @return A pointer to the element containing the given string in
 *  its str field. When not found returns a null pointer.
*/
struct list* list_search_string(struct list* head, char* _str);

/** @fn void init_bin(FILE* out_bin, unsigned int count)
 *  @brief Initializes the binary output file in which
 *  the compressed input file will be stored.
 *
 *  The output binary file is initialized by writing at the beginning
 *  "LZ78" using 3 bytes followed by the amount of entries that will be
 *  stored in this file using 4 bytes.
 *  @param out_bin The pointer to the stream which will be initialized.
 *  @param count The amount of entries to be stored in the file.
*/
void init_bin(FILE* out_bin, unsigned int count);

/** @fn void list_write_bin(FILE* out_bin, struct list* head)
 *  @brief Writes the whole list pointed by its head pointer
 *  as binary data to a file.
 *
 *  From each entry of the list only the id and entry fields are written to the file.
 *  The entry always uses one byte (it's a character) but the id uses the minimal
 *  amount of bytes in which it can be stored. For example it can use one byte for
 *  values up to 255 by saving the value as an unsigned char datatype and above that value
 *  start using two bytes in unsigned short. The exact max values of each datatype are included
 *  in the limits.h header. This is done to achieve high compression ratios.
 *  @param out_bin The pointer to the stream in which the data will be written.
 *  @param head The head pointer of the list to be written in a file.
*/
void list_write_bin(FILE* out_bin, struct list* head);

/** @fn void compress(char* in_txt_name, char* out_bin_name)
 *  @brief Coordinates the whole compression process.
 *
 *  This function opens the file streams, reads the input file, calls other
 *  functions to construct the LZ78 dictionary, print the compressed data
 *  to a given output file and free the allocated memory leaving no memory leaks.
 *  @param in_txt_name The name of the text input file.
 *  @param out_bin_name The name of the output binary file.
*/
void compress(char* in_txt_name, char* out_bin_name);

/** @fn unsigned int check_input(FILE* in_bin)
 *  @brief Checks the format of the input binary file and reads the amount of entries saved in it.
 *
 *  This function checks the correctness of the format of the input binary file
 *  and reads the amount of entries to be read from it.
 *  The compressed file should start with "LZ78" saved in the first 3 bytes. That should
 *  be followed by the number of entries to be read from the file saved in the next 4 bytes.
 *  That is exactly the way that @see init_bin initializes the binary file prior to output of
 *  the compressed data.
 *  @param in_bin The pointer to the stream from which data will be read.
 *  @return The amount of entries (character and numerical variable pair) to be read from the file.
*/
unsigned int check_input(FILE* in_bin);

/** @fn struct list* list_search_key(struct list* head, unsigned int _key)
 *  @brief Searches the list for a given numerical key and returns a pointer to
 *  the element containing it.
 *  @param head The head pointer of the list to be searched.
 *  @param _key The value of key that will be searched for.
 *  @return The pointer to the element that contains the _key value
 *  in its key field.
*/
struct list* list_search_key(struct list* head, unsigned int _key);

/** @fn void list_write_txt(FILE* out_txt, struct list* head)
 *  @brief Writes the decompressed data to the output text file.
 *
 *  This function writes the str field from each element of the list
 *  into a text file. This makes up the entirety of the decoded data
 *  and the resulting file is the same as the original file that was compressed.
 *  @param out_text The pointer to the file in which the text will be written.
 *  @param head The head pointer of the list to be written into a file.
*/
void list_write_txt(FILE* out_txt, struct list* head);

/** @fn void decompress(char* in_bin_name, char* out_txt_name)
 *  @brief Coordinates the whole process of decompressing a file.
 *
 *  This function opens the file streams, reads the input file, calls other
 *  functions to reconstruct the LZ78 dictionary, print the decompressed data
 *  to a given output file and free the allocated memory leaving no memory leaks.
 *  @param in_bin_name The name of the binary input file.
 *  @param out_txt_name The name of the output text file.
*/
void decompress(char* in_bin_name, char* out_txt_name);

/** @fn int read_cmd(int count, char* args[], char* file_names[])
 *  @brief Reads the command line arguments and processes them.
 *
 *  This function reads the command line arguments passed to the program,
 *  checks their correctness and in case of any wrong/insufficient input
 *  causes the Help for the program to be displayed in console.
 *  @param count The amount of arguments passed to the program (argc).
 *  @param args The arguments passed to the program (argv).
 *  @param[out] file_names The array of two strings in which the read file names
 *  will be saved (0 for input file name, 1 for output file name).
 *  @return The action to be taken by the program. For 1 compression,
 *  for 2 decompression and for -1 printing of the Help due to wrong arguments.
*/
int read_cmd(int count, char* args[], char* file_names[]);

/** @fn void print_help()
 *  @brief Prints the Help for the program to the console.
*/
void print_help();

#endif
