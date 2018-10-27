%include "io.inc"

section .bss
pointer	resb 5 ;

section .text
global CMAIN

CMAIN:
    mov edi, pointer
    mov byte [pointer+6], 0xA; установить границу

_input_loop:
    GET_CHAR al
    cmp al, 0xA; проверка конца строки '\n'
    je _pop_loop

_push:
    mov [edi], al; запись в стек
    inc edi
    cmp byte [pointer+6], 0xA; проверка сохранности ограничителя
    jne _error
    jmp _input_loop

_pop_loop:
    dec edi; вывод стека
    PRINT_CHAR [edi]
    cmp edi,pointer
    jne _pop_loop
    Ret

_error:
   PRINT_STRING "[ERROR] stack overflow"
   NEWLINE
   RetÒ