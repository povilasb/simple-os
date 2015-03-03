[EXTERN int0xUnknown_handler]
[EXTERN int0x80_handler]
[EXTERN int0x20_handler]
[EXTERN int0x21_handler]
[EXTERN int0x8_handler]
[EXTERN int0xB_handler]
[EXTERN int0xC_handler]
[EXTERN int0xD_handler]
[EXTERN int0xE_handler]
[EXTERN int0xF_handler]

[GLOBAL isr0xUnknown]
isr0xUnknown:
    cli  
    pusha
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0xUnknown_handler
    
    mov eax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
iret 

[GLOBAL isr0x80]
isr0x80:
    cli  
    pusha
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0x80_handler
    
    mov eax, 0x10
    mov ss, ax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    popa
    sti
iret 

[GLOBAL isr0x20]
isr0x20:
    cli   
    pusha
    mov ax, ds
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0x20_handler
    
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0x21]
isr0x21:
    cli   
    pusha
    mov ax, ds
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0x21_handler
    
    mov eax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0x8] ;double fault
isr0x8: 
    cli
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0x8_handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0xB] ;segment not present, pushes error code
isr0xB: 
    cli
    pusha
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0xB_handler
    
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0xC] ;stack fault
isr0xC: 
    cli
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0xC_handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0xD] ;general protection fault
isr0xD: 
    cli
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0xD_handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0xE] ;page fault, pushes error code
isr0xE: 
    cli
    mov ebp, esp
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    mov eax, cr2
    push eax
    call int0xE_handler
    
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 

[GLOBAL isr0xF] ;unknown interrupt
isr0xF: 
    cli
    pusha
    mov ax, ds
    push eax
    mov ax, 0x10 ;data segment
    mov ds, ax
    mov es, ax
    mov gs, ax
    mov fs, ax
    mov ss, ax
    
    call int0xF_handler
    
    pop eax
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    popa
    sti
iret 