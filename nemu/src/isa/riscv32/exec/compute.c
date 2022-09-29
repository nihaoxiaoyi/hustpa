#include "cpu/exec.h"

make_EHelper(lui) {
  rtl_sr(id_dest->reg, &id_src->val, 4);

  print_asm_template2(lui);
}

/* my instruction */
/* Start */
make_EHelper(auipc) {
  rtl_add(&id_dest->val, &cpu.pc, &id_src->val);
  rtl_sr(id_dest->reg,&id_dest->val,4);

  print_asm_template2(auipc);
}

make_EHelper(I_opcode_4){
  switch (decinfo.isa.instr.funct3){
  case 0:{  // addi
    rtl_add(&id_dest->val,&id_src->val,&id_src2->val);
    rtl_sr(id_dest->reg,&id_dest->val,4);
    print_asm_template2(addi);
    break;
  }
  case 1:{  // slli

    break;
  }
  case 2:{  // slti
    
    break;
  }
  case 3:{  // sltiu
    
    break;
  }
  case 4:{  // xori
    
    break;
  }
  case 5:{  // srli || srai
    
    break;
  }
  case 6:{  // ori
    
    break;
  }
  case 7:{  // andi
    
    break;
  }
  default:
    break;
  }
}

/* End */