LOAD R0, 5
CALL factorial
PUSH R1
LOAD R0, 10
CALL sum_n
POP_REG R2
PRINT_REG R1
PRINT_REG R2
HALT

[factorial]
LOAD R1, 1
LOAD R2, 1
[fact_loop]
MUL_REG R1, R0
SUB_REG R0, R2
JNZ fact_loop
RET

[sum_n]
LOAD R1, 0
LOAD R2, 1
[sum_loop]
ADD_REG R1, R0
SUB_REG R0, R2
JNZ sum_loop
RET
