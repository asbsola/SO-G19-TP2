
GLOBAL _cli, _sti, picMasterMask, picSlaveMask, haltcpu, _hlt

GLOBAL _irq00Handler, _irq01Handler, _int80Handler

GLOBAL _exception0Handler, _exception6Handler

EXTERN irqDispatcher
EXTERN softIntDispatcher
EXTERN exceptionDispatcher

SECTION .text

%macro pushState 0
	push r15
	push r14
	push r13
	push r12
	push r11
	push r10
	push r9
	push r8
	push rsi
	push rdi
	push rbp
	push rdx
	push rcx
	push rbx
	push rax
%endmacro

%macro popState 0
	pop rax
	pop rbx
	pop rcx
	pop rdx
	pop rbp
	pop rdi
	pop rsi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
%endmacro

%macro popStateWithoutRax 0
	add rsp, 8
	pop rbx
	pop rcx
	pop rdx
	pop rbp
	pop rdi
	pop rsi
	pop r8
	pop r9
	pop r10
	pop r11
	pop r12
	pop r13
	pop r14
	pop r15
%endmacro

%macro irqHandlerMaster 1
	pushState
	mov rdi, %1 ; pasaje de parametro
	mov rsi, rsp
	call irqDispatcher
	mov al, 20h	; signal pic EOI (End of Interrupt)
	out 20h, al
	popState
	iretq
%endmacro

%macro intSoftHandlerMaster 0
	pushState
	mov rdi, rsp
	sti
	call softIntDispatcher
	popStateWithoutRax ; Preserving interrupts' return value
	iretq
%endmacro

%macro exceptionHandler 1
	pushState
	mov rdi, %1
	mov rsi, rsp
	call exceptionDispatcher
	popState
	iretq
%endmacro

_hlt:
	sti
	hlt
	ret

_cli:
	cli
	ret


_sti:
	sti
	ret

picMasterMask:
	push rbp
    mov rbp, rsp
    mov ax, di
    out	21h,al
    pop rbp
    retn

picSlaveMask:
	push    rbp
    mov     rbp, rsp
    mov     ax, di  ; ax = mascara de 16 bits
    out	0A1h,al
    pop     rbp
    retn

;8254 Timer (Timer Tick)
_irq00Handler:
	irqHandlerMaster 0

;Keyboard
_irq01Handler:
	irqHandlerMaster 1

;Syscalls
_int80Handler:
	intSoftHandlerMaster

;Zero Division Exception
_exception0Handler:
	exceptionHandler 0

;Invalid Op Code Exception
_exception6Handler:
	exceptionHandler 6

haltcpu:
	cli
	hlt
	ret

SECTION .bss
	aux resq 1
