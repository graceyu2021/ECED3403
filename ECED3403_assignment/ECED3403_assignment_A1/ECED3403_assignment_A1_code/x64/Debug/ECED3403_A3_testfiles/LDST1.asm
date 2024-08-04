;
; Example of direct addressing
; Load and store
;
; V1 <- V1 + 2
;
	data
	org	#100
V1	word	#FFFD
;
	code
	org	#200
LDST1
;
; Get address of V1 into R0
;
	movl	V1,R0		; R0 = #??00
	movh	V1,R0		; R0 = #0100
;
; R1 <- mem[R0]
;
; R0 contains the effective address
; mem[R0] is rvalue (a value or expression)
; R1 is lvalue (where the result is to be stored)
;
; LD from-memory (rvalue),to-register (lvalue)
;
	LD	R0,R1		; R1 <- #FFFD
;
	ADD	#2,R1		; R1 <- R1 + 2
;
; mem[R0] <- R1
;
; R1 is rvalue
; R0 is the effective address
; mem[R0] is lvalue
; 
	ST	R1,R0
;
; Put breakpoint on next address
;
BrkPtHere
;
	end LDST1
