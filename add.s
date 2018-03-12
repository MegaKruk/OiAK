EXIT_SUCCESS= 0
SYSEXIT=1
.data
liczba1: .long 0x10304008, 0x701100FF, 0x45100020, 0x08570030
sizeofliczba1 = . - liczba1
 
liczba2: .long 0xF040500C, 0x00220026, 0x321000CB, 0x04520031
sizeofliczba2 = . - liczba2
 
 
.text
.global _start
_start:
   
    clc
    mov $sizeofliczba1, %eax
    mov $4, %ebx
    div %ebx
 
    movl %eax, %esi
 
    mov $sizeofliczba2, %eax
    mov $4, %ebx
    div %ebx
 
    movl %eax, %edi
 
    cmpl %edi, %esi
    jle ustaw_licznik
    movl %edi, %ecx
 
dodaj:
    clc
petla:
    decl %esi
    decl %edi
    movl liczba1(,%esi,4),%eax
    movl liczba2(,%edi,4),%ebx
    adcl %eax,%ebx
    pushl %ebx
loop petla
    jc uzupelnijzprzeniesieniem
 
    cmpl $0, %esi
    jne uzupelnij1
 
    cmpl $0, %edi
    jne uzupelnij2
 
    jmp kontynuuj
 
uzupelnijzprzeniesieniem:
 
    cmpl $0, %esi
    jne uzupelnij1zprzeniesieniem
 
    cmpl $0, %edi
    jne uzupelnij2zprzeniesieniem
 
    jmp kontynuujzprzeniesieniem
 
kontynuujzprzeniesieniem:
    pushl $1
    jmp zakoncz
 
kontynuuj:
    jc kontynuujzprzeniesieniem
    pushl $0
    jmp zakoncz
 
ustaw_licznik:
    mov %esi, %ecx
    jmp dodaj
 
uzupelnij1:
    movl %esi, %ecx
petla2:
    decl %esi
    movl liczba1(,%esi,4),%eax
    adcl $0, %eax
    pushl %eax
loop petla2
    jmp kontynuuj
 
uzupelnij2:
    movl %edi, %ecx
petla3:
    decl %edi
    movl liczba2(,%edi,4),%eax
    adcl $0, %eax
    pushl %eax
loop petla3
    jmp kontynuuj
 
uzupelnij1zprzeniesieniem:
    stc
    movl %esi, %ecx
petla4:
    decl %esi
    movl liczba1(,%esi,4),%eax
    adcl $0, %eax
    pushl %eax
loop petla4
    jmp kontynuuj
 
uzupelnij2zprzeniesieniem:
    stc
    movl %edi, %ecx
petla5:
    decl %edi
    movl liczba2(,%edi,4),%eax
    adcl $0, %eax
    pushl %eax
loop petla5
    jmp kontynuuj
 
zakoncz:
    #movl $0, %eax
    popl %eax
    popl %eax
    popl %eax
    popl %eax
    popl %eax
    mov $SYSEXIT, %eax
    mov $EXIT_SUCCESS, %ebx
    int $0x80
