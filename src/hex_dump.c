#include "hex_dump.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

const char*
status_to_string(Status status) {
    switch (status) {
        case STATUS_OK:
            return "Success";
        case STATUS_ERROR:
            return "General error";
        case STATUS_FILE_NOT_FOUND:
            return "File not found";
        case STATUS_FILE_READ_ERROR:
            return "File read error";
        case STATUS_INVALID_ARGUMENT:
            return "Invalid argument";
        case STATUS_MEMORY_ALLOCATION_ERROR:
            return "Memory allocation error";
        case STATUS_PERMISSION_DENIED:
            return "Permission denied";
        default:
            return "Unknown status";
    }
}

Status
print_hex(const unsigned char* const buffer, int length, int offset, OutputFormat format) {
    if (buffer == NULL || length < 0) {
        return STATUS_INVALID_ARGUMENT;
    }

    printf("%08x: ", offset);

    for (int i = 0; i < length; i++) {
        switch (format) {
            case FORMAT_HEX:
                printf("%02x ", buffer[i]);
                break;
            case FORMAT_BINARY:
                for (int j = 7; j >= 0; j--) {
                    printf("%d", (buffer[i] >> j) & 1);
                }
                printf(" ");
                break;
            case FORMAT_OCTAL:
                printf("%03o ", buffer[i]);
                break;
            case FORMAT_DECIMAL:
                printf("%03d ", buffer[i]);
                break;
        }
    }

    for (int i = length; i < 16; i++) {
        printf("    ");
    }

    printf(" ");
    for (int i = 0; i < length; i++) {
        printf("%c", (buffer[i] >= 32 && buffer[i] <= 126) ? buffer[i] : '.');
    }

    printf("\n");
    return STATUS_OK;
}

Status
hex_dump_file(
    const char* const filename,
    OutputFormat format,
    int line_width,
    const char* const output_file,
    int detailed,
    int max_size,
    int filter_printable
) {
    if (filename == NULL || line_width <= 0 || line_width > 16) {
        return STATUS_INVALID_ARGUMENT;
    }

    FILE* file = fopen(filename, "rb");
    if (!file) {
        if (errno == ENOENT) {
            return STATUS_FILE_NOT_FOUND;
        } else if (errno == EACCES) {
            return STATUS_PERMISSION_DENIED;
        } else {
            return STATUS_ERROR;
        }
    }

    FILE* out_file = stdout;
    if (output_file != NULL) {
        out_file = fopen(output_file, "w");
        if (!out_file) {
            fclose(file);
            return STATUS_PERMISSION_DENIED;
        }
    }

    unsigned char buffer[16];
    int offset = 0;
    int bytesRead;
    int total_bytes_read = 0;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        if (max_size > 0 && total_bytes_read + bytesRead > max_size) {
            bytesRead = max_size - total_bytes_read;
        }

        if (filter_printable) {
            for (int i = 0; i < bytesRead; i++) {
                if (buffer[i] < 32 || buffer[i] > 126) {
                    buffer[i] = '.';
                }
            }
        }

        print_hex(buffer, bytesRead, offset, format);
        offset += bytesRead;
        total_bytes_read += bytesRead;

        if (max_size > 0 && total_bytes_read >= max_size) {
            break;
        }
    }

    if (ferror(file)) {
        fclose(file);
        if (out_file != stdout) fclose(out_file);
        return STATUS_FILE_READ_ERROR;
    }

    fclose(file);
    if (out_file != stdout) fclose(out_file);
    return STATUS_OK;
}

void
print_help() {
    printf("Usage: hex_dump <filename> [options]\n");
    printf("Options:\n");
    printf("  -l <line_width>     Number of bytes to display per line (1-16).\n");
    printf("  -o <output_file>    Specify an output file for the hex dump.\n");
    printf("  -d                  Enable detailed output.\n");
    printf("  -m <max_size>       Maximum bytes to read from the file.\n");
    printf("  -f                  Filter non-printable characters.\n");
    printf("  -fmt <format>       Output format: hex, binary, octal, decimal.\n");
    printf("  -h                  Display this help message.\n");
}
