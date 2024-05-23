GLOBAL get_hours, get_minutes, get_seconds

section .text

get_hours:
	cli ; disable interrupts

	mov al, 0x04 ; read hours
	out 70h, al
	in al, 71h

	sti ; enable interrupts
	ret

get_minutes:
	cli ; disable interrupts

	mov al, 0x02 ; read minutes
	out 70h, al
	in al, 71h

	sti ; enable interrupts
	ret

get_seconds:
	cli ; disable interrupts

	mov al, 0x00 ; read seconds
	out 70h, al
	in al, 71h

	sti ; enable interrupts
	ret