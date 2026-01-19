#include "./elfi.h"

ELFIAPI int elfi_getMagic(const char *buffer) {
    /* null-check... */
    if (!buffer) { return (0); }

    if (buffer[EI_MAG0] != ELFMAG0 ||
        buffer[EI_MAG1] != ELFMAG1 ||
        buffer[EI_MAG2] != ELFMAG2 ||
        buffer[EI_MAG3] != ELFMAG3
    ) {
        return (0);
    }
    return (1);
}

ELFIAPI int elfi_getArch(const char *buffer) {
    /* null-check... */
    if (!buffer) { return (0); }

    return (buffer[EI_CLASS]);
}

ELFIAPI char *elfi_shdr_getType(const uint32_t type) {
    switch (type) {
        case (SHT_PROGBITS) : { return ("SHT_PROGBITS"); }
        case (SHT_SYMTAB)   : { return ("SHT_SYMTAB"); }
        case (SHT_STRTAB)   : { return ("SHT_STRTAB"); }
        case (SHT_RELA)     : { return ("SHT_RELA"); }
        case (SHT_HASH)     : { return ("SHT_HASH"); }
        case (SHT_DYNAMIC)  : { return ("SHT_DYNAMIC"); }
        case (SHT_NOTE)     : { return ("SHT_NOTE"); }
        case (SHT_NOBITS)   : { return ("SHT_NOBITS"); }
        case (SHT_REL)      : { return ("SHT_REL"); }
        case (SHT_SHLIB)    : { return ("SHT_SHLIB"); }
        case (SHT_DYNSYM)   : { return ("SHT_DYNSYM"); }
        case (SHT_LOPROC)   : { return ("SHT_LOPROC"); }
        case (SHT_HIPROC)   : { return ("SHT_HIPROC"); }
        case (SHT_LOUSER)   : { return ("SHT_LOUSER"); }
        case (SHT_HIUSER)   : { return ("SHT_HIUSER"); }
        
        default             : { return ("SHT_NULL"); }
    }
}

ELFIAPI void *elfi_extract(const char *buffer, const size_t size, const size_t offset) {
    /* null-check... */
    if (!buffer) { return (0); }

    /* ...alloc... */
    void *result = malloc(size);
    if (!result) {
        perror(g_progName);
        return (0);
    }

    /* ...copy... */
    if (!memcpy(result, buffer + offset, size)) {
        perror(g_progName);
        free(result);
        return (0);
    }

    /* ...return */
    return (result);
}
