#if !defined (_elfi_h_)
# define _elfi_h_ 1
#
# include <stdio.h>
# include <stddef.h>
# include <stdint.h>
# include <stdlib.h>
# include <string.h>
# include <elf.h>
# include <fcntl.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/mman.h>
#
# if !defined (ELFIAPI)
#  define ELFIAPI extern
# endif /* ELFIAPI */

/* SECTION: global variables and constants */

extern const char *g_progName; /* g_progName - name of the executable */

/* elfi-main.c */

ELFIAPI int main(int, char **);
ELFIAPI int elfi_processFile(const char *, const size_t);

/* elfi-x32.c */

ELFIAPI int elfi_x32(const char *, const size_t);

/* elfi-x64.c */

ELFIAPI int elfi_x64(const char *, const size_t);

/* elfi-util.c */

ELFIAPI int elfi_getMagic(const char *);
ELFIAPI int elfi_getArch(const char *);

ELFIAPI char *elfi_shdr_getType(const uint32_t);

ELFIAPI void *elfi_extract(const char *, const size_t, const size_t);

#endif /* _elfi_h_ */
