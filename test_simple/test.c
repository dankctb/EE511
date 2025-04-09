/*********************************************************
 *                                                       *
 *  EE511 Project 1                                      *
 *                                                       *
 *  C test code                                          *
 *                                                       *
 *********************************************************/

/*your test code here*/
#include <stdint.h>
#include <stdio.h>

// Global variables for memory operations testing
volatile uint32_t g_data[16];
volatile uint8_t g_bytes[16];
volatile uint16_t g_halfwords[16];

// Function to print operation results for comparison
void print_result(const char* op_name, int result) {
    printf("%s: %d (0x%08X)\n", op_name, result, result);
}

// Function for testing branches
void test_func(int a, int b, int *result) {
    *result = a + b;
    print_result("test_func result", *result);
}

// Function with many parameters to test stack operations
int multi_param_func(int a, int b, int c, int d, int e, int f, int g, int h) {
    int result = a + b + c + d + e + f + g + h;
    print_result("multi_param_func result", result);
    return result;
}

// Bit manipulation functions to exercise various instructions
uint32_t bit_clear(uint32_t val, uint32_t mask) {
    uint32_t result = val & (~mask);
    print_result("bit_clear result", result);
    return result;  // BIC operation
}

uint32_t rev_bytes(uint32_t value) {
    uint32_t result = ((value & 0xFF) << 24) | 
           ((value & 0xFF00) << 8) | 
           ((value & 0xFF0000) >> 8) | 
           ((value & 0xFF000000) >> 24);
    print_result("rev_bytes result", result);
    return result;
}

uint32_t rev16_bytes(uint32_t value) {
    uint32_t result = ((value & 0xFF) << 8) | 
           ((value & 0xFF00) >> 8) | 
           ((value & 0xFF0000) << 8) | 
           ((value & 0xFF000000) >> 8);
    print_result("rev16_bytes result", result);
    return result;
}

uint32_t rev_sign_extend(uint32_t value) {
    uint16_t half = ((value & 0xFF) << 8) | ((value & 0xFF00) >> 8);
    uint32_t result;
    // Sign extend
    if (half & 0x8000) {
        result = half | 0xFFFF0000;
    } else {
        result = half;
    }
    print_result("rev_sign_extend result", result);
    return result;
}

int main(void) {
    int result = 0;
    int a = 5, b = 7, c = 0;
    
    printf("Starting test with a=%d, b=%d\n", a, b);
    
    // Test data processing instructions
    c = a + b;                 // ADDS
    print_result("ADDS (a + b)", c);
    
    c = a - b;                 // SUBS
    print_result("SUBS (a - b)", c);
    
    c = a * b;                 // MULS
    print_result("MULS (a * b)", c);
    
    c = a & b;                 // ANDS
    print_result("ANDS (a & b)", c);
    
    c = a | b;                 // ORRS
    print_result("ORRS (a | b)", c);
    
    c = a ^ b;                 // EORS
    print_result("EORS (a ^ b)", c);
    
    c = ~a;                    // MVNS
    print_result("MVNS (~a)", c);
    
    c = -a;                    // NEGS (RSB #0)
    print_result("NEGS (-a)", c);
    
    // Test bit manipulation operations
    c = bit_clear(0xFFFF, 0x00F0);  // BIC (should be 0xFF0F)
    print_result("BIC (0xFFFF & ~0x00F0)", c);
    
    // Test shift operations
    c = a << 2;                // LSLS immediate
    print_result("LSLS immediate (a << 2)", c);
    
    c = a >> 2;                // LSRS immediate
    print_result("LSRS immediate (a >> 2)", c);
    
    c = ((int32_t)0x80000000) >> 4;  // ASRS immediate
    print_result("ASRS immediate (0x80000000 >> 4)", c);
    
    b = 3;
    printf("Setting b=%d for shift tests\n", b);
    
    c = a << b;                // LSLS register
    print_result("LSLS register (a << b)", c);
    
    c = a >> b;                // LSRS register
    print_result("LSRS register (a >> b)", c);
    
    c = ((int32_t)0x80000000) >> b;  // ASRS register
    print_result("ASRS register (0x80000000 >> b)", c);
    
    // Test memory operations
    g_data[0] = 0x12345678;    // STR
    print_result("STR to g_data[0]", g_data[0]);
    
    c = g_data[1];             // LDR
    print_result("LDR from g_data[1]", c);
    
    g_bytes[0] = 0xAA;         // STRB
    print_result("STRB to g_bytes[0]", g_bytes[0]);
    
    c = g_bytes[1];            // LDRB
    print_result("LDRB from g_bytes[1]", c);
    
    g_halfwords[0] = 0xBEEF;   // STRH
    print_result("STRH to g_halfwords[0]", g_halfwords[0]);
    
    c = g_halfwords[1];        // LDRH
    print_result("LDRH from g_halfwords[1]", c);
    
    // Byte reversal functions (REV, REV16, REVSH)
    c = rev_bytes(0x12345678);      // REV
    print_result("REV (0x12345678)", c);
    
    c = rev16_bytes(0x12345678);    // REV16
    print_result("REV16 (0x12345678)", c);
    
    c = rev_sign_extend(0x1234);    // REVSH
    print_result("REVSH (0x1234)", c);
    
    // Sign/zero extension
    int8_t byte_val = -5;
    int16_t half_val = -1000;
    
    c = (int32_t)byte_val;     // SXTB
    print_result("SXTB (sign extend byte -5)", c);
    
    c = (int32_t)half_val;     // SXTH
    print_result("SXTH (sign extend half -1000)", c);
    
    c = (uint32_t)(uint8_t)byte_val;  // UXTB
    print_result("UXTB (zero extend byte -5)", c);
    
    c = (uint32_t)(uint16_t)half_val; // UXTH
    print_result("UXTH (zero extend half -1000)", c);
    
    // Flag testing and conditional execution
    if (a > b) {
        c = a + 1;             // Will generate conditional branch
        print_result("IF (a > b) branch taken", c);
    } else {
        c = b + 1;             // Will generate conditional branch
        print_result("ELSE (a <= b) branch taken", c);
    }
    
    // Test function calls (for branch with link)
    test_func(a, b, &result);
    print_result("After test_func call", result);
    
    // Test with multiple parameters to exercise stack operations
    c = multi_param_func(1, 2, 3, 4, 5, 6, 7, 8);
    print_result("After multi_param_func call", c);
    
    // Compare operations
    if (a == b) {
        c++;
        print_result("CMP (a == b) true", c);
    } else {
        print_result("CMP (a == b) false", c);
    }
    
    if ((a + b) == 12) {
        c++;
        print_result("CMN (a + b == 12) true", c);
    } else {
        print_result("CMN (a + b == 12) false", c);
    }
    
    if ((a & b) == 5) {
        c++;
        print_result("TST (a & b == 5) true", c);
    } else {
        print_result("TST (a & b == 5) false", c);
    }
    
    printf("Final result: %d (0x%08X)\n", c, c);
    return c;  // Return a value derived from our calculations
}

