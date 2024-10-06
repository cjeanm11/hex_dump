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
print_hex(unsigned char* buffer, int length, int offset) {
    if (buffer == NULL) {
        return STATUS_INVALID_ARGUMENT;
    }

    if (length < 0 || length > 16) {
        return STATUS_INVALID_ARGUMENT;
    }

    printf("%08x: ", offset);

    for (int i = 0; i < length; i++) {
        printf("%02x ", buffer[i]);
    }

    for (int i = length; i < 16; i++) {
        printf("   ");
    }

    printf(" ");
    for (int i = 0; i < length; i++) {
        if (buffer[i] >= 32 && buffer[i] <= 126) {
            printf("%c", buffer[i]);
        } else {
            printf(".");
        }
    }
    printf("\n");

    return STATUS_OK;
}

Status
hex_dump_file(const char* filename) {
    if (filename == NULL) {
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

    unsigned char buffer[16];
    int offset = 0;
    int bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        Status status = print_hex(buffer, bytesRead, offset);
        if (status != STATUS_OK) {
            fclose(file);
            return status;
        }
        offset += bytesRead;
    }

    if (ferror(file)) {
        fclose(file);
        return STATUS_FILE_READ_ERROR;
    }

    fclose(file);
    return STATUS_OK;
}