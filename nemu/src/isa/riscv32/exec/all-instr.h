#include "cpu/exec.h"

make_EHelper(lui);      // compute.c

make_EHelper(ld);       // ldst.c
make_EHelper(st);       // ldst.c

make_EHelper(inv);          // special.c
make_EHelper(nemu_trap);    // special.c

/* my instruction */
/* Start */

// compute.c
make_EHelper(auipc);
make_EHelper(I_opcode_4);
make_EHelper(R_opcode_c);

// control.c
make_EHelper(jal);
make_EHelper(jalr);
make_EHelper(B_opcode_18);

// ldst.c
make_EHelper(lh);
make_EHelper(lb);

// system.c
make_EHelper(system);

/* End */