; 
;lab 1, question 1: sum an array of 16-bit numbers, stopping when a program-specified liit is reached
;name: Grace Yu
;B00: 902046
;class: ECED3403
;instructor: Dr. Larry Hughes
;
DATA
		org #40
ARRAY	word $5					; create array
		word $1
		word $1
		word $1
		word $1
		word $1
CODE
		org #1000
MAIN
		movl	#9,R0
		movl	#1,R1
		sub.b	R1,R0
DONE
		bra		DONE			; loop
end MAIN