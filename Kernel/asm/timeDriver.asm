GLOBAL get_time

section .text

get_hour:
	push rbp
	mov rbp, rsp			    ; armado del stack frame
	
    push rdx
    push rax

	mov al, 04				    ; codigo para pedir horas
	out 70h, al
	in al, 71h				    ; guardamos las horas en al

	mov cl, al					; guardamos el valor en cl para separar decena y unidad

	mov ah, 0					; nos aseguramos que la parte alta de ax esta en 0 para el shift
	shr al, 4					; guardamos la decena en al 

	and cl, 0b00001111			; guardamos la unidad

	mov rdx, time

    call number_to_char         ; en time[0] = decena hora

	inc rdx
    mov al, cl         

    call number_to_char         ; en time[1] = unidad hora         

    pop rax
	pop rdx

	mov rsp, rbp			    ; desarmado del stack frame
	pop rbp

	ret

get_minute:
	push rbp
	mov rbp, rsp			; armado del stack frame

    push rdx
    push rax

	mov al, 02				    ; codigo para pedir minutos
	out 70h, al
	in al, 71h				    ; guardamos los minutos en al

	mov cl, al					; guardamos el valor en cl para separar decena y unidad

	mov ah, 0					; nos aseguramos que la parte alta de ax esta en 0 para el shift
	shr al, 4					; guardamos la decena en al 

	and cl, 0b00001111			; guardamos la unidad

	mov rdx, time
	add rdx, 3

    call number_to_char         ; en time[0] = decena minuto		

	inc rdx
    mov al, cl

	call number_to_char         ; en time[1] = unidad minuto	


	pop rax
    pop rdx

	mov rsp, rbp			    ; desarmado del stack frame
	pop rbp

	ret

get_time:
	push rbp
	mov rbp, rsp

	call get_hour

	mov rax, time
	add rax, 2
	mov byte [rax], ':'				; ponemos en time[2] = ':'
	
	call get_minute

	mov rax, time

	mov rsp, rbp
	pop rbp

	ret

; recibe en rdx una posicion de memoria y en al un numero
; setea en la posicion de memoria el valor el numero en ascii
number_to_char:
    push rbp
    mov rbp, rsp

    mov byte [rdx], '0'
    add [rdx], al

    mov rsp, rbp
    pop rbp
    
    ret


section .bss
	time resb 5
