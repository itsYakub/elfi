#include "./elfi.h"

ELFIAPI int elfi_x64(ElfEhdr *ehdr, const char *buffer, const size_t size) {
    if (!ehdr) { return (0); }

    (void) ehdr;
    (void) buffer;
    (void) size;
    return (1);
}
