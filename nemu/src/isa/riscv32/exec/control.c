#include "cpu/exec.h"

/* my instruction */
/* Start */
make_EHelper(jal){
  t0 = cpu.pc + 4;
  rtl_sr(id_dest->reg, &t0, 4);
  // printf("control.c J:%s=0x%x\n",reg_name(id_dest->reg,4),reg_l(id_dest->reg));  // my test

  rtl_add(&decinfo.jmp_pc, &cpu.pc, &id_src->val);
  rtl_j(decinfo.jmp_pc);

  print_asm_template2(jal);
}

make_EHelper(jalr){
  t0 = cpu.pc + 4;
  rtl_sr(id_dest->reg, &t0, 4);
  decinfo.jmp_pc = (id_src->val+id_src2->val)&(~1);
  rtl_j(decinfo.jmp_pc);

  difftest_skip_dut(1, 2); //difftest

  print_asm_template2(jalr);
}

/**
 * opcode of B_type_Instruction equal to 0x18
*/
make_EHelper(B_opcode_18){

  decinfo.jmp_pc = cpu.pc + id_dest->val;   // compute addr of jump pc

  switch (decinfo.isa.instr.funct3)
  {
  case 0: { // beq || beqz
    rtl_jrelop(RELOP_EQ, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(beq);

    break;
  }
  case 1: { // bne
    rtl_jrelop(RELOP_NE, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(bne);
    
    break;
  }
  case 4: { // blt
    rtl_jrelop(RELOP_LT, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(blt);
    
    break;
  }
  case 5: { // bge
    rtl_jrelop(RELOP_GE, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(bge);
    
    break;
  }
  case 6: { // bltu
    rtl_jrelop(RELOP_LTU, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(bltu);
    
    break;
  }
  case 7: { // bgeu
    rtl_jrelop(RELOP_GEU, &id_src->val, &id_src2->val, decinfo.jmp_pc);

    print_asm_template3(bgeu);
    
    break;
  }
  default: break;
  }
}
/* End */