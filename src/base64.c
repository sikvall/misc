#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

const char base64_table[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

void base64_encode(FILE *in, FILE *out) {
    unsigned char input[3];
    unsigned char output[4];
    size_t bytes_read;
    int line_length = 0;

    while ((bytes_read = fread(input, 1, 3, in)) > 0) {
        output[0] = base64_table[input[0] >> 2];
        output[1] = base64_table[((input[0] & 0x03) << 4) | (input[1] >> 4)];
        output[2] = (bytes_read > 1) ? base64_table[((input[1] & 0x0F) << 2) | (input[2] >> 6)] : '=';
        output[3] = (bytes_read > 2) ? base64_table[input[2] & 0x3F] : '=';

        for (int i = 0; i < 4; i++) {
            fputc(output[i], out);
            line_length++;
            if (line_length >= 72) {
                fputc('\n', out);
                line_length = 0;
            }
        }
    }

    if (line_length > 0) {
        fputc('\n', out);  // Final newline
    }
}

void base64_decode(FILE *in, FILE *out) {
    unsigned char input[4];
    unsigned char output[3];
    size_t bytes_read;
    int value;

    while ((bytes_read = fread(input, 1, 4, in)) > 0) {
        size_t len = 0;

        for (int i = 0; i < bytes_read; i++) {
            if (isspace(input[i]) || input[i] == '\n' || input[i] == '\r') {
                continue;
            }
            if (input[i] == '=') {
                break;
            }
            len++;
            input[len - 1] = input[i];
        }

        if (len < 4) {
            break;
        }

        value = (strchr(base64_table, input[0]) - base64_table) << 18 |
                (strchr(base64_table, input[1]) - base64_table) << 12 |
                (strchr(base64_table, input[2]) - base64_table) << 6 |
                (strchr(base64_table, input[3]) - base64_table);

        output[0] = (value >> 16) & 0xFF;
        output[1] = (value >> 8) & 0xFF;
        output[2] = value & 0xFF;

        for (int i = 0; i < 3; i++) {
            if (input[i + 2] == '=') {
                break;
            }
            fputc(output[i], out);
        }
    }
}

int main() {
    FILE *input = fopen("input.txt", "rb");
    FILE *encoded = fopen("encoded.txt", "wb");
    FILE *decoded = fopen("decoded.txt", "wb");

    if (!input || !encoded || !decoded) {
        perror("Error opening files");
        return 1;
    }

    base64_encode(input, encoded);
    fclose(encoded);

    FILE *encoded_for_decoding = fopen("encoded.txt", "rb");
    base64_decode(encoded_for_decoding, decoded);
    fclose(input);
    fclose(encoded_for_decoding);
    fclose(decoded);

    printf("Base64 encoding and decoding completed.\n");
    return 0;
}


