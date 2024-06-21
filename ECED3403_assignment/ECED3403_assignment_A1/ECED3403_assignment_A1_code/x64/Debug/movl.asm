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
		movl	#90,R2
		movl	#90,R4
		add.b	R4,R2
		movl 	#0,R3
		addc.b	#0,R3
		movl	#90,R2
		add.b	R4,R2
		cmp	R2,R3
		movl 	#0,R3
		addc.b	#0,R3		
		movh	#1100,R2
		movh	#4400,R1
		movlz	#66,R2
		movls	#88,R1
DONE
		bra		DONE			; loop
end MAIN