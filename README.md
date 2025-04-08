# ARM Cortex-M0 Instruction Set Simulator

This is term project 1 of EE511 in KAIST Spring 2025, which requires implementing an instruction set simulator for the ARM Cortex-M0 processor in thread mode. 

## Simulator Limitations

The simulator only implements thread mode operations as specified in the project requirements. It does not include:
- Handler mode operations
- Memory barrier instructions (DMB, DSB, ISB)
- SVC instructions
- Privilege mode operations

## Running the Simulator

To run the simulator with the test program:

```bash
./run.sh
```

This will execute the test.c program through the simulator and show the results.

## Simulator Commands

The ARM Cortex-M0 simulator provides an interactive interface with the following commands:

| Command | Description | Example | Output |
|---------|-------------|---------|--------|
| `s` | Process a single instruction | `s` | Executes one instruction and displays register values, flags, and PC |
| `m N` | Process multiple instructions | `m 10` | Executes N instructions and displays final state |
| `b ADDR` | Run until breakpoint | `b 120` | Executes until PC reaches the specified hex address |
| `v ADDR` | View memory contents | `v 10000` | Displays memory content at the specified hex address |
| `q` | Quit simulator | `q` | Exits the program |

### Execution Output

After executing an instruction, the simulator displays:

```
Inst: XXXX                          # The instruction opcode that was executed
Current PC: XXXXXXXX  Executed PC: XXXXXXXX  # Current and previously executed PC values

Registers                           # Register values after instruction execution
[0] XXXXXXXX [1] XXXXXXXX [2] XXXXXXXX [3] XXXXXXXX
[4] XXXXXXXX [5] XXXXXXXX [6] XXXXXXXX [7] XXXXXXXX
[8] XXXXXXXX [9] XXXXXXXX [10]XXXXXXXX [11]XXXXXXXX
[12]XXXXXXXX [13]XXXXXXXX [14]XXXXXXXX [15]XXXXXXXX

NZCV: XXXX                          # Condition flags state (Negative, Zero, Carry, oVerflow)
```

### Example Execution Session

Running a single instruction:
```
Request: s

Inst: 2100
Current PC: 00000004  Executed PC: 00000000
Registers
[ 0]00000001 [ 1]00000000 [ 2]00000000 [ 3]00000000
[ 4]00000000 [ 5]00000000 [ 6]00000000 [ 7]00000000
[ 8]00000000 [ 9]00000000 [10]00000000 [11]00000000
[12]00000000 [13]00010000 [14]00000000 [15]00000004
NZCV: 0000

===============================================

```

Running until a breakpoint:
```
Request: b 20
...execution continues...

Inst: XXXX
Current PC: 00000020  Executed PC: 0000001C
...register state displayed...
```

Viewing memory contents:
```
Request: v 10000
10000 : 00010000
```

## Test Program and Instruction Validation

The test program (test.c) systematically exercises each instruction type implemented in the simulator. Below is a validation guide for each instruction category:

### Data Processing Instructions

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| ADDS | `c = a + b` | 5 + 7 = 12 |
| SUBS | `c = a - b` | 5 - 7 = -2 |
| MULS | `c = a * b` | 5 * 7 = 35 |
| ANDS | `c = a & b` | 5 & 7 = 5 |
| ORRS | `c = a \| b` | 5 \| 7 = 7 |
| EORS | `c = a ^ b` | 5 ^ 7 = 2 |
| MVNS | `c = ~a` | ~5 = -6 |
| NEGS/RSB | `c = -a` | -5 = -5 |
| BICS | `c = bit_clear(0xFFFF, 0x00F0)` | 0xFFFF & ~0x00F0 = 0xFF0F |

### Shift Operations

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| LSLS (immediate) | `c = a << 2` | 5 << 2 = 20 |
| LSRS (immediate) | `c = a >> 2` | 5 >> 2 = 1 |
| ASRS (immediate) | `c = ((int32_t)0x80000000) >> 4` | Sign-extended result = 0xF8000000 |
| LSLS (register) | `c = a << b` | 5 << 3 = 40 |
| LSRS (register) | `c = a >> b` | 5 >> 3 = 0 |
| ASRS (register) | `c = ((int32_t)0x80000000) >> b` | Sign-extended result = 0xF0000000 |

### Memory Operations

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| STR | `g_data[0] = 0x12345678` | Memory at g_data[0] contains 0x12345678 |
| LDR | `c = g_data[1]` | c contains value from g_data[1] |
| STRB | `g_bytes[0] = 0xAA` | Memory at g_bytes[0] contains 0xAA |
| LDRB | `c = g_bytes[1]` | c contains byte value from g_bytes[1] |
| STRH | `g_halfwords[0] = 0xBEEF` | Memory at g_halfwords[0] contains 0xBEEF |
| LDRH | `c = g_halfwords[1]` | c contains halfword value from g_halfwords[1] |

### Byte Reversal Instructions

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| REV | `c = rev_bytes(0x12345678)` | c = 0x78563412 |
| REV16 | `c = rev16_bytes(0x12345678)` | c = 0x34127856 |
| REVSH | `c = rev_sign_extend(0x1234)` | c = 0xFFFF3412 (sign-extended) |

### Extension Instructions

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| SXTB | `c = (int32_t)byte_val` | -5 sign-extended to 32 bits = 0xFFFFFFFB |
| SXTH | `c = (int32_t)half_val` | -1000 sign-extended to 32 bits = 0xFFFFFC18 |
| UXTB | `c = (uint32_t)(uint8_t)byte_val` | -5 zero-extended to 32 bits = 0x000000FB |
| UXTH | `c = (uint32_t)(uint16_t)half_val` | -1000 zero-extended to 32 bits = 0x0000FC18 |

### Branch and Control Flow

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| B (unconditional) | `b loop` | Program jumps to loop label |
| B (conditional) | `if (a > b) { ... } else { ... }` | Program takes the 'else' branch (since 5 < 7) |
| BL | `test_func(a, b, &result)` | Program calls test_func and correctly returns |

### Stack Operations

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| PUSH | Inside `multi_param_func` | Stack correctly stores registers |
| POP | Inside `multi_param_func` | Stack correctly restores registers |

### Compare Instructions

| Instruction | Test Case in test.c | Expected Behavior |
|-------------|---------------------|-------------------|
| CMP | `if (a == b)` | Sets flags for a == b comparison (false in this case) |
| CMN | `if ((a + b) == 12)` | Sets flags for (a + b) == 12 comparison (true in this case) |
| TST | `if ((a & b) == 5)` | Sets flags for (a & b) == 5 comparison (true in this case) |

