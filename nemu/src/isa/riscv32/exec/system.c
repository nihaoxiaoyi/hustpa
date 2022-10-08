#include "cpu/exec.h"

/* my instruction */
/* Start */

void raise_intr(uint32_t NO, vaddr_t epc);

enum {
    SSTATUS=0x100, STVEC=0x105, SEPC=0x141, SCAUSE=0x142
    };

make_EHelper(system){
    switch (decinfo.isa.instr.funct3){
    case 0: {
      if(decinfo.isa.instr.simm11_0 == 0x0){ //ecall
        raise_intr(reg_l(17), cpu.pc);
        }
      else if(decinfo.isa.instr.simm11_0 == 0x102){ //sret
        rtl_j(cpu.sepc+4);
        }
      break;
      }
    case 1: { //csrrw 
      switch(decinfo.isa.instr.csr){
        case SEPC: {
          t0 = cpu.sepc;
          cpu.sepc = id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case SSTATUS: {
          t0 = cpu.sstatus;
          cpu.sstatus = id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case SCAUSE: {
          t0 = cpu.scause;
          cpu.scause = id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case STVEC:{
          t0=cpu.stvec;
          cpu.stvec=id_src->val;
          rtl_sr(id_dest->reg,&t0,4);
          break;
          } 
        default: break;
        }
      print_asm_template3(csrrw);
      break;
      }
    case 2: { //csrrs
      switch(decinfo.isa.instr.csr){
        case SEPC: {
          t0 = cpu.sepc;
          cpu.sepc = t0 | id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case SSTATUS: {
          t0 = cpu.sstatus;
          cpu.sstatus = t0 | id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case SCAUSE:{
          t0 = cpu.scause;
          cpu.scause = t0 | id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        case STVEC:{
          t0 = cpu.stvec;
          cpu.stvec = t0|id_src->val;
          rtl_sr(id_dest->reg, &t0, 4);
          break;
          }
        default: break;
        }
      print_asm_template3(csrrs);
      break;
      }
    default: break;
    }
}

/* End */