#ifndef __ARCH_H__
#define __ARCH_H__

struct _Context {
  uintptr_t epc, gpr[32], cause, status;
  struct _AddressSpace *as;
};

#define GPR1 gpr[17]

/* Start */

#define GPR2 gpr[10]
#define GPR3 gpr[11]
#define GPR4 gpr[12]
#define GPRx gpr[10]

/* End */

#endif
