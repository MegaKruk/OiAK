EXIT_SUCCESS= 0
SYSEXIT=1

# rejestr control world - kontrola precyzji bity 8-9 little endian
# 0b00 - single precision, 0b10 - double precision, domyslnie/0b11 - double-extened precision, 01 - not used
# bity 10-11 rounding control
# 00 — round to nearest, 01 — round down (toward negative infinity), 10 — round up (toward positive infinity), 11 — round toward zero
# aby notowac wyjatki nalezy ustawic bity 0-5 na 0

#SPRN=0b0000000001111111     # little endian
#SPRD=0b0000010001111111
#SPRU=0b0000100001111111
#SPRZ=0b0000110001111111

#DPRN=0b0000001001111111
#DPRD=0b0000011001111111
#DPRU=0b0000101001111111
#DPRZ=0b0000111001111111

#DEFA=0b0000001101111111 
#EPRN=0b0000001101111111
#EPRD=0b0000011101111111
#EPRU=0b0000101101111111
#EPRZ=0b0000111101111111

.data
    val0:
        .float 0.0
    val1:
        .float 66.66
    val2:
        .double 123.1234
    val3:
        .float -42.42
    val4:
        .double -321.4321
    val5:
        .float -1.0
    prec:
        .long 0         # 0 single, 2 double
    round:
        .long 0         # 0 round nearest, 1 round down, 2 round up, 3 round to zero

.section .bss
    .lcomm status, 2
    .lcomm control, 2
.text
.global _start
_start:
nop
finit
fstsw status        # check status and control registers
fstcw control

# tutaj wybor prec i round

fld val5
fsqrt                   # NaN, square root

fldz                    # +0.0

fldz
fmul val5               # -0.0, multiplying

fld val1
fdiv val0               # inf, dividing

fld val3
fdiv val0               # -inf, dividing

fldl val2
faddl val4               # adding

fld val2
fsub val1               # substracting


jmp stop

stop:
mov $SYSEXIT, %eax
mov $EXIT_SUCCESS, %ebx
int $0x80
