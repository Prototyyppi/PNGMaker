/*
 * Header for PNGMaker
 */

#ifndef __PNGMaker_H__
#define __PNGMaker_H__


/*
 * Function for creating random image from /dev/urandom
 * Arguments: None
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on failed
 */
int random_picture();

/*
 * Function for initializing png file.
 * Arguments: char** argv, FILE* pic
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on failed
 */
int initialize_png(char**, FILE*);

#endif /* __PNGMaker_H__ */