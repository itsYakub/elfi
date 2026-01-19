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

            const char *buffer = malloc(stat.st_size);
            if (!buffer) {
                perror(g_progName);
                close(fd);
                return (1);
            }
            if (read(fd, (void *) buffer, stat.st_size) == -1) {
                perror(g_progName);
                free((void *) buffer);
                close(fd);
                return (1);
            }

            /* ...execute... */
            if (!elfi_processFile(buffer, stat.st_size)) {
                free((void *) buffer);
                close(fd);
                return (1);
            }

            /* ...cleanup */
            free((void *) buffer);
            close(fd);
        }
    }

    return (0);
}

ELFIAPI int elfi_processFile(const char *buffer, const size_t size) {
    if (!buffer) { return (0); }
    if (!size)   { return (0); }

    /* check ELF header... */
    if (!elfi_getMagic(buffer)) {
        printf("%s: not an ELF file\n", g_progName);
        return (0);
    }

    /* check ELF arch... */
    switch (elfi_getArch(buffer)) {
        case (ELFCLASS32) : { return (elfi_x32(buffer)); }
        case (ELFCLASS64) : { return (elfi_x64(buffer)); }

        default : {
            printf("%s: invalid file architecture\n", g_progName);
            return (0);
        };
    }
}

/* SECTION: global variables and constants */

const char *g_progName = "elfi"; /* g_progName - name of the executable */
