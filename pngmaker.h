/*
 * Header for PNGMaker
 */

#ifndef __PNGMaker_H__
#define __PNGMaker_H__


/*
 * Function for creating random image from /dev/urandom
 * Arguments: None
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on fail
 */
int random_picture();

/*
 * Function for creating random  greyscale image from /dev/urandom
 * Arguments: Size, How many rectangles
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on fail
 */
int random_sized_picture_greyscale(int size);

/*
 * Function for creating random colored image from /dev/urandom
 * Arguments: Size, How many rectangles
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on fail
 */
int random_sized_picture_colorful(int size);

/*
 * Function for initializing png file.
 * Arguments: char** argv, FILE* pic
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on failure
 */
int initialize_png(char**, FILE*);

#endif /* __PNGMaker_H__ */