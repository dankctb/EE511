/*********************************************************
 *                                                       *
 *  EE511 Project 1                                      *
 *                                                       *
 *  C test code                                          *
 *                                                       *
 *********************************************************/

/*your test code here*/
#include <stdint.h>

// Global variables for memory operations testing
volatile uint32_t g_data[16];
volatile uint8_t g_bytes[16];
volatile uint16_t g_halfwords[16];

// Function for testing branches
void test_func(int a, int b, int *result) {
    *result = a + b;
}

// Function with many parameters to test stack operations
int multi_param_func(int a, int b, int c, int d, int e, int f, int g, int h) {
    return a + b + c + d + e + f + g + h;
}

// Bit manipulation functions to exercise various instructions
uint32_t bit_clear(uint32_t val, uint32_t mask) {
    return val & (~mask);  // BIC operation
}

uint32_t rev_bytes(uint32_t value) {
    return ((value & 0xFF) << 24) | 
           ((value & 0xFF00) << 8) | 
           ((value & 0xFF0000) >> 8) | 
           ((value & 0xFF000000) >> 24);
}

uint32_t rev16_bytes(uint32_t value) {
    return ((value & 0xFF) << 8) | 
           ((value & 0xFF00) >> 8) | 
           ((value & 0xFF0000) << 8) | 
           ((value & 0xFF000000) >> 8);
}

uint32_t rev_sign_extend(uint32_t value) {
    uint16_t half = ((value & 0xFF) << 8) | ((value & 0xFF00) >> 8);
    // Sign extend
    if (half & 0x8000) {
        return half | 0xFFFF0000;
    } else {
        return half;
    }
}

int main(void) {
    int result = 0;
    int a = 5, b = 7, c = 0;
    
    // Test data processing instructions
    c = a + b;                 // ADDS
    c = a - b;                 // SUBS
    c = a * b;                 // MULS
    c = a & b;                 // ANDS
    c = a | b;                 // ORRS
    c = a ^ b;                 // EORS
    c = ~a;                    // MVNS
    c = -a;                    // NEGS (RSB #0)
    
    // Test bit manipulation operations
    c = bit_clear(0xFFFF, 0x00F0);  // BIC (should be 0xFF0F)
    
    // Test shift operations
    c = a << 2;                // LSLS immediate
    c = a >> 2;                // LSRS immediate
    c = ((int32_t)0x80000000) >> 4;  // ASRS immediate
    b = 3;
    c = a << b;                // LSLS register
    c = a >> b;                // LSRS register
    c = ((int32_t)0x80000000) >> b;  // ASRS register
    
    // Test memory operations
    g_data[0] = 0x12345678;    // STR
    c = g_data[1];             // LDR
    
    g_bytes[0] = 0xAA;         // STRB
    c = g_bytes[1];            // LDRB
    
    g_halfwords[0] = 0xBEEF;   // STRH
    c = g_halfwords[1];        // LDRH
    
    // Byte reversal functions (REV, REV16, REVSH)
    c = rev_bytes(0x12345678);      // REV
    c = rev16_bytes(0x12345678);    // REV16
    c = rev_sign_extend(0x1234);    // REVSH
    
    // Sign/zero extension
    int8_t byte_val = -5;
    int16_t half_val = -1000;
    c = (int32_t)byte_val;     // SXTB
    c = (int32_t)half_val;     // SXTH
    c = (uint32_t)(uint8_t)byte_val;  // UXTB
    c = (uint32_t)(uint16_t)half_val; // UXTH
    
    // Flag testing and conditional execution
    if (a > b) {
        c = a + 1;             // Will generate conditional branch
    } else {
        c = b + 1;             // Will generate conditional branch
    }
    
    // Test function calls (for branch with link)
    test_func(a, b, &result);
    
    // Test with multiple parameters to exercise stack operations
    c = multi_param_func(1, 2, 3, 4, 5, 6, 7, 8);
    
    // Compare operations
    if (a == b) c++;           // CMP
    if ((a + b) == 12) c++;    // CMN
    if ((a & b) == 5) c++;     // TST
    
    return c;  // Return a value derived from our calculations
}

