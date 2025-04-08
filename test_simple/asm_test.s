.syntax unified
.cpu cortex-m0
.thumb

.section .text
.align 2
.global _start

_start:
    // Initialize registers
    movs    r0, #0          // Clear R0
    movs    r1, #1          // Set R1 = 1
    movs    r2, #10         // Set R2 = 10
    movs    r3, #0          // Clear R3
    
    // Data processing test
data_proc_test:
    adds    r0, r0, r1      // R0 = R0 + R1 (0 + 1 = 1)
    adds    r3, r0, #5      // R3 = R0 + 5 (1 + 5 = 6)
    subs    r3, r3, #2      // R3 = R3 - 2 (6 - 2 = 4)
    muls    r4, r2, r1      // R4 = R2 * R1 (10 * 1 = 10)
    
    // Bitwise operations test
bit_ops_test:
    movs    r5, #0x0F       // R5 = 0xF (1111)
    movs    r6, #0x03       // R6 = 0x3 (0011)
    ands    r7, r5, r6      // R7 = R5 & R6 (0xF & 0x3 = 0x3)
    movs    r0, #0x08       
    orrs    r7, r7, r0      // R7 = R7 | 0x8 (0x3 | 0x8 = 0xB)
    movs    r0, #0x0F
    eors    r7, r7, r0      // R7 = R7 ^ 0xF (0xB ^ 0xF = 0x4)
    
    // Shift operations test
shift_test:
    movs    r0, #1          // R0 = 1
    lsls    r0, r0, #3      // R0 = R0 << 3 (1 << 3 = 8)
    lsrs    r0, r0, #1      // R0 = R0 >> 1 (8 >> 1 = 4)
    movs    r1, #0x80       // High bit in a byte
    lsls    r1, r1, #24     // Shift to create 0x80000000
    asrs    r1, r1, #1      // R1 = R1 >> 1 with sign extension
    
    // Memory operations test
mem_test:
    // Setup a memory area
    ldr     r0, =mem_area   // Load address of mem_area
    movs    r1, #0x42       // R1 = 0x42
    str     r1, [r0]        // Store R1 at [R0]
    ldr     r2, [r0]        // Load from [R0] into R2
    
    // Byte/halfword operations
    movs    r3, #0x55       // R3 = 0x55
    strb    r3, [r0, #4]    // Store byte at [R0+4]
    ldrb    r4, [r0, #4]    // Load byte from [R0+4]
    movs    r3, #0xAA       // First byte
    movs    r5, #0x55       // Second byte
    lsls    r3, r3, #8      // Shift to upper byte
    adds    r3, r3, r5      // Combine to make 0xAA55
    strh    r3, [r0, #8]    // Store halfword at [R0+8]
    ldrh    r4, [r0, #8]    // Load halfword from [R0+8]
    
    // Stack operations test
stack_test:
    push    {r0-r3}         // Push R0-R3 onto stack
    movs    r0, #0xFF       // Change R0
    movs    r1, #0xFF       // Change R1
    pop     {r0-r3}         // Pop back original values
    
    // Conditional execution test
cond_test:
    movs    r0, #5          // R0 = 5
    movs    r1, #10         // R1 = 10
    cmp     r0, r1          // Compare R0 with R1 (sets flags)
    blt     less_than       // Branch if R0 < R1
    b       end_test        // Should not be taken
less_than:
    adds    r2, r0, r1      // R2 = R0 + R1 (5 + 10 = 15)
    
end_test:
    // Set a success pattern in registers
    movs    r0, #0xAA       // Success code
    b       loop            // Loop forever
    
loop:
    b       loop            // Infinite loop to end test
    
    // Data area
    .align 4
mem_area:
    .space 32               // Reserve 32 bytes of memory