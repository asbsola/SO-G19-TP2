
GLOBAL _cli, _sti, picMasterMask, picSlaveMask, haltcpu, _hlt

GLOBAL _irq00Handler, _irq01Handler, _int80Handler

GLOBAL _exception0Handler, _exception6Handler

EXTERN irqDispatcher
EXTERN softIntDispatcher
EXTERN exceptionDispatcher

SECTION .text

%macro pushState 0
	push rbx
	push rcx
	push rdx
	push rbp
	push rdi
	push rsi
	push r8
	push r9
	push r10
	push r11
	push r12
	push r13
	push r14
	push r15
	push rax
%endmacro

%macro popState 0
	pop rax
	pop r15
	pop r14
	pop r13
	pop r12
	pop r11
	pop r10
	pop r9
	pop r8
	pop rsi
	pop rdi
	pop rbp
	pop rdx
	pop rcx
	pop rbx
%endmacro

%macro irqHandlerMaster 1
	pushState

	mov rdi, %1 ; pasaje de parametro
	call irqDispatcher

	; signal pic EOI (End of Interrupt)
	mov al, 20h
	out 20h, al

	popState
	iretq
%endmacro

%macro exceptionHandler 1
	pushState

	; rax		<- rsp
	; r15
	; r14
	; r13
	; r12
	; r11
	; r10
	; r9
	; r8
	; rsi
	; rdi
	; rbp
	; rdx
	; rcx
	; rbx

	; rip
	; cs
	; rflags
	; ss:rsp

	; Quiero pasar todos los registros como argumentos
	; exceptionDispatcher(exceptionCode, rax, rbx, rcx, rdx, rsi, rdi, rbp, r8, r9, r10, r11, r12, r13, r14, r15, rip, ss_rsp)


	mov rax, rsp
	
	mov rdi, %1 ; pasaje de parametro
	mov rsi, [rax]			; Pasaje de rax
	mov rdx, [rax+8*14] 	; Pasaje de rbx
	mov rcx, [rax+8*13]		; Pasaje de rcx
	mov r8, [rax+8*12]		; Pasaje de rdx
	mov r9, [rax+8*9]		; Pasaje de rsi

	mov rbx, [rax+8*18]		; Pasaje de ss:rsp
	push rbx
	
	mov rbx, [rax+8*15]		; Pasaje de rip
	push rbx

	mov rbx, [rax+8*1]      ; Pasaje de r15
	push rbx

	mov rbx, [rax+8*2]      ; Pasaje de r14
	push rbx

	mov rbx, [rax+8*3]      ; Pasaje de r13
	push rbx

	mov rbx, [rax+8*4]      ; Pasaje de r12
	push rbx

	mov rbx, [rax+8*5]      ; Pasaje de r11
	push rbx

	mov rbx, [rax+8*6]      ; Pasaje de r10
	push rbx

	mov rbx, [rax+8*7]      ; Pasaje de r9
	push rbx

	mov rbx, [rax+8*8]      ; Pasaje de r8
	push rbx

	mov rbx, [rax+8*11]     ; Pasaje de rbp
	push rbx

	mov rbx, [rax+8*10]     ; Pasaje de rdi
	push rbx

	call exceptionDispatcher

	add rsp, 96				; 12 registros * 8 bytes

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
	pushState
	pop rax ; No preservamos rax
	; Habria que pasar los parametros rax, rdi, rsi, rdx, r10, r8, r9
	push r9
	mov r9, r8
	mov r8, r10
	mov rcx, rdx
	mov rdx, rsi
	mov rsi, rdi
	mov rdi, rax
	call softIntDispatcher
	add rsp, 8
	push rax
	popState
	iretq

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
