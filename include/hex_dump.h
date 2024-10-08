//
// Created by Jean-Michel on 2024-10-06.
//

#ifndef HEX_DUMP_HEX_DUMP_H
#define HEX_DUMP_HEX_DUMP_H

typedef enum {
    STATUS_OK = 0,
    STATUS_ERROR = 1,
    STATUS_FILE_NOT_FOUND = 2,
    STATUS_FILE_READ_ERROR = 3,
    STATUS_INVALID_ARGUMENT = 4,
    STATUS_MEMORY_ALLOCATION_ERROR = 5,
    STATUS_PERMISSION_DENIED = 6
} Status;

typedef enum {
    FORMAT_HEX,
    FORMAT_BINARY,
    FORMAT_OCTAL,
    FORMAT_DECIMAL
} OutputFormat;

const char*
status_to_string(Status status);

Status
print_hex(const unsigned char* const buffer, int length, int offset, OutputFormat format);

Status
hex_dump_file(
    const char* filename,
    OutputFormat format,
    int line_width,
    const char* output_file,
    int detailed,
    int max_size,
    int filter_printable
);

void
print_help();

#endif // HEX_DUMP_HEX_DUMP_H