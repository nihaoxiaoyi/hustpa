// #include <unistd.h>
// #include <sys/stat.h>
// #include <setjmp.h>
// #include <sys/time.h>
// #include <assert.h>
// #include <time.h>
// #include "syscall.h"

// // helper macros
// #define _concat(x, y) x ## y
// #define concat(x, y) _concat(x, y)
// #define _args(n, list) concat(_arg, n) list
// #define _arg0(a0, ...) a0
// #define _arg1(a0, a1, ...) a1
// #define _arg2(a0, a1, a2, ...) a2
// #define _arg3(a0, a1, a2, a3, ...) a3
// #define _arg4(a0, a1, a2, a3, a4, ...) a4
// #define _arg5(a0, a1, a2, a3, a4, a5, ...) a5

// // extract an arguments from the macro array
// #define SYSCALL  _args(0, ARGS_ARRAY)
// #define GPR1 _args(1, ARGS_ARRAY)
// #define GPR2 _args(2, ARGS_ARRAY)
// #define GPR3 _args(3, ARGS_ARRAY)
// #define GPR4 _args(4, ARGS_ARRAY)
// #define GPRx _args(5, ARGS_ARRAY)

// // ISA-depedent definitions
// #if defined(__ISA_X86__)
// # define ARGS_ARRAY ("int $0x80", "eax", "ebx", "ecx", "edx", "eax")
// #elif defined(__ISA_MIPS32__)
// # define ARGS_ARRAY ("syscall", "v0", "a0", "a1", "a2", "v0")
// #elif defined(__ISA_RISCV32__)
// # define ARGS_ARRAY ("ecall", "a7", "a0", "a1", "a2", "a0")
// #elif defined(__ISA_AM_NATIVE__)
// # define ARGS_ARRAY ("call *0x100000", "rax", "rdi", "rsi", "rdx", "rax")
// #else
// #error syscall is not supported
// #endif

// /* Start */

// extern char _end;

// /* End */

// intptr_t _syscall_(intptr_t type, intptr_t a0, intptr_t a1, intptr_t a2) {
//   register intptr_t _gpr1 asm (GPR1) = type;
//   register intptr_t _gpr2 asm (GPR2) = a0;
//   register intptr_t _gpr3 asm (GPR3) = a1;
//   register intptr_t _gpr4 asm (GPR4) = a2;
//   register intptr_t ret asm (GPRx);
//   asm volatile (SYSCALL : "=r" (ret) : "r"(_gpr1), "r"(_gpr2), "r"(_gpr3), "r"(_gpr4));
//   return ret;
// }

// void _exit(int status) {
//   _syscall_(SYS_exit, status, 0, 0);
//   while (1);
// }

// int _open(const char *path, int flags, mode_t mode) {
//   /* Start */

//   return (int)_syscall_(SYS_open, (uintptr_t)path, flags, mode);

//   /* End */
// }

// int _write(int fd, void *buf, size_t count) {
//   /* Start */

//   return (int)_syscall_(SYS_write, fd, (intptr_t)buf, count);

//   /* End */
// }

// void *_sbrk(intptr_t increment) {
//   /* Start */

//   static void* program_break = (uintptr_t)&_end;
//   void* old = program_break;
//   if(_syscall_(SYS_brk, (uintptr_t)program_break + increment, 0, 0) == 0){
//     program_break = program_break + increment;
//     return (void*) old;
//   }
//   return (void *)-1;

//   /* End */
// }

// int _read(int fd, void *buf, size_t count) {
//   /* Start */

//   return (int)_syscall_(SYS_read, fd, (uintptr_t)buf, count);

//   /* End */
// }

// int _close(int fd) {
//   /* Start */

//   return (int)_syscall_(SYS_close, fd, 0, 0);

//   /* End */
// }

// off_t _lseek(int fd, off_t offset, int whence) {
//    /* Start */

//   return (off_t)_syscall_(SYS_lseek, fd, offset, whence);

//   /* End */
// }

// int _execve(const char *fname, char * const argv[], char *const envp[]) {
//   _exit(SYS_execve);
//   return 0;
// }

// // The code below is not used by Nanos-lite.
// // But to pass linking, they are defined as dummy functions

// int _fstat(int fd, struct stat *buf) {
//   return 0;
// }

// int _kill(int pid, int sig) {
//   _exit(-SYS_kill);
//   return -1;
// }

// pid_t _getpid() {
//   _exit(-SYS_getpid);
//   return 1;
// }

// pid_t _fork() {
//   assert(0);
//   return -1;
// }

// int _link(const char *d, const char *n) {
//   assert(0);
//   return -1;
// }

// int _unlink(const char *n) {
//   assert(0);
//   return -1;
// }

// pid_t _wait(int *status) {
//   assert(0);
//   return -1;
// }

// clock_t _times(void *buf) {
//   assert(0);
//   return 0;
// }

// int _gettimeofday(struct timeval *tv) {
//   assert(0);
//   tv->tv_sec = 0;
//   tv->tv_usec = 0;
//   return 0;
// }
#include "proc.h"
#include <elf.h>
#include <fs.h>

#ifdef __ISA_AM_NATIVE__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

size_t ramdisk_read(void *buf, size_t offset, size_t len);


//finished loader
static uintptr_t loader(PCB *pcb, const char *filename) {
  Elf_Ehdr head;
  int fd=fs_open(filename,0,0);
  fs_lseek(fd,0,SEEK_SET);
  fs_read(fd,&head,sizeof(head));
  for(int i=0;i<head.e_phnum;i++){
    Elf_Phdr temp;
    fs_lseek(fd,head.e_phoff+i*head.e_phentsize,SEEK_SET);
    fs_read(fd,&temp,sizeof(temp));
    if(temp.p_type==PT_LOAD){
      fs_lseek(fd,temp.p_offset,SEEK_SET);
      fs_read(fd,(void*)temp.p_vaddr,temp.p_filesz);
      memset((void*)(temp.p_vaddr+temp.p_filesz),0,temp.p_memsz-temp.p_filesz);
    }
  }
  return head.e_entry;
}

//finished naive_uload
void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %x", entry);
  ((void(*)())entry) ();
}

void context_kload(PCB *pcb, void *entry) {
  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _kcontext(stack, entry, NULL);
}

void context_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);

  _Area stack;
  stack.start = pcb->stack;
  stack.end = stack.start + sizeof(pcb->stack);

  pcb->cp = _ucontext(&pcb->as, stack, stack, (void *)entry, NULL);
}
