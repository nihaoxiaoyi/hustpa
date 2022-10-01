#include "nemu.h"
#include "monitor/diff-test.h"

/* my diff_test code*/
/* Start */

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  bool ret = true;
  for (size_t i = 0; i < 32; i++){
    if(cpu.gpr[i]._32 != ref_r->gpr[i]._32){
      printf("reg_name:%-4s nemu:0x%-8x qemu:0x%-8x\n", reg_name(i,4), cpu.gpr[i]._32, ref_r->gpr[i]._32);
      ret = false;
    }
  }
  return false;
}

void isa_difftest_attach(void) {
}
/* End */