/*********************************************************
 *                                                       *
 *  EE511 Project 1                                      *
 *                                                       *
 *  C simulator for Cortex-M0                            *
 *                                                       *
 *********************************************************/

#ifndef ISS_H
#define ISS_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>


// Registers
extern uint32_t R[16];

typedef struct APSR {
  int N;
  int Z;
  int C;
  int V;
} APSR_t;

extern APSR_t APSR;

#define SP R[13]
#define LR R[14]
#define PC R[15]

#define MEM_SIZE  0x4000000
extern uint8_t *mem;


// BRANCH should be set to 1 whenever branch occurs
extern int branch;
//instruction address for EXE pipeline stage(PC-4)
extern uint32_t EXE_PC;

uint16_t fetch(void);
void process(uint16_t inst);
void updatePC(void);

uint32_t extract32(uint32_t data, int end, int start);
uint32_t extract32_(uint32_t data, int pos);
uint16_t extract16(uint16_t data, int end, int start);
uint16_t extract16_(uint16_t data, int pos);
uint32_t sign_extend(uint32_t a, int length);

/* Shorthand for extracting bits in inst. */
#define INST(x, y) extract16(inst, x, y)
#define INST_(x) extract16_(inst, x)

#define INST32(x, y) extract32(inst, x, y)
#define INST32_(x) extract32_(inst, x)

#define MSB(x) extract32_(x, 31)

// Sign extension
#define zeroExtend32(x) (x)
#define signExtend32(x, n) (((((x) >> ((n)-1)) & 0x1) != 0) ? (~((unsigned int)0) << (n)) | (x) : (x))

/* Memory access functions. */
uint32_t read_word(uint32_t addr);
uint32_t read_halfword(uint32_t addr);
uint32_t read_byte(uint32_t addr);

void write_word(uint32_t addr, uint32_t value);
void write_halfword(uint32_t addr, uint32_t value);
void write_byte(uint32_t addr, uint32_t value);

/* your code here */

/*********************************************************
Notes : 
1. Skip (CPS, YIELD, WFE, WFI, SEV, MSR, MRS) which are privileged/system instructions 
2. Name of the function is same as the name of the instruction in the .pdf
3. Order of instructions : as in the Cortex-M0 Instruction Set Encoding .pdf
*********************************************************/

void lsl_immediate(uint16_t inst);
void lsr_immediate(uint16_t inst);
void asr_immediate(uint16_t inst);
void add_register1(uint16_t inst);
void sub_register(uint16_t inst);
void add_immediate3(uint16_t inst);
void sub_immediate3(uint16_t inst);
void mov_immediate(uint16_t inst);
void cmp_immediate(uint16_t inst);
void add_immediate8(uint16_t inst);
void sub_immediate8(uint16_t inst);
void and_register(uint16_t inst);
void eor_register(uint16_t inst);
void lsl_register(uint16_t inst);
void lsr_register(uint16_t inst);
void asr_register(uint16_t inst);
void adc_register(uint16_t inst);
void sbc_register(uint16_t inst);
void ror_register(uint16_t inst);
void tst_register(uint16_t inst);
void rsb_immediate(uint16_t inst);
void cmp_register1(uint16_t inst);
void cmn_register(uint16_t inst);
void orr_register(uint16_t inst);
void mul(uint16_t inst);
void bic_register(uint16_t inst);
void mvn_register(uint16_t inst);
void add_register2(uint16_t inst);
void cmp_register2(uint16_t inst);
void mov_register(uint16_t inst);
void bx(uint16_t inst);
void blx(uint16_t inst);
void ldr_literal(uint16_t inst);
void str_register(uint16_t inst);
void strh_register(uint16_t inst);
void strb_register(uint16_t inst);
void ldrsb_register(uint16_t inst);
void ldr_register(uint16_t inst);
void ldrh_register(uint16_t inst);
void ldrb_register(uint16_t inst);
void ldrsh_register(uint16_t inst);
void str_immediate5(uint16_t inst);
void ldr_immediate5(uint16_t inst);
void strb_immediate(uint16_t inst);
void ldrb_immediate(uint16_t inst);
void strh_immediate(uint16_t inst);
void ldrh_immediate(uint16_t inst);
void str_immediate8(uint16_t inst);
void ldr_immediate8(uint16_t inst);
void adr(uint16_t inst);
void add_sp_immediate1(uint16_t inst);
void add_sp_immediate2(uint16_t inst);
void sub_sp_immediate(uint16_t inst);
void sxth(uint16_t inst);
void sxtb(uint16_t inst);
void uxth(uint16_t inst);
void uxtb(uint16_t inst);
void push(uint16_t inst);
void rev(uint16_t inst);
void rev16(uint16_t inst);
void revsh(uint16_t inst);
void pop(uint16_t inst);
void bkpt(uint16_t inst);
void nop(uint16_t inst);
void stm(uint16_t inst);
void ldm(uint16_t inst);
void b_conditional(uint16_t inst);
void b_unconditional(uint16_t inst);
void bl(uint32_t inst);


// Helper functions
int condition_passed(uint16_t inst);
void update_apsr(uint32_t result, uint32_t op1, uint32_t op2, int add);
#endif
