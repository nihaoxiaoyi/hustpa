#include "cpu/exec.h"
#include "monitor/monitor.h"

make_EHelper(inv) {
  /* invalid opcode */

  uint32_t instr[2];
  *pc = cpu.pc;
  instr[0] = instr_fetch(pc, 4);
  instr[1] = instr_fetch(pc, 4);

  printf("invalid opcode(PC = 0x%08x): %08x %08x ...\n\n",
      cpu.pc, instr[0], instr[1]);

  display_inv_msg(cpu.pc);

  rtl_exit(NEMU_ABORT, cpu.pc, -1);

  print_asm("invalid opcode");
}

make_EHelper(nemu_trap) {
  difftest_skip_ref();
  
  printf("cpu.pc = 0x%x , $a0 = 0x%x\n", cpu.pc, cpu.gpr[10]._32);
  rtl_exit(NEMU_END, cpu.pc, cpu.gpr[10]._32); // grp[10] is $a0
  printf("ok\n");

  print_asm("nemu trap");
  printf("nemu trap\n");
  return;
}
