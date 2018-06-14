#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MASK(M)	(1 << M)
#define DBG_PRINT(str) printf(str); \
						fflush(stdout);
/* Is only going to work on linux / mac */


int crc32(uint8_t* array, int elements) {
	/* calculation here */
	unsigned int crc32_polynomial = 0x04C11DB7;
	unsigned int crc = 0x00000000;
	int xor_enable = 0;
	//uint32_t computevalue;

	for (int i = 0; i < elements; i++) {

		for(int j = 8; j >= 0; j--) {
			if (crc & 0x80000000)
				xor_enable = 1;
			crc <<= 1;
			crc |= MASK(j);
			if (xor_enable)
				crc ^= crc32_polynomial;
			xor_enable = 0;
			//printf("%u", crc);
		}
	}

	return crc;
}

int calculate_crc(uint8_t* array, int length) {
	uint8_t calculateable[length - 8];
	memcpy(calculateable, &array[8], length - 8);
	return crc32(calculateable, length - 8);
}

/* Sample picture from wikipedia
0000000 89 50 4e 47 0d 0a 1a 0a 00 00 00 0d 49 48 44 52
0000010 00 00 00 80 00 00 00 44 08 02 00 00 00 c6 25 aa
0000020 3e 00 00 00 c2 49 44 41 54 78 5e ed d4 81 06 c3
0000030 30 14 40 d1 b7 34 dd ff ff 6f b3 74 56 ea 89 12
0000040 6c 28 73 e2 aa 34 49 03 87 d6 fe d8 7b 89 bb 52
0000050 8d 3b 87 fe 01 00 80 00 00 10 00 00 02 00 40 00
0000060 00 08 00 00 01 00 20 00 00 04 00 80 00 00 10 00
0000070 00 02 00 40 00 00 08 00 00 01 00 20 00 00 00 d4
0000080 5e 6a 64 4b 94 f5 98 7c d1 f4 92 5c 5c 3e cf 9c
0000090 3f 73 71 58 5f af 8b 79 5b ee 96 b6 47 eb f1 ea
00000a0 d1 ce b6 e3 75 3b e6 b9 95 8d c7 ce 03 39 c9 af
00000b0 c6 33 93 7b 66 37 cf ab bf f9 c9 2f 08 80 00 00
00000c0 10 00 00 02 00 40 00 00 08 00 00 01 00 20 00 00
00000d0 04 00 80 00 00 10 00 00 02 00 40 00 00 08 00 00
00000e0 01 00 20 00 00 8c 37 db 68 03 20 fb ed 96 65 00
00000f0 00 00 00 49 45 4e 44 ae 42 60 82               
00000fb
*/

int main(int argc, char** argv) {
	FILE* source;
	FILE* output;
	int width, height, ihdr_crc = 0, data_crc = 0;
	unsigned char* data;
	const uint8_t PNG_HEADER[] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A};
	uint8_t IHDR[] = {
					0x00, 0x00, 0x00, 0x0d, 0x49, 0x48, 0x44, 0x52,
					0x00, 0x00, 0x00, 0x00, 0x00, 0x00 ,0x00 ,0x00,
					0x08, 0x02, 0x00, 0x00, 0x00}; 
					// Length 4 byte, folowed by "IHDR" 
						 //width, height pixels (4bytes each)
	//uint8_t specifications_ihdr[] = {0x08, 0x02, 0x00, 0x00, 0x00 }; // image is 8-bit regular, no compression, adaptive filtering, do not interlace
	uint8_t IDAT[8] = {0x00, 0x00, 0x00, 0x00, 0x49, 0x44, 0x41, 0x54};
	uint8_t IEND[8] = {0x49, 0x45, 0x4e, 0x44, 0xae, 0x42, 0x60, 0x82};

	if (argc < 2) {
		printf("Give width as 1st. parameter and height as 2nd.");
		return 1;
	}
	width = strtoul(argv[1], NULL, 10);
	IHDR[11] = width & 0xFF;
	IHDR[10] = (width & 0xFF00) >> 8;
	IHDR[9] = (width & 0xFF0000) >> 16;
	IHDR[8] = (width & 0xFF000000) >> 24;

	height = strtoul(argv[2], NULL, 10);
	IHDR[15] = height & 0xFF;
	IHDR[14] = (height & 0xFF00) >> 8;
	IHDR[13] = (height & 0xFF0000) >> 16;
	IHDR[12] = (height & 0xFF000000) >> 24;
	
	ihdr_crc = calculate_crc(IHDR, sizeof(IHDR));
	printf("%x", ihdr_crc);
	source = fopen("/dev/urandom", "rb");
	if (source == NULL) {
		printf("Error opening random source");
		return 1;
	}
	output = fopen("pic.png", "wb+");
	if (output == NULL) {
		printf("Error opening png file");
		return 1;
	}
	data = malloc(width * height);
	if (data == NULL) {
		printf("Malloc failed");
		return 1;
	}
	int nr;
	nr = fread(data, 1, width * height, source);
	if (nr < 0) {
		printf("Read failed");
		return 1;
	}

	data_crc = calculate_crc(data, width*height);
	fwrite(PNG_HEADER, 1, sizeof(PNG_HEADER), output); //Write header
	fwrite(IHDR, 1, sizeof(IHDR), output); //Write IHDR
	fwrite(&ihdr_crc, 1, 4, output); //Write IHDR crc
	fwrite(IDAT, 1, sizeof(IDAT), output); //Write IDAT
	fwrite(data, 1, width * height, output); //Write data
	fwrite(&data_crc, 1, sizeof(data_crc), output); //Write data crc
	fwrite(IEND, 1, sizeof(IEND), output); //Write END
	fclose(source);
	fclose(output);

}