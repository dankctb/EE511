# ARM Cortex-M0 Instruction Set Simulator

This is term project 1 of EE511 in KAIST Spring 2025, which requires implementing an instruction set simulator for the ARM Cortex-M0 processor in thread mode. 

## Simulator Limitations

The simulator only implements thread mode operations as specified in the project requirements. It does not include:
- Handler mode operations
- Memory barrier instructions (DMB, DSB, ISB)
- SVC instructions
- Privilege mode operations

In addition, as requirement, all the relevant code should be put in ’thumb.c’, and ‘iss.h’ so these files are quite long ...

## Running the Simulator

To run the simulator with the test program:

```bash
cd test_simple/
bash compile.sh
cd ..
bash run.sh
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

