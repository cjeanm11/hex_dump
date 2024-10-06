#include "hex_dump.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char* argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Error: Filename is required.\n");
        print_help();
        return 1;
    }

    const char* filename = argv[1];
    OutputFormat format = FORMAT_HEX;
    long line_width = 16;
    const char* output_file = NULL;
    int detailed = 0;
    long max_size = -1;
    int filter_printable = 0;

    for (int i = 2; i < argc; i++) {
        if (strcmp(argv[i], "-l") == 0) {
            if (i + 1 < argc) {
                char* endptr;
                line_width = strtol(argv[++i], &endptr, 10);

                if (*endptr != '\0' || line_width < 1 || line_width > 16) {
                    fprintf(stderr, "Error: line width must be between 1 and 16.\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "Error: -l option requires a value.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
            } else {
                fprintf(stderr, "Error: -o option requires a filename.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-d") == 0) {
            detailed = 1;
        } else if (strcmp(argv[i], "-m") == 0) {
            if (i + 1 < argc) {
                char* endptr;
                max_size = strtol(argv[++i], &endptr, 10);

                if (*endptr != '\0' || max_size < 0) {
                    fprintf(stderr, "Error: max size must be a non-negative integer.\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "Error: -m option requires a value.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-f") == 0) {
            filter_printable = 1;
        } else if (strcmp(argv[i], "-fmt") == 0) {
            if (i + 1 < argc) {
                if (strcmp(argv[++i], "hex") == 0) {
                    format = FORMAT_HEX;
                } else if (strcmp(argv[i], "binary") == 0) {
                    format = FORMAT_BINARY;
                } else if (strcmp(argv[i], "octal") == 0) {
                    format = FORMAT_OCTAL;
                } else if (strcmp(argv[i], "decimal") == 0) {
                    format = FORMAT_DECIMAL;
                } else {
                    fprintf(stderr, "Error: Unknown format: %s\n", argv[i]);
                    return 1;
                }
            } else {
                fprintf(stderr, "Error: -fmt option requires a format type.\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0) {
            print_help();
            return 0;
        } else {
            fprintf(stderr, "Error: Unknown option: %s\n", argv[i]);
            print_help();
            return 1;
        }
    }

    Status status = hex_dump_file(filename, format, line_width, output_file, detailed, max_size, filter_printable);
    if (status != STATUS_OK) {
        fprintf(stderr, "Error: %s\n", status_to_string(status));
        return 1;
    }

    return 0;
}