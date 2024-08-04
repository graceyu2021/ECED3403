;
; Structure example
; ECED 3403
; 25 Jun 2024
;
; struct stex
; {
;	unsigned short V0;
;	char V1;
;	short V2;
; };
;
; struct stex S1, S2;
;
; Define offsets
;
V0	equ	$0
V1	equ	$2
V2	equ	$4
V3	equ	$-1
;
; Data - reserve space for structures
;
	DATA
	org	#100
S1	bss	$6	; 6 bytes
	bss	$2	; Fill
S2	bss	$6	; 6 bytes
;
; Code - initialize S1
;
	CODE
	org	#1000
;
StrtEx	movlz	S1,R0
	movh	S1,R0
;
; Initialize S1
;
	movh	#1100,R1
	movl	#11,R1
	str	R1,R0,V0
;
	movlz	'A',R1
	str.b	R1,R0,V1
;
	movlz	#FF,R1
	str	R1,R0,V2
;
; Initialize S2 from S1
;
	mov	R0,R2
	add	#8,R2	; Addr of S2
;
; s2.v0 <- s1.v0 + 2
;
	ldr	R0,V0,R1
	add	#2,R1
	str	R1,R2,V0
;
; S2.v1 <- S1.v1 + 1
;
	ldr.b	R0,V1,R1
	add	#1,R1
	str.b	R1,R2,V1
;
; S2.v2 <- S1.v2 - 2
;
	add	$4,R2
	add	$1,R2
	ldr	R0,V2,R1
	sub	#2,R1
	str	R1,R2,V3
;
Done
	end	StrtEx
	
