;
; Stack frame example
; ECED 3403
; 25 Jun 2024
;
; call subr(d0, d1, d2);
;
; func subr(p0, p1)
; auto a0, a1;
;
BP	equ	R4
LR	equ	R5
SP	equ	R6
PC	equ	R7
;
; Offsets into stack
;
A1off	equ	$-2
A0off	equ	$0
BPoff	equ	$+2
D0off	equ	$+4
D1off	equ	$+6
;
; Stack in data memory
;
	data
	org	#80
	bss	#7E
StkStrt	bss	#2	; #FE
;
; Caller's parameters
;
D0	word	#D0D0
D1	word	#D1D1
D2	word	#D2D2
;
	code
	org	#1000
;
; Initialize SP
;
SFEx
	movl	StkStrt,SP
	movh	StkStrt,SP
;
; Assign value to BP (R4)
;
	movls	#FF,BP
;
; call subr(data0, data1);
;
	movl	D2,R0
	movh	D2,R0
;
	ld	R0-,R1
	st	R1,SP-
;
	ld	R0-,R1
	st	R1,SP-
;
	ld	R0,R1
	st	R1,SP-
;
	bl	subr
;
; Restore SP
;
	add	#4,SP
	add	#2,SP
;
EndOfEx
;
; Subroutine
;
subr
	st	BP,SP-	; Caller's BP
;
	mov	SP,BP	; 
;
; Reserve 2 words for autos
;
	sub	#4,SP
;
	ldr	BP,D0off,R1
	str	R1,BP,A0off
;
	ldr	BP,D1off,R1
	str	R1,BP,A1off
;
; Some processing
; Done...
;
	mov	BP,SP	; Restore SP
;
	ld	+SP,BP	; Restore BP
;
	mov	LR,PC	; Return
;
	end	SFEx
