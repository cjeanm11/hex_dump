#include "hex_dump.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        return STATUS_ERROR;
    }

    Status result = hex_dump_file(argv[1]);
    if (result != STATUS_OK) {
        fprintf(stderr, "Error: %s\n", status_to_string(result));
    }

    return result;
}
