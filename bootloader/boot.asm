[BITS 16]

ORG 0x7C00

jmp START

print_bios:
    pusha 
    print_loop:
        lodsb
        or al, al  ;set ZF
        jz print_loopEnd    
        mov ah, 0x0E
        int 0x10
        jmp print_loop
    
    print_loopEnd:
    popa
ret

a20_enable:
    pusha
    cli
    
    call a20_waitInput
    mov al, 0xAD ;disable keyboard
    out 0x64, al
    
    call a20_waitInput
    mov al, 0xD0 ;copy command byte to 0x60 port
    out 0x64, al
    
    ;call a20_waitOutput
    in al, 0x60
    push ax ;save command byte
    
    call a20_waitInput
    mov al, 0xD1 ;next written port to 0x60 is written to 0x64
    out 0x64, al
    
    pop ax
    or al, 2 ;enable a20
    call a20_waitInput
    out 0x64, al
    
    call a20_waitInput
    mov al, 0xAE ;enable keyboard
    out 0x64, al
    
    call a20_waitInput
    
    sti
    popa
ret

a20_waitOutput: ;wait until keyboard output buffer isn't empty
    pusha
    a20_waitOuputLoop:
    in al, 0x64
    test al, 1
    jnz a20_waitOuputLoop
    popa
ret

a20_waitInput: ;wait until keyboard input buffer isn't empty
    pusha
    a20_waitInputLoop:
    in al, 0x64
    test al, 2
    jnz a20_waitInputLoop
    popa
ret

;reading to [es:bx]
read_kernel1:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 17 ;number of sectors
    mov ch, 0 ;cylinder
    mov cl, 2 ;sector
    mov dh, 0 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel1End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel1End:
        popa
        add bx, 0x2200 ;bx = bx + 512 * 17
ret

;reading to [es:bx]
read_kernel2:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 18 ;number of sectors
    mov ch, 0 ;cylinder
    mov cl, 1 ;sector
    mov dh, 1 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel2End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel2End:
        popa
        add bx, 0x2400 ;bx = bx + 512 * 18
ret

;reading to [es:bx]
read_kernel3:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 18 ;number of sectors
    mov ch, 1 ;cylinder
    mov cl, 1 ;sector
    mov dh, 0 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel3End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel3End:
        popa
        add bx, 0x2400 ;bx = bx + 512 * 18
ret

;reading to [es:bx]
read_kernel4:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 18 ;number of sectors
    mov ch, 1 ;cylinder
    mov cl, 1 ;sector
    mov dh, 1 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel4End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel4End:
        popa
        add bx, 0x2400 ;bx = bx + 512 * 18
ret

;reading to [es:bx]
read_kernel5:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 18 ;number of sectors
    mov ch, 2 ;cylinder
    mov cl, 1 ;sector
    mov dh, 0 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel5End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel5End:
        popa
        add bx, 0x2400 ;bx = bx + 512 * 18
ret

;reading to [es:bx]
read_kernel6:
    pusha
    mov ah, 2 ;read sector into memory
    mov al, 18 ;number of sectors
    mov ch, 2 ;cylinder
    mov cl, 1 ;sector
    mov dh, 1 ;head
    mov dl, [externalDiskDrive] ;floppy drive number
    int 0x13
    jnc read_kernel6End
    
    mov si, floppyErrorMsg
    call print_bios
    
    read_kernel6End:
        popa
        add bx, 0x2400 ;bx = bx + 512 * 18
ret

START:
    xor ax, ax ;ax = 0
    mov es, ax
    mov ds, ax
    mov ss, ax
    mov sp, 0x700 ;stack 512 bytes
    
    ;mov si, welcomeMsg
    ;call print_bios
    
    call a20_enable
    
    mov bx, 0x7E00
    call read_kernel4
    call read_kernel5
    call read_kernel6
    
    mov edi, 0x500
    mov esi, 0x7E00    ;kernel source code
    mov ecx, 0x6C00     ;27 kB
    rep movsb
    
    mov bx, 0x7E00
    call read_kernel1
    call read_kernel2
    call read_kernel3
    
    cli
    lgdt [gdt_desc]
    mov eax, cr0
    or eax, 1
    mov cr0, eax ;switch to protected mode
    ;sti
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    mov esp, [KERNEL_STACK_POINTER]
    jmp 0x8:protected_modeStart
    
[BITS 32]
protected_modeStart:
    mov edi, [KERNEL_START_ADDR] ;kernel memory
    mov esi, 0x7E00   ;kernel source code
    mov ecx, 0x6A00     ;26,5 kB
    rep movsb
    
    mov esi, 0x500
    mov ecx, 0x6C00 ;27 kB
    rep movsb
    
    cli
    jmp [KERNEL_START_ADDR]
    
    
externalDiskDrive db 0x0
welcomeMsg db "SOS 2011 bootloader", 13, 10, 0
floppyErrorMsg db "Error reading kernel from floppy!", 13, 10, 0

KERNEL_START_ADDR dd 0x6400000
KERNEL_STACK_POINTER dd 0x6504FFF

gdt_start:

gdt_null:
    dd 0
    dd 0
    
gdt_code:
    dw 0xFFFF ;segment limiter
    dw 0x0000 ;base
    db 0x00 ;base
    db 10011010b
    db 11001111b
    db 0x0 ;base
    
gdt_data:
    dw 0xFFFF ;segment limiter
    dw 0x0000 ;base
    db 0x00 ;base
    db 10010010b
    db 11001111b
    db 0x0 ;base   
    
gdt_vram:
    dw 0xFFFF ;segment limiter
    dw 0x8000 ;base
    db 0x0B ;base
    db 10010010b
    db 11001111b
    db 0x0 ;base 

gdt_end:

gdt_desc: ;gdt descriptor
    dw gdt_end - gdt_start - 1
    dd gdt_start

times 510-($-$$) db 0
db 0x55
db 0xAA