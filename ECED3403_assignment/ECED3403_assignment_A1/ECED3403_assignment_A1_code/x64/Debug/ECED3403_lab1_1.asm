; 
;lab 1, question 1: sum an array of 16-bit numbers, stopping when a program-specified liit is reached
;name: Grace Yu
;B00: 902046
;class: ECED3403
;instructor: Dr. Larry Hughes
;
		org #1000
MAIN
		MOVL #1234,R1
		MOVLZ #1234,R1
		MOVLS #1234,R1
		MOVH #1234,R1
DONE
		bra		DONE			; loop
end MAIN