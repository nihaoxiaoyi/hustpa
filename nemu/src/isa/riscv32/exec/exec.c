#include "cpu/exec.h"
#include "all-instr.h"

// mycode: EX(lb)  EX(lh)  EXW(ld,1)  EXW(ld,2)
static OpcodeEntry load_table [8] = {
  EX(lb), EX(lh), EXW(ld, 4), EMPTY, EXW(ld,1), EXW(ld,2), EMPTY, EMPTY
};

static make_EHelper(load) {
  decinfo.width = load_table[decinfo.isa.instr.funct3].width;
  idex(pc, &load_table[decinfo.isa.instr.funct3]);
}

// mycode: EXW(st,1), EXW(st,2)
static OpcodeEntry store_table [8] = {
  EXW(st,1), EXW(st,2), EXW(st, 4), EMPTY, EMPTY, EMPTY, EMPTY, EMPTY
};

static make_EHelper(store) {
  decinfo.width = store_table[decinfo.isa.instr.funct3].width;
  idex(pc, &store_table[decinfo.isa.instr.funct3]);
}

// mycode: IDEX(I, imm_opcode_4) IDEX(U, auipc) IDEX(I, jalr) IDEX(J, jal)
static OpcodeEntry opcode_table [32] = {
  /* b00 */ IDEX(ld, load), EMPTY, EMPTY, EMPTY, IDEX(I, imm_opcode_4), IDEX(U, auipc), EMPTY, EMPTY,
  /* b01 */ IDEX(st, store), EMPTY, EMPTY, EMPTY, EMPTY, IDEX(U, lui), EMPTY, EMPTY,
  /* b10 */ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
  /* b11 */ EMPTY, IDEX(I, jalr), EX(nemu_trap), IDEX(J, jal), EMPTY, EMPTY, EMPTY, EMPTY,
};

void isa_exec(vaddr_t *pc) {
  decinfo.isa.instr.val = instr_fetch(pc, 4);
  assert(decinfo.isa.instr.opcode1_0 == 0x3);
  printf("0x%x : 0x%x\n", cpu.pc, decinfo.isa.instr.val);
  idex(pc, &opcode_table[decinfo.isa.instr.opcode6_2]);
}
