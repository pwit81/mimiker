#ifndef _SYS_MMAN_H_
#define _SYS_MMAN_H_

#include <stdint.h>

#define MAP_FAILED ((void *)-1)

#ifndef _KERNELSPACE

#define MAP_FILE 0
#define MAP_ANON 1
#define MAP_SHARED 2
#define MAP_PRIVATE 4
#define MAP_FIXED 8
#define MAP_STACK 16

#define PROT_NONE 0
#define PROT_READ 1
#define PROT_WRITE 2
#define PROT_EXEC 4

/* Newlib does not provide mmap prototype, so we need to use our own. */
void *mmap(void *addr, size_t length, int prot, int flags);
int munmap(void *addr, size_t len);
int mprotect(void *addr, size_t len, int prot);

#else /* _KERNELSPACE */

#include <vm.h>

int do_mmap(vaddr_t *addr_p, size_t length, vm_prot_t prot, vm_flags_t flags);
int do_munmap(vaddr_t addr, size_t length);

#endif /* !_KERNELSPACE */

#endif /* !_SYS_MMAN_H_ */
