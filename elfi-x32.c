#include "./elfi.h"

ELFIAPI int elfi_x32(const char *buffer) {
    /* helper 'exitcode' variable for goto statement... */
    int exitcode = 1;
   
    /* extract ELF header... */
    Elf32_Ehdr *ehdr = elfi_extract(buffer, sizeof(Elf32_Ehdr), 0);
    if (!ehdr) {
        exitcode = 0; goto elfi_x32_exit;
    }

    /* extract ELF section header... */
    Elf32_Shdr *shdr_tb = elfi_extract(buffer, ehdr->e_shnum * ehdr->e_shentsize, ehdr->e_shoff);
    if (!shdr_tb) {
        exitcode = 0; goto elfi_x32_exit;
    }

    /* extract STRTAB's... */
    const char *shstrtab = elfi_extract(buffer, shdr_tb[ehdr->e_shstrndx].sh_size, shdr_tb[ehdr->e_shstrndx].sh_offset);
    const char *dynstr = 0;
    const char *strtab = 0;
    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        /* section header object... */
        Elf32_Shdr shdr = shdr_tb[i];

        if (!strcmp(shstrtab + shdr.sh_name, ".dynstr")) {
            dynstr = elfi_extract(buffer, shdr.sh_size, shdr.sh_offset);
            if (!dynstr) {
                exitcode = 0; goto elfi_x32_exit;
            }
        }

        else if (!strcmp(shstrtab + shdr.sh_name, ".strtab")) {
            strtab = elfi_extract(buffer, shdr.sh_size, shdr.sh_offset);
            if (!strtab) {
                exitcode = 0; goto elfi_x32_exit;
            }
        }
    }

    /* print shdr... */
    printf("Section headers:\n");
    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        /* section header object... */
        Elf32_Shdr shdr = shdr_tb[i];
        
        /* helper variables... */
        const char *sh_name = shstrtab + shdr.sh_name;

        printf(" > %s:\n", sh_name[0] ? sh_name : "...");
        printf("    - Type:   %s\n", elfi_shdr_getType(shdr.sh_type));
        printf("    - Addr:   %016x\n", shdr.sh_addr);
        printf("    - Offset: %d\n", shdr.sh_offset);
        printf("    - Size:   %d\n", shdr.sh_size);
        printf("    - Link:   %d\n", shdr.sh_link);
        printf("    - Info:   %d\n", shdr.sh_info);
        printf("    - Align:  %016x\n", shdr.sh_addralign);

        if (shdr.sh_type == SHT_SYMTAB ||
            shdr.sh_type == SHT_DYNSYM
        ) {
            if (shdr.sh_entsize > 0) {
                /* assign the current string table... */
                const char *tab = 0;
                if (shdr.sh_type == SHT_SYMTAB)      { tab = strtab; }
                else if (shdr.sh_type == SHT_DYNSYM) { tab = dynstr; }

                /* check if string table is valid... */
                if (tab) {
                    printf("\n    Symbol table found for '%s':\n", sh_name);

                    /* symbol table... */
                    Elf32_Sym *sym_tb = elfi_extract(buffer, shdr.sh_size, shdr.sh_offset);
                    if (!sym_tb) {
                        exitcode = 0; goto elfi_x32_exit;
                    }

                    for (size_t j = 0; j < shdr.sh_size / sizeof(Elf32_Sym); j++) {
                        /* symbol table object... */
                        Elf32_Sym sym = sym_tb[j];

                        /* helper variables... */
                        const uint8_t st_bind= ELF32_ST_BIND(sym.st_info);
                        const uint8_t st_type = ELF32_ST_TYPE(sym.st_info);
                        const char *st_name = tab + sym.st_name;

                        printf("     > %s:\n", st_name[0] ? st_name : "...");
                        printf("        - Value: %d\n", sym.st_value);
                        printf("        - Size:  %d\n", sym.st_size);
                        printf("        - Type:  %s\n", elfi_sym_getType(st_type));
                        printf("        - Bind:  %s\n", elfi_sym_getBind(st_bind));
                        printf("        - Other: %d\n", sym.st_other);
                        printf("        - Shndx: %d\n", sym.st_shndx);


                        if (j < shdr.sh_size / sizeof(Elf32_Sym) - 1) { putc(10, stdout); }
                    }

                    free(sym_tb), sym_tb = 0;
                }
            }
        }

        if (i < (size_t) ehdr->e_shnum - 1) { putc(10, stdout); }
    }

elfi_x32_exit:
    if (shstrtab) { free((void *) shstrtab), shstrtab = 0; }
    if (dynstr)   { free((void *) dynstr), dynstr = 0; }
    if (strtab)   { free((void *) strtab), strtab = 0; }
    if (shdr_tb)  { free(shdr_tb), shdr_tb = 0; }
    if (ehdr)     { free(ehdr), ehdr = 0; }
    return (exitcode);
}
