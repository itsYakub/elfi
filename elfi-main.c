#include "./elfi.h"

int main(int ac, char **av) {
    /* validate argument count */
    if (ac == 1) {
        printf("%s [ OPTIONS...] [ FILES... ]\n", g_progName);
        return (1);
    }

    for (size_t i = 1; i < (size_t) ac; i++) {
        /* process options... */
        if (*av[i] == '-') { }

        /* process files... */
        else {
            /* setup... */
            int fd = open(av[i], O_RDONLY);
            if (fd == -1) {
                perror(g_progName);
                return (1);
            }

            struct stat stat = { 0 };
            if (fstat(fd, &stat) == -1) {
                perror(g_progName);
                close(fd);
                return (1);
            }

            const char *buffer = mmap(0, stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (!buffer) {
                perror(g_progName);
                close(fd);
                return (1);
            }

            /* ...execute... */
            if (!elfi_processFile(buffer, stat.st_size)) {
                perror(g_progName);
                munmap((void *) buffer, stat.st_size);
                close(fd);
                return (1);
            }

            /* ...cleanup */
            munmap((void *) buffer, stat.st_size);
            close(fd);
        }
    }

    return (0);
}

ELFIAPI int elfi_processFile(const char *buffer, const size_t size) {
    if (!buffer) { return (0); }
    if (!size)   { return (0); }

    ElfEhdr ehdr = { 0 };
    if (!memcpy(&ehdr, buffer, sizeof(ElfEhdr))) {
        perror(g_progName);
        return (0);
    }

    if (ehdr.e_ident[EI_MAG0] != ELFMAG0 ||
        ehdr.e_ident[EI_MAG1] != ELFMAG1 ||
        ehdr.e_ident[EI_MAG2] != ELFMAG2 ||
        ehdr.e_ident[EI_MAG3] != ELFMAG3
    ) {
        printf("%s: not an ELF file\n", g_progName);
        return (0);
    }

    /* get an architecture of the binary based on e_ident[EI_CLASS]... */
    switch (ehdr.e_ident[EI_CLASS]) {
        case (ELFCLASS32) : { return (elfi_x32(&ehdr, buffer, size)); }
        case (ELFCLASS64) : { return (elfi_x64(&ehdr, buffer, size)); }

        default : {
            printf("%s: invalid file architecture\n", g_progName);
            return (0);
        };
    }
}

/* SECTION: global variables and constants */

const char *g_progName = "elfi"; /* g_progName - name of the executable */
