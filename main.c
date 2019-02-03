#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

const char Base64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

int tobit(char *bufA, int *birr) {
	int size[8]; int y = 0;
	int birar[24];
	int stop = 0;
	for (int j = 0; j < 3; ++j) {
		int i = 0;
		if (bufA[j] == '\n') {
			stop = 1;
		}
		int sim = bufA[j];
		while (i < 8) {
			size[i] = sim % 2;
			sim /= 2;
			i++;
		}
		for (int t = 0; t < 4; ++t) {
			int temp = size[t];
			size[t] = size[7 - t];
			size[7 - t] = temp;
		}
		if (stop != 1) {
			for (int t = 0; t < 8; t++) {
				birar[t + y] = size[t];
			}
		}
		y += 8;
		
	}
	int count = 0;
	for (int i = 0; i < 24; ++i) {
		if ((birar[i] == 1) || (birar[i] == 0))
			count++;
		birr[i] = birar[i];
	}
	return count;
}

void encode(FILE *input, FILE *output) {
	char *buf = malloc(3 * sizeof(char));
	int *bitbuf = malloc(24 * sizeof(int));
	while (!feof(input)) {
		fread(buf, sizeof(char), 3, input);
		int count = tobit(buf, bitbuf);
		int e = 0;
		for (e ; e < count;) {
			int sum = 0;
			for (int i = 0; i < 6; ++i) {
				sum = (sum * 2) + bitbuf[i + e];
			}
			e += 6;
			if (e > count) {
				printf("=");
				break;
			}

			printf("%c", Base64[sum]);
			//fprintf(output,"%c", Base64[sum]);
			
		}
		if (e > count) {
			break;
		}
	}


}
 
void decode(FILE *input, FILE *output) {
	char *buf = malloc(3 * sizeof(char));
	int *bitbuf = malloc(24 * sizeof(int));
	while (!feof(input)) {
		fread(buf, sizeof(char), 4, input);
		
		int y = 0;
		int birar[24]= {8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8,8}; 
		int sim;
		for (int j = 0; j < 4; ++j) {
			int size[6] = { 8,8,8,8,8,8 };
			int i = 0; int r = 0;
			if ('\n' == buf[j]) {
				j++;
			}
			for (r; r <= 64; r++) {
				if (Base64[r] == buf[j]) {
					sim = r;
					break;
				}

			}
			
			while (i < 6) {
				size[i] = sim % 2;
				sim /= 2;
				i++;
			}
			for (int t = 0; t < 3; ++t) {
				int temp = size[t];
				size[t] = size[5 - t];
				size[5 - t] = temp;
			}
			
				for (int t = 0; t < 6; t++) {
					birar[t + y] = size[t];
				}
			y += 6;

		}
		int e = 0;
		for (e; e < 24;) {
			int sum = 0;
			for (int i = 0; i < 8; ++i) {
				sum = (sum * 2) + birar[i + e];
			}
			e += 8;
			//unsigned char sym = sum;
			printf("%c", sum);
		}
	}
}

int main(int argc, char argv) {
	FILE *in = fopen("in.txt", "r");
	FILE *out = fopen("out.txt", "r");

	encode(in, out);
	printf("\n\n");
	decode(out, out);

	fclose(in);
	fclose(out);
	return 0;
}