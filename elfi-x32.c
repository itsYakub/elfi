#include "./elfi.h"

ELFIAPI int elfi_x32(const char *buffer, const size_t size) {
    (void) size;

    /* helper 'exitcode' variable for goto statement... */
    int exitcode = 1;
    
    /* extract ELF header... */
    Elf64_Ehdr ehdr = { 0 };
    if (!memcpy(&ehdr, buffer, sizeof(Elf64_Ehdr))) {
        return (0);
    }

    /* allocate section header table... */
    Elf32_Shdr *shdr_tb = malloc(ehdr.e_shnum * ehdr.e_shentsize);
    if (!shdr_tb) {
        perror(g_progName);
        exitcode = 0; goto elfi_x32_exit;
    }
    if (!memcpy(shdr_tb, buffer + ehdr.e_shoff, ehdr.e_shnum * ehdr.e_shentsize)) {
        perror(g_progName);
        exitcode = 0; goto elfi_x32_exit;
    }

    /* iterate over section headers... */
    for (size_t i = 0; i < ehdr.e_shnum; i++) {
        Elf32_Shdr shdr = shdr_tb[i];
        if (shdr.sh_type == SHT_NULL ||
            shdr.sh_type == SHT_NOBITS
        ) {
            continue;
        }

        Elf32_Sym *sym_tb = malloc(shdr.sh_size);
        if (!sym_tb) {
            perror(g_progName);
            exitcode = 0; goto elfi_x32_exit;
        }
        if (!memcpy(sym_tb, buffer + shdr.sh_offset, shdr.sh_size)) {
            perror(g_progName);
            exitcode = 0; goto elfi_x32_exit;
        }


        /* iterate over section symbols... */
        for (size_t j = 0; j < shdr.sh_size / sizeof(Elf32_Sym); j++) {
            Elf32_Sym sym = sym_tb[j];
            uint8_t sym_type = ELF32_ST_TYPE(sym.st_info);

            if (sym_type == STT_NOTYPE) {
                continue;
            }

            /* ... */
        }

        if (sym_tb) { free(sym_tb), sym_tb = 0; }
    }

elfi_x32_exit:
    if (shdr_tb) { free(shdr_tb), shdr_tb = 0; }
    return (exitcode);
}
