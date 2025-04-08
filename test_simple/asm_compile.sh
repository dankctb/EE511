
arm-none-eabi-as -mcpu=cortex-m0 -mthumb -o asm_test.o asm_test.s
arm-none-eabi-ld -Tlinkerscript.ld -o asm_test.elf asm_test.o
arm-none-eabi-objcopy -O binary asm_test.elf asm_test.bin
./B2H_CONVERTER asm_test.bin asm_test.hex
cp asm_test.hex ../asm_test.hex
