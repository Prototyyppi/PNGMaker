#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <png.h>
#include "pngmaker.h"

/* Global variables, that are needed often */
png_structp png_ptr;
png_infop info_ptr;
png_uint_32 width, height;

/*
 * Function for initializing png file.
 * Arguments: char** argv, FILE* pic
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on failed
 */
int initialize_png(char** argv, FILE* pic) {

	width = (png_uint_32)strtoul(argv[1], NULL, 10);
	if (width == 0){
		printf("Error with width parameter");
		return EXIT_FAILURE;
	}

	height = (png_uint_32)strtoul(argv[2], NULL, 10);
	if (height == 0) {
		printf("Error with height parameter");
		return EXIT_FAILURE;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error with create stuct\n");
		return EXIT_FAILURE;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error with init io\n");
		return EXIT_FAILURE;
	}
	png_init_io(png_ptr, pic);

	png_set_IHDR(png_ptr, info_ptr, width, height, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
						PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	if (setjmp(png_jmpbuf(png_ptr))) {
		fprintf(stderr, "Error with write info\n");
		return EXIT_FAILURE;
	}
	png_write_info(png_ptr, info_ptr);

	return EXIT_SUCCESS;
}

/*
 * Function for creating random image from /dev/urandom
 * Arguments: None
 * Return value: EXIT_SUCCESS on success, EXIT_FAILURE on failed
 */
int random_picture() {
	FILE* source;
	png_bytep row;

	row = malloc(3 * width * sizeof(png_byte));
	if (row == NULL) {
		printf("Malloc failed");
		return EXIT_FAILURE;
	}

	source = fopen("/dev/urandom", "rb");
	if (source == NULL) {
		printf("Error opening random source");
		return EXIT_FAILURE;
	}

	for (int i=0 ; i<height ; i++) {
		fread(row, 1, width*3, source);
		png_write_row(png_ptr, row);
	}

	png_write_end(png_ptr, NULL);
	if (source != NULL)
		fclose(source);
	if (row != NULL)
		free(row);
	return EXIT_SUCCESS;
}

int main(int argc, char** argv) {
	int ret, mode = 1;
	
	FILE* pic;

	if (argc <= 2) {
		printf("Help:\n%s [width] [height] (optional)[mode] \n", argv[0]);
		return EXIT_FAILURE;
	}
	pic = fopen("pic.png", "wb");
	ret = initialize_png(argv, pic);
	if (ret != EXIT_SUCCESS)
		EXIT_FAILURE;


	if (argc >= 3) {
		mode = strtoul(argv[2], NULL, 10);
		if (mode == 0) {
			printf("Error with mode parameter");
			return EXIT_FAILURE;
		}
	}

	switch(mode) {
		case 1:
			random_picture();
			break;
		default:
			random_picture();
			break;
	}

	if (pic)
		fclose(pic);
	if (info_ptr != NULL)
		png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL)
		png_destroy_write_struct(&png_ptr, (png_infopp)NULL);


}