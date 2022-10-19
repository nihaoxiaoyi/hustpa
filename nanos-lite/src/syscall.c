#include "common.h"
#include "syscall.h"

/* Start */

#include "proc.h"

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);
void naive_uload(PCB *pcb, const char *filename);

/* End */

_Context* do_syscall(_Context *c) {
  uintptr_t a[4];
  a[0] = c->GPR1;
  /* Start */
 
  a[1] = c->GPR2;
  a[2] = c->GPR3;
  a[3] = c->GPR4;

  int ret = 0;
 
  /* End */

  switch (a[0]) {
    /* Start */
    
    case SYS_yield : _yield(); break;
    case SYS_exit  : naive_uload(NULL, "/bin/init"); break;
    case SYS_write : ret = fs_write(a[1],(void*)a[2],a[3]); break;
    case SYS_read  : ret = fs_read(a[1],(void*)a[2],a[3]); break;
    case SYS_open  : ret = fs_open((char*)a[1],a[2],a[3]); break;
    case SYS_close : ret = fs_close(a[1]); break;
    case SYS_lseek : ret = fs_lseek(a[1],a[2],a[3]); break;
    case SYS_brk   : break;
    case SYS_execve: naive_uload(NULL, (const char*)a[1]); break;
    
    /* End */
    default: panic("Unhandled syscall ID = %d", a[0]);
  }
  /* Start */

  c->GPRx = ret;

  /* End */
  return NULL;
}
