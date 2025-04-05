Program_name="test"
make clean
make
./B2H_CONVERTER ${Program_name}.bin ${Program_name}.hex
cp ${Program_name}.hex ../${Program_name}.hex
