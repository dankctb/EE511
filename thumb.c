/*********************************************************
 *                                                       *
 *  EE511 Project 1                                      *
 *                                                       *
 *  C simulator for Cortex-M0                            *
 *                                                       *
 *********************************************************/


#include "iss.h"

void b_unconditional(uint16_t inst);
void bl(uint32_t inst);

void process(uint16_t inst)
{
  uint16_t inst2;
  uint32_t inst32;

  if ((INST(15, 10) == 0x10) && (INST(9, 8) != 0x0)) {
    // Data processing instructions
    switch (INST(9, 6)) {
      case 0x0: and_register(inst); break;
      case 0x1: eor_register(inst); break;
      case 0x2: lsl_register(inst); break;
      case 0x3: lsr_register(inst); break;
      case 0x4: asr_register(inst); break;
      case 0x5: adc(inst); break;
      case 0x6: sbc_register(inst); break;
      case 0x7: ror_register(inst); break;
      case 0x8: tst_register(inst); break;
      case 0x9: rsb_immediate(inst); break;
      case 0xA: cmp_register(inst); break;
      case 0xB: cmn_register(inst); break;
      case 0xC: orr_register(inst); break;
      case 0xD: mul(inst); break;
      case 0xE: bic_register(inst); break;
      case 0xF: mvn_register(inst); break;
    }
  }
  else if (INST(15, 11) == 0x0) {
    // Shift (immediate), add, subtract, move, and compare
    if (INST(12, 11) == 0x0)
      lsl_immediate(inst);
    else if (INST(12, 11) == 0x1)
      lsr_immediate(inst);
    else if (INST(12, 11) == 0x2)
      asr_immediate(inst);
    else if (INST(12, 10) == 0x6)
      add_register(inst);
    else if (INST(12, 10) == 0x7)
      sub_register(inst);
    else if (INST(12, 9) == 0x8)
      add_immediate(inst);
    else if (INST(12, 9) == 0x9)
      sub_immediate(inst);
    else if (INST(12, 11) == 0x5)
      mov_immediate(inst);
    else if (INST(12, 11) == 0x7)
      cmp_immediate(inst);
  }
  else if (INST(15, 10) == 0x11) {
    // Special data instructions and branch and exchange
    if (INST(9, 8) == 0x0)
      add_register(inst);
    else if (INST(9, 8) == 0x1)
      cmp_register(inst);
    else if (INST(9, 8) == 0x2)
      mov_register(inst);
    else if (INST(9, 8) == 0x3)
      bx(inst);
  }
  else if (INST(15, 13) == 0x1) {
    // Load/store single data item instructions
    if (INST(12, 11) == 0x2) {
      if (INST(10, 9) == 0x0)
        str_register(inst);
      else if (INST(10, 9) == 0x1)
        strh_register(inst);
      else if (INST(10, 9) == 0x2)
        strb_register(inst);
      else if (INST(10, 9) == 0x3)
        ldrsb_register(inst);
      else if (INST(10, 9) == 0x4)
        ldr_register(inst);
      else if (INST(10, 9) == 0x5)
        ldrh_register(inst);
      else if (INST(10, 9) == 0x6)
        ldrb_register(inst);
      else if (INST(10, 9) == 0x7)
        ldrsh_register(inst);
    }
  }
  else if (INST(15, 13) == 0x2) {
    // Load/store single data item (immediate)
    if (INST(12, 11) == 0x0)
      str_immediate(inst);
    else if (INST(12, 11) == 0x1)
      ldr_immediate(inst);
    else if (INST(12, 11) == 0x2)
      strb_immediate(inst);
    else if (INST(12, 11) == 0x3)
      ldrb_immediate(inst);
  }
  else if (INST(15, 13) == 0x3) {
    // More load/store single data item instructions
    if (INST(12, 11) == 0x0)
      strh_immediate(inst);
    else if (INST(12, 11) == 0x1)
      ldrh_immediate(inst);
  }
  else if (INST(15, 12) == 0x4) {
    // Miscellaneous 16-bit instructions
    if (INST_(11) == 0) {
      if (INST(10, 8) == 0x0)
        add_sp_immediate(inst);
      else if (INST(10, 8) == 0x1)
        sub_sp_immediate(inst);
      else if (INST(10, 7) == 0x4)
        push(inst);
    } else {
      if (INST(10, 8) == 0x5)
        pop(inst);
      else if (INST(10, 9) == 0x2)
        stm(inst);
      else if (INST(10, 9) == 0x3)
        ldm(inst);
    }
  }
  else if (INST(15, 12) == 0x9) {
    // Load from literal pool, load/store SP relative
    if (INST_(11) == 0)
      ldr_literal(inst);
  }
  else if (INST(15, 12) == 0xA) {
    // Generate PC-relative address
    adr(inst);
  }
  else if (INST(15, 12) == 0xB) {
    // Generate SP-relative address
    if (INST_(11) == 0)
      add_sp_immediate(inst);
    else
      sub_sp_immediate(inst);
  }
  else if (INST(15, 12) == 0xD) {
    // Conditional branch
    if (INST(11, 8) != 0xF)
      b_conditional(inst);
  }
  else if (INST(15, 11) == 0x1C) {
    b_unconditional(inst);
  }
  else if (INST(15, 11) == 0x1E) {
    inst2 = read_halfword(EXE_PC + 2);
    inst32 = ((uint32_t) inst << 16) | ((uint32_t) inst2);
    if (extract16_(inst2, 14) && extract16_(inst2, 12))
      bl(inst32);
  }
}

/* your code here (for additional functions)*/

void b_unconditional(uint16_t inst)
{
  uint32_t imm11 = INST(10, 0);
  uint32_t address;

  address = PC + signExtend32((imm11 << 1), 12);
  branch = 1;
  PC = address & 0xFFFFFFFE;
}
void bl(uint32_t inst)
{
  uint32_t S = INST32_(10 + 16);
  uint32_t imm10 = INST32(9 + 16, 0 + 16);
  uint32_t J1 = INST32_(13);
  uint32_t J2 = INST32_(11);
  uint32_t imm11 = INST32(10, 0);
  uint32_t I1, I2, imm32, address;
  
  I1 = !(J1 ^ S);
  I2 = !(J2 ^ S);
  imm32 = sign_extend((S << 24) | (I1 << 23) | (I2 << 22) | (imm10 << 12) | (imm11 << 1), 25);

  LR = PC | 0x00000001;

  address = PC + imm32;
  branch = 1;
  PC = address & 0xFFFFFFFE;
}

// Helper function for condition flags
int condition_passed(uint16_t inst) {
  uint8_t cond = INST(11, 8);
  switch (cond) {
    case 0x0: return APSR.Z == 1;                          // EQ
    case 0x1: return APSR.Z == 0;                          // NE
    case 0x2: return APSR.C == 1;                          // CS/HS
    case 0x3: return APSR.C == 0;                          // CC/LO
    case 0x4: return APSR.N == 1;                          // MI
    case 0x5: return APSR.N == 0;                          // PL
    case 0x6: return APSR.V == 1;                          // VS
    case 0x7: return APSR.V == 0;                          // VC
    case 0x8: return (APSR.C == 1) && (APSR.Z == 0);       // HI
    case 0x9: return (APSR.C == 0) || (APSR.Z == 1);       // LS
    case 0xA: return APSR.N == APSR.V;                     // GE
    case 0xB: return APSR.N != APSR.V;                     // LT
    case 0xC: return (APSR.Z == 0) && (APSR.N == APSR.V);  // GT
    case 0xD: return (APSR.Z == 1) || (APSR.N != APSR.V);  // LE
    case 0xE: return 1;                                    // AL
    default: return 1;
  }
}

// Helper function to update APSR flags
void update_apsr(uint32_t result, uint32_t op1, uint32_t op2, int add) {
  // Update N and Z flags
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
  
  // Update C and V flags
  if (add) {
    // Addition: C = carry, V = signed overflow
    APSR.C = ((uint64_t)op1 + (uint64_t)op2) > 0xFFFFFFFF ? 1 : 0;
    APSR.V = ((~(op1 ^ op2) & (op1 ^ result)) >> 31) & 0x1;
  } else {
    // Subtraction: C = NOT borrow, V = signed overflow
    APSR.C = op1 >= op2 ? 1 : 0;
    APSR.V = (((op1 ^ op2) & (op1 ^ result)) >> 31) & 0x1;
  }
}

// Data processing instructions

// Add with Carry
void adc(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t op1 = R[rd_n];
  uint32_t op2 = R[rm];
  uint32_t result = op1 + op2 + APSR.C;
  
  R[rd_n] = result;
  update_apsr(result, op1, op2 + APSR.C, 1);
}

// Add immediate
void add_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm3 = INST(8, 6);
  uint32_t op1 = R[rn];
  uint32_t op2 = imm3;
  uint32_t result = op1 + op2;
  
  R[rd] = result;
  update_apsr(result, op1, op2, 1);
}

// Add register
void add_register(uint16_t inst) {
  uint32_t rd, rn, rm, op1, op2, result;
  
  if (INST(15, 10) == 0x11) {  // High register operations
    rd = (INST_(7) << 3) | INST(2, 0);
    rn = rd;
    rm = INST(6, 3);
    op1 = R[rn];
    op2 = R[rm];
    result = op1 + op2;
    R[rd] = result;
    // No flag updates for high register operations
  } else {  // Standard ADD register
    rd = INST(2, 0);
    rn = INST(5, 3);
    rm = INST(8, 6);
    op1 = R[rn];
    op2 = R[rm];
    result = op1 + op2;
    R[rd] = result;
    update_apsr(result, op1, op2, 1);
  }
}

// Add SP plus immediate
void add_sp_immediate(uint16_t inst) {
  uint32_t imm8, rd, imm, result;
  
  if (INST(15, 11) == 0x14) {  // ADD Rd, SP, imm
    rd = INST(10, 8);
    imm8 = INST(7, 0);
    imm = imm8 << 2;
  } else {  // ADD SP, SP, imm
    rd = 13;  // SP
    imm = INST(6, 0) << 2;
  }
  
  result = SP + imm;
  R[rd] = result;
}

// Add SP plus register
void add_sp_register(uint16_t inst) {
  uint32_t rm = INST(6, 3);
  
  SP = SP + R[rm];
}

// Generate PC-relative address
void adr(uint16_t inst) {
  uint32_t rd = INST(10, 8);
  uint32_t imm8 = INST(7, 0);
  uint32_t imm = imm8 << 2;
  
  // Align PC to 4 bytes
  uint32_t aligned_pc = (PC & ~0x3);
  R[rd] = aligned_pc + imm;
}

// Logical AND
void and_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rd_n] & R[rm];
  
  R[rd_n] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Arithmetic Shift Right
void asr_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t shift_n = imm5 == 0 ? 32 : imm5;
  uint32_t result;
  
  if (shift_n == 32) {
    // Special case: shift by 32
    if ((R[rm] & 0x80000000) != 0) {
      result = 0xFFFFFFFF;
    } else {
      result = 0;
    }
    APSR.C = (R[rm] >> 31) & 0x1;
  } else {
    // Normal shift
    result = ((int32_t)R[rm]) >> shift_n;
    APSR.C = (R[rm] >> (shift_n - 1)) & 0x1;
  }
  
  R[rd] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Arithmetic Shift Right register
void asr_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t shift_n = R[rm] & 0xFF;
  uint32_t result;
  
  if (shift_n == 0) {
    result = R[rd_n];
  } else if (shift_n < 32) {
    result = ((int32_t)R[rd_n]) >> shift_n;
    APSR.C = (R[rd_n] >> (shift_n - 1)) & 0x1;
  } else {
    // Shift by 32 or more
    if ((R[rd_n] & 0x80000000) != 0) {
      result = 0xFFFFFFFF;
    } else {
      result = 0;
    }
    APSR.C = (R[rd_n] >> 31) & 0x1;
  }
  
  R[rd_n] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Conditional branch
void b_conditional(uint16_t inst) {
  if (condition_passed(inst)) {
    uint32_t imm8 = INST(7, 0);
    uint32_t imm32 = signExtend32((imm8 << 1), 9);  // Sign-extend and shift
    uint32_t target = PC + imm32;
    
    branch = 1;
    PC = target;
  }
}

// Bit Clear
void bic_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rd_n] & (~R[rm]);
  
  R[rd_n] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Branch with Exchange
void bx(uint16_t inst) {
  uint32_t rm = INST(6, 3);
  uint32_t address = R[rm];
  
  branch = 1;
  PC = address & 0xFFFFFFFE;  // Clear bit 0 for Thumb
}

// Compare Negative
void cmn_register(uint16_t inst) {
  uint32_t rn = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t op1 = R[rn];
  uint32_t op2 = R[rm];
  uint32_t result = op1 + op2;
  
  update_apsr(result, op1, op2, 1);
}

// Compare immediate
void cmp_immediate(uint16_t inst) {
  uint32_t rn = INST(10, 8);
  uint32_t imm8 = INST(7, 0);
  uint32_t op1 = R[rn];
  uint32_t op2 = imm8;
  uint32_t result = op1 - op2;
  
  update_apsr(result, op1, op2, 0);
}

// Compare register
void cmp_register(uint16_t inst) {
  uint32_t rn, rm, op1, op2, result;
  
  if (INST(15, 10) == 0x10) {  // Regular register comparison
    rn = INST(2, 0);
    rm = INST(5, 3);
  } else {  // High register comparison
    rn = (INST_(7) << 3) | INST(2, 0);
    rm = INST(6, 3);
  }
  
  op1 = R[rn];
  op2 = R[rm];
  result = op1 - op2;
  
  update_apsr(result, op1, op2, 0);
}

// Exclusive OR
void eor_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rd_n] ^ R[rm];
  
  R[rd_n] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Load and store instructions

// Load Multiple
void ldm(uint16_t inst) {
  uint32_t rn = INST(10, 8);
  uint32_t register_list = INST(7, 0);
  uint32_t address = R[rn];
  uint32_t wback_address = address;
  int count = 0;
  
  // Execute load for each set bit in register_list
  for (int i = 0; i < 8; i++) {
    if ((register_list >> i) & 0x1) {
      R[i] = read_word(address);
      address += 4;
      count++;
    }
  }
  
  // Write back the new address to base register if it's not in the register list
  if (!(register_list & (1 << rn))) {
    R[rn] = wback_address + (count * 4);
  }
}

// Load Register immediate
void ldr_immediate(uint16_t inst) {
  uint32_t rt, rn, imm, address;
  
  // Different instruction encodings
  if (INST(15, 13) == 0x3) {  // LDR (SP-relative)
    rt = INST(10, 8);
    imm = INST(7, 0) << 2;
    address = SP + imm;
  } else if (INST(15, 12) == 0x6) {  // LDR (immediate)
    rt = INST(2, 0);
    rn = INST(5, 3);
    imm = INST(10, 6) << 2;
    address = R[rn] + imm;
  } else {  // LDR (immediate)
    rt = INST(2, 0);
    rn = INST(5, 3);
    imm = INST(10, 6) << 2;
    address = R[rn] + imm;
  }
  
  R[rt] = read_word(address);
}

// Load Register literal
void ldr_literal(uint16_t inst) {
  uint32_t rt = INST(10, 8);
  uint32_t imm8 = INST(7, 0);
  uint32_t imm32 = imm8 << 2;
  
  // Align PC to 4 bytes
  uint32_t aligned_pc = (PC & ~0x3);
  uint32_t address = aligned_pc + imm32;
  
  R[rt] = read_word(address);
}

// Load Register register
void ldr_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  R[rt] = read_word(address);
}

// Load Register Byte immediate
void ldrb_immediate(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t address = R[rn] + imm5;
  
  R[rt] = read_byte(address);
}

// Load Register Byte register
void ldrb_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  R[rt] = read_byte(address);
}

// Load Register Halfword immediate
void ldrh_immediate(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t imm = imm5 << 1;  // Multiply by 2 for halfword access
  uint32_t address = R[rn] + imm;
  
  R[rt] = read_halfword(address);
}

// Load Register Halfword register
void ldrh_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  R[rt] = read_halfword(address);
}

// Load Register Signed Byte
void ldrsb_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  uint32_t byte = read_byte(address);
  
  // Sign extend
  R[rt] = (byte & 0x80) ? (0xFFFFFF00 | byte) : byte;
}

// Load Register Signed Halfword
void ldrsh_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  uint32_t halfword = read_halfword(address);
  
  // Sign extend
  R[rt] = (halfword & 0x8000) ? (0xFFFF0000 | halfword) : halfword;
}

// Logical Shift Left immediate
void lsl_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t result;
  
  if (imm5 == 0) {
    // No shift
    result = R[rm];
  } else {
    // Normal shift
    result = R[rm] << imm5;
    APSR.C = (R[rm] >> (32 - imm5)) & 0x1;
  }
  
  R[rd] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Logical Shift Left register
void lsl_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t shift_n = R[rm] & 0xFF;
  uint32_t result;
  
  if (shift_n == 0) {
    // No shift
    result = R[rd_n];
  } else if (shift_n < 32) {
    // Normal shift
    result = R[rd_n] << shift_n;
    APSR.C = (shift_n > 0) ? ((R[rd_n] >> (32 - shift_n)) & 0x1) : APSR.C;
  } else if (shift_n == 32) {
    // Shift by 32 -> 0 with C = bit 0
    result = 0;
    APSR.C = R[rd_n] & 0x1;
  } else {
    // Shift by more than 32 -> 0 with C = 0
    result = 0;
    APSR.C = 0;
  }
  
  R[rd_n] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Logical Shift Right immediate
void lsr_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t shift_n = imm5 == 0 ? 32 : imm5;
  uint32_t result;
  
  if (shift_n == 32) {
    // Special case: shift by 32
    result = 0;
    APSR.C = (R[rm] >> 31) & 0x1;
  } else {
    // Normal shift
    result = R[rm] >> shift_n;
    APSR.C = (R[rm] >> (shift_n - 1)) & 0x1;
  }
  
  R[rd] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Logical Shift Right register
void lsr_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t shift_n = R[rm] & 0xFF;
  uint32_t result;
  
  if (shift_n == 0) {
    // No shift
    result = R[rd_n];
  } else if (shift_n < 32) {
    // Normal shift
    result = R[rd_n] >> shift_n;
    APSR.C = (R[rd_n] >> (shift_n - 1)) & 0x1;
  } else if (shift_n == 32) {
    // Shift by 32 -> 0 with C = bit 31
    result = 0;
    APSR.C = (R[rd_n] >> 31) & 0x1;
  } else {
    // Shift by more than 32 -> 0 with C = 0
    result = 0;
    APSR.C = 0;
  }
  
  R[rd_n] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Move immediate
void mov_immediate(uint16_t inst) {
  uint32_t rd = INST(10, 8);
  uint32_t imm8 = INST(7, 0);
  
  R[rd] = imm8;
  
  APSR.N = 0;  // imm8 is always positive (< 0x80000000)
  APSR.Z = (imm8 == 0) ? 1 : 0;
}

// Move register
void mov_register(uint16_t inst) {
  uint32_t rd, rm;
  
  if (INST(15, 10) == 0x11) {  // High register operations
    rd = (INST_(7) << 3) | INST(2, 0);
    rm = INST(6, 3);
  } else {
    rd = INST(2, 0);
    rm = INST(5, 3);
  }
  
  R[rd] = R[rm];
  
  // Flag updates only for low register MOV
  if (INST(15, 10) != 0x11) {
    APSR.N = (R[rd] >> 31) & 0x1;
    APSR.Z = (R[rd] == 0) ? 1 : 0;
  }
}

// Multiply
void mul(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rd_n] * R[rm];
  
  R[rd_n] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Move NOT
void mvn_register(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = ~R[rm];
  
  R[rd] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Negate
void neg(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t op2 = R[rm];
  uint32_t result = 0 - op2;
  
  R[rd] = result;
  
  update_apsr(result, 0, op2, 0);
}

// Logical OR
void orr_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rd_n] | R[rm];
  
  R[rd_n] = result;
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Pop registers
void pop(uint16_t inst) {
  uint32_t register_list = INST(7, 0);
  uint32_t p = INST_(8);  // P bit for including PC
  uint32_t address = SP;
  
  // If P=1, add PC (bit 15) to the register list
  if (p) {
    register_list |= (1 << 15);
  }
  
  // Load registers from memory
  for (int i = 0; i < 8; i++) {
    if ((register_list >> i) & 0x1) {
      R[i] = read_word(address);
      address += 4;
    }
  }
  
  // Check and handle PC
  if (p) {
    PC = read_word(address) & 0xFFFFFFFE;  // Clear bit 0 for Thumb
    address += 4;
    branch = 1;
  }
  
  // Update SP
  SP = address;
}

// Push registers
void push(uint16_t inst) {
  uint32_t register_list = INST(7, 0);
  uint32_t m = INST_(8);  // M bit for including LR
  uint32_t count = 0;
  
  // If M=1, add LR (bit 14) to the register list
  if (m) {
    register_list |= (1 << 14);
  }
  
  // Count registers to calculate new SP
  for (int i = 0; i < 15; i++) {
    if ((register_list >> i) & 0x1) {
      count++;
    }
  }
  
  // Update SP first
  uint32_t address = SP - (count * 4);
  SP = address;
  
  // Store registers to memory
  for (int i = 0; i < 8; i++) {
    if ((register_list >> i) & 0x1) {
      write_word(address, R[i]);
      address += 4;
    }
  }
  
  // Store LR if M=1
  if (m) {
    write_word(address, LR);
  }
}

// Rotate Right
void ror_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t shift_n = R[rm] & 0xFF;
  uint32_t result;
  
  if (shift_n == 0) {
    // No rotation
    result = R[rd_n];
  } else {
    shift_n = shift_n & 0x1F;  // Modulo 32
    if (shift_n == 0) {
      // Rotate by 32 is same as no rotation, but update C flag
      result = R[rd_n];
      APSR.C = (R[rd_n] >> 31) & 0x1;
    } else {
      // Normal rotation
      result = (R[rd_n] >> shift_n) | (R[rd_n] << (32 - shift_n));
      APSR.C = (R[rd_n] >> (shift_n - 1)) & 0x1;
    }
  }
  
  R[rd_n] = result;
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Reverse Subtract
void rsb_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t op1 = 0;  // Always 0 in Cortex-M0
  uint32_t op2 = R[rn];
  uint32_t result = op1 - op2;
  
  R[rd] = result;
  
  update_apsr(result, op1, op2, 0);
}

// Subtract with Carry
void sbc_register(uint16_t inst) {
  uint32_t rd_n = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t op1 = R[rd_n];
  uint32_t op2 = R[rm];
  uint32_t result = op1 - op2 - (!APSR.C);
  
  R[rd_n] = result;
  
  // For Borrow: NOT Carry, so we use !APSR.C
  update_apsr(result, op1, op2 + (!APSR.C), 0);
}

// Store Multiple
void stm(uint16_t inst) {
  uint32_t rn = INST(10, 8);
  uint32_t register_list = INST(7, 0);
  uint32_t address = R[rn];
  uint32_t count = 0;
  
  // Count registers in list
  for (int i = 0; i < 8; i++) {
    if ((register_list >> i) & 0x1) {
      count++;
    }
  }
  
  // Store registers
  for (int i = 0; i < 8; i++) {
    if ((register_list >> i) & 0x1) {
      write_word(address, R[i]);
      address += 4;
    }
  }
  
  // Update base register
  R[rn] = R[rn] + (count * 4);
}

// Store Register immediate
void str_immediate(uint16_t inst) {
  uint32_t rt, rn, imm, address;
  
  // Different instruction encodings
  if (INST(15, 13) == 0x3) {  // STR (SP-relative)
    rt = INST(10, 8);
    imm = INST(7, 0) << 2;
    address = SP + imm;
  } else if (INST(15, 12) == 0x6) {  // STR (immediate)
    rt = INST(2, 0);
    rn = INST(5, 3);
    imm = INST(10, 6) << 2;
    address = R[rn] + imm;
  } else {  // STR (immediate)
    rt = INST(2, 0);
    rn = INST(5, 3);
    imm = INST(10, 6) << 2;
    address = R[rn] + imm;
  }
  
  write_word(address, R[rt]);
}

// Store Register register
void str_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  write_word(address, R[rt]);
}

// Store Register Byte immediate
void strb_immediate(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t address = R[rn] + imm5;
  
  write_byte(address, R[rt] & 0xFF);
}

// Store Register Byte register
void strb_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  write_byte(address, R[rt] & 0xFF);
}

// Store Register Halfword immediate
void strh_immediate(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm5 = INST(10, 6);
  uint32_t imm = imm5 << 1;  // Multiply by 2 for halfword alignment
  uint32_t address = R[rn] + imm;
  
  write_halfword(address, R[rt] & 0xFFFF);
}

// Store Register Halfword register
void strh_register(uint16_t inst) {
  uint32_t rt = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t address = R[rn] + R[rm];
  
  write_halfword(address, R[rt] & 0xFFFF);
}

// Subtract immediate
void sub_immediate(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t imm3 = INST(8, 6);
  uint32_t op1 = R[rn];
  uint32_t op2 = imm3;
  uint32_t result = op1 - op2;
  
  R[rd] = result;
  
  update_apsr(result, op1, op2, 0);
}

// Subtract register
void sub_register(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rn = INST(5, 3);
  uint32_t rm = INST(8, 6);
  uint32_t op1 = R[rn];
  uint32_t op2 = R[rm];
  uint32_t result = op1 - op2;
  
  R[rd] = result;
  
  update_apsr(result, op1, op2, 0);
}

// Subtract SP minus immediate
void sub_sp_immediate(uint16_t inst) {
  uint32_t imm7 = INST(6, 0);
  uint32_t imm = imm7 << 2;  // Scale by 4
  
  SP = SP - imm;
}

// Sign extend Byte
void sxtb(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t byte = R[rm] & 0xFF;
  
  // Sign extend the byte
  R[rd] = (byte & 0x80) ? (0xFFFFFF00 | byte) : byte;
}

// Sign extend Halfword
void sxth(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t halfword = R[rm] & 0xFFFF;
  
  // Sign extend the halfword
  R[rd] = (halfword & 0x8000) ? (0xFFFF0000 | halfword) : halfword;
}

// Test
void tst_register(uint16_t inst) {
  uint32_t rn = INST(2, 0);
  uint32_t rm = INST(5, 3);
  uint32_t result = R[rn] & R[rm];
  
  APSR.N = (result >> 31) & 0x1;
  APSR.Z = (result == 0) ? 1 : 0;
}

// Zero extend Byte
void uxtb(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  
  // Zero extend the byte
  R[rd] = R[rm] & 0xFF;
}

// Zero extend Halfword
void uxth(uint16_t inst) {
  uint32_t rd = INST(2, 0);
  uint32_t rm = INST(5, 3);
  
  // Zero extend the halfword
  R[rd] = R[rm] & 0xFFFF;
}

