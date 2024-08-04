;
; Stack example
; XM23 does not have a stack or stack instructions
;
; SP points to top of stack (empty word)
; Operations:
; PUSH: value onto stack (into low mem):
;	mem[SP] <- value
;	SP <- SP - 2
; PULL: value from stack (into high mem):
;	SP <- SP + 2
;	value <- mem[SP]
;
SP	equ	R6	; 'SP' equates to R6
;
	data
;

	org	#70
	bss	#0E	; Reserve 70 to 7C	
StkTop	bss	#1	; Top-of-stack (7E)
;
	code
	org	#100
StkEx
;
; Assign values to R0, R1, R2
;
	movlz	#ff,R0	; R0 <- #00FF
	movls	#ff,R1	; R1 <- #FFFF
	movh	#ff00,R2 ; R2 <- #FF00
;
; SP <- address of StkTop
;
	movlz	StkTop,SP
;
; Push R0, R1, R2
; SP decrement
;
	st	R0,SP-	; mem[SP] <- R0; SP <- SP - 2
	st	R1,SP-	; mem[SP] <- R1; SP <- SP - 2
	st	R2,SP-	; mem[SP] <- R2; SP <- SP - 2
;
; Modify R0, R1, R2 (complement)
;
	xor	$-1,R0
	xor	$-1,R1
	xor	$-1,R2
;
; Restore (pull) R2, R1, R0
;
	ld	+SP,R2	; SP <- SP + 2; R2 <- mem[SP]
	ld	+SP,R1	; SP <- SP + 2; R1 <- mem[SP]
	ld	+SP,R0	; SP <- SP + 2; R0 <- mem[SP]
;
; Breakpoint address
;
BrkPtAdr
	end	StkEx
