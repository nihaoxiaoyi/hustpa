#include "cpu/exec.h"

/* my instruction */
/* Start */
make_EHelper(jal){
  uint32_t addr=cpu.pc+4;
  rtl_sr(id_dest->reg, &addr, 4);
  printf("control.c J:%s=0x%x\n",reg_name(id_dest->reg,4),reg_l(id_dest->reg));  // test

  rtl_add(&decinfo.jmp_pc, &cpu.pc, &id_src->val);
  rtl_j(decinfo.jmp_pc);

  print_asm_template2(jal);
}

make_EHelper(jalr){
  uint32_t addr=cpu.pc+4;
  rtl_sr(id_dest->reg, &addr, 4);
  decinfo.jmp_pc = (id_src->val+id_src2->val)&(~1);
  rtl_j(decinfo.jmp_pc);

  difftest_skip_dut(1, 2); //difftest

  print_asm_template2(jalr);
}
/* End */