#ifndef STRUCTURES_H
#define STRUCTURES_H

/** @file structures.h
 *  @author Radoslaw Rzeczkowski
 *  @date 06.2020
 *  @brief A header file containing the structures used by the program.
 */

/** @struct list
 *  @brief A singly linked list structure representing a LZ78 dictionary entry.
*/
struct list{
    /**@{ */
    unsigned int id;        /**< The key of the linked entry.*/
    char entry;             /**< The character in this entry.*/
    unsigned int key;       /**< The key of this entry.*/
    char* str;              /**< The string encoded with this entry.*/
    struct list *p_next;    /**< The pointer to the next entry in the dictionary.*/
    /**@} */
};

#endif
