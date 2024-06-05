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
INITIALIZE
		; initialize r0 to be the base memory location of array
		movl	ARRAY,R0		; R0 is now base value of array
		movh	ARRAY,R0
		;
		; initialize r1, max count, to be the value at base memory location of array
		ld		R0,R1			; max count <- mem(r1)
		;
		; increment array to next value
		add		$2,R0			
		;
		; initialize r2 and r3 to be 0
		movlz	$0,R2			; current value R2 <- 0
		movlz	$0,R3			; sum			R3 <- 0
LOOP
		;compare R1 to 0
		cmp		$1,R1
		blt		DONE			; jump to done if count is equal to zero

		; put current value into sum
		ld		R0,R2			; load array value into register
		add		R2,R3			; add array value into sum
		;
		; increment array to next value
		add		$2,R0
		;
		;decrement the current counter r1
		sub		$1,R1
		;
		;jump to loop
		bra		LOOP
DONE
		bra		DONE			; loop
end MAIN