#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// BASE64 encoding table
static const char base64_enc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

// BASE64 decoding table (initialized in main)
static char base64_dec[256];

void init_decode_table() {
	memset(base64_dec, 0, 256);
	for (int i = 0; i < 64; i++) {
		base64_dec[(unsigned char)base64_enc[i]] = i;
	}
}

void encode_base64() {
	unsigned char in[3], out[4];
	int i, len, count = 0;
	
	while ((len = fread(in, 1, 3, stdin)) > 0) {
		out[0] = base64_enc[in[0] >> 2];
		out[1] = base64_enc[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
		out[2] = (len > 1) ? base64_enc[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : '=';
		out[3] = (len > 2) ? base64_enc[in[2] & 0x3f] : '=';
		
		fwrite(out, 1, 4, stdout);
		count += 4;
		if (count >= 76) { // Standard line length for BASE64
			putchar('\n');
			count = 0;
		}
	}
	if (count > 0) putchar('\n');
}

void decode_base64() {
	unsigned char in[4], out[3];
	int i, len;
	
	while ((len = fread(in, 1, 4, stdin)) == 4) {
		// Skip whitespace
		for (i = 0; i < 4; i++) {
			while (in[i] == '\n' || in[i] == '\r') {
				in[i] = fgetc(stdin);
			}
			if (in[i] == EOF) return;
		}
		
		// Check for padding
		int pad = 0;
		if (in[2] == '=') pad = 2;
		else if (in[3] == '=') pad = 1;
		
		// Decode
		out[0] = (base64_dec[in[0]] << 2) | (base64_dec[in[1]] >> 4);
		out[1] = ((base64_dec[in[1]] & 0x0f) << 4) | (base64_dec[in[2]] >> 2);
		out[2] = ((base64_dec[in[2]] & 0x03) << 6) | base64_dec[in[3]];
		
		fwrite(out, 1, 3 - pad, stdout);
	}
}

int main(int argc, char *argv[]) {
	if (argc != 2 || (strcmp(argv[1], "encode") != 0 && strcmp(argv[1], "decode") != 0)) {
		fprintf(stderr, "Usage: %s [encode|decode]\n", argv[0]);
		return 1;
	}
	
	if (strcmp(argv[1], "decode") == 0) {
		init_decode_table();
		decode_base64();
	} else {
		encode_base64();
	}
	
	return 0;
}
