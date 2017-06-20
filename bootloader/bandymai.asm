[BITS 16]

ORG 0x7C00

jmp START

;spausdina ax desimtainiu formatu
PrintDec:
 pusha
 mov cx, 0 ;skaitmenu skaitliukas
 mov dx, 0 ;reikia, kad butu 0, nes DIV daro taip DX:AX / operandas
 divide:
    mov bx, 10
    div bx
    add cx, 1
    push dx ;skaitmuo dedamas i steka
    mov dx, 0
    cmp ax, 0
    jne divide
 print:
    pop dx  ;skaitmuo imamas is steko
    sub cx, 1
    add dx, 30h ;gaunamas skaiciaus ASCII simbolis
    mov si, sp
    mov [si], dl
    call print_bios
    cmp cx, 0
    jne print
 popa
ret

keaboard_handler:
    pusha
    mov si, keyMsg
    call print_bios

    in  al, 0x60
    call print_keyCode
    mov al, 0x20
    out 0x20, al
    popa
iret

kernel_handler:
    pusha
    push es
    xor ax, ax
    mov es, ax
    mov si, welcomeMsg
    call print_bios
    pop es
    popa
iret

print_keyCode:

ret

clear_scr:
    pusha
    push es
    mov ax, 0xB800
    mov es, ax
    mov si, 0
    keepClearing:
        mov byte [es:si], ' '
        inc si
        mov byte [es:si], 0
        inc si
        cmp si, 4000     ;80*25*2
        jl keepClearing

    pop es
    popa
ret

print_vram:
    pusha
    push es
        mov ax, 0xB800
        mov es, ax

        mov di, 0
        print_vramLoop:
            mov al, [si]
            cmp al, 0
            je print_vramEnd
            mov byte [es:di], al
            mov byte [es:di + 1], 3 ;white
            add di, 2
            inc si
            jmp print_vramLoop

    print_vramEnd:
        pop es
        popa
ret

print_bios:
    pusha
    print_loop:
        lodsb
        or al, al  ;nustatomas zf
        jz print_loopEnd
        mov ah, 0x0E
        int 0x10
        jmp print_loop

    print_loopEnd:
    popa
ret

;skaitoma i [es:bx]
read_2ndLoader:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 4 ;sektoriu skaicius
    mov ch, 0 ;cilindtas
    mov cl, 2 ;sektorius
    mov dh, 0 ;galvute
    mov dl, 0 ;floppy drive number
    int 0x13
    jnc read_2ndLoaderEnd

    mov si, floppyErrorMsg
    call print_bios

    read_2ndLoaderEnd:
        popa
ret

START:
    xor ax, ax ;ax = 0
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov sp, 0xAC00 ;steko virsune, 300 baitu

    mov si, startMsg
    call print_bios
    mov bx, 0xAD00
    call read_2ndLoader

    cli
    mov si, 9
    shl si, 2   ;9 * 4
    mov [es:si], word keaboard_handler
    mov [es:si + 2], cs

    mov si, 0x80
    shl si, 2   ;21 * 4
    mov [es:si], word kernel_handler
    mov [es:si + 2], cs
    sti

    call 0x0000:0xAD00
    hang:
    jmp hang

startMsg db 'ADOS 2011 :D', 13, 10, 0
keyMsg db 'Key pressed', 13, 10, 0
floppyErrorMsg db 'Error reading kernel from flopy!', 13, 10, 0
welcomeMsg db 'Greetings C kernel!', 13, 10, 0

times 510-($-$$) db 0
db 0x55
db 0xAA

;times 1474560-($-$$) db 0  ;1.44 MB
