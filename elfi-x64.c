#include "./elfi.h"

ELFIAPI int elfi_x64(const char *buffer, const size_t size) {
    (void) size;

    /* helper 'exitcode' variable for goto statement... */
    int exitcode = 1;
   
    /* extract ELF header... */
    Elf64_Ehdr *ehdr = elfi_extract(buffer, sizeof(Elf64_Ehdr), 0);
    if (!ehdr) {
        exitcode = 0; goto elfi_x64_exit;
    }

    /* extract ELF section header... */
    Elf64_Shdr *shdr_tb = elfi_extract(buffer, ehdr->e_shnum * ehdr->e_shentsize, ehdr->e_shoff);
    if (!shdr_tb) {
        exitcode = 0; goto elfi_x64_exit;
    }

    /* extract .strtab... */
    Elf64_Shdr shdr_strtab = shdr_tb[ehdr->e_shstrndx];
    const char *strtab = elfi_extract(buffer, shdr_strtab.sh_size, shdr_strtab.sh_offset);
    if (!strtab) {
        exitcode = 0; goto elfi_x64_exit;
    }

    /* print shdr... */
    printf("Section headers:\n");
    for (size_t i = 0; i < ehdr->e_shnum; i++) {
        Elf64_Shdr shdr = shdr_tb[i];

        printf(" [ %03zu ] %s:\n", i, strtab + shdr.sh_name);
        printf("         Type:   %s\n", elfi_shdr_getType(shdr.sh_type));
        printf("         Addr:   %016lx\n", shdr.sh_addr);
        printf("         Offset: %ld\n", shdr.sh_offset);
        printf("         Size:   %ld\n", shdr.sh_size);
        printf("         Link:   %d\n", shdr.sh_link);
        printf("         Info:   %d\n", shdr.sh_info);
        printf("         Align:  %016lx\n", shdr.sh_addralign);
        
        if (i < (size_t) ehdr->e_shnum - 1) { putc(10, stdout); }
    }

elfi_x64_exit:
    if (strtab)  { free((void *) strtab), strtab = 0; }
    if (shdr_tb) { free(shdr_tb), shdr_tb = 0; }
    if (ehdr)    { free(ehdr), ehdr = 0; }
    return (exitcode);
}
