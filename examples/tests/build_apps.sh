make clean
make -j20
cp test-runner.riscv ../../../build/overlay/root 
cp -r */*.eapp_riscv ../../../build/overlay/root 