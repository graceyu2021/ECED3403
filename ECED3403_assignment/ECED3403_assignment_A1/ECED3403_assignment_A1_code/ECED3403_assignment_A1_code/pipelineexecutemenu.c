/*
file: pipelineexecutemenu.c
programmer: Grace Yu
b00: B00902046
class: ECED3403
professor: Dr. Larry Hughes

This is the pipeline execute menu function file of my program.
*/

#include "MAINHEADER.H"

// execute function
void execute0() {
	// temporary return just until full instruciton set is implemented
	if (((opcode >= ADD && opcode <= SXT) || (opcode >= SETCC && opcode <= STR)) != TRUE) // not an instruction to implement in a2
		return;

	unsigned short dest_value, srccon_value, bytevalue;
	int wordbyte;

	// initilalize variables to struct contents for readability
	if (opcode >= ADD && opcode <= BIS) // if opcode is from add to bis
		add_to_bis_ops(operand.dst, operand.srccon, operand.srcconcheck, &dest_value, &srccon_value, &wordbyte);

	else if (opcode >= MOV && opcode <= SXT) // if opcode is from mov to sxt
		mov_to_sxt_ops(operand.dst, operand.srccon, &dest_value, &srccon_value, &wordbyte);

	else if (opcode >= MOVL && opcode <= MOVH) // if opcode is from movl to movh
		movl_to_movh_ops(&bytevalue);

	// switch case to identify what execute process to go through
	switch (opcode) {
	case(BL):
		bl_execute();
		break;
	case(BEQBZ):
		beq_to_bra_execute(psw.z == SET);
		break;
	case(BNEBNZ):
		beq_to_bra_execute(psw.z == CLEAR);
		break;
	case(BCBHS):
		beq_to_bra_execute(psw.c == SET);
		break;
	case(BNCBLO):
		beq_to_bra_execute(psw.c == CLEAR);
		break;
	case(BN):
		beq_to_bra_execute(psw.n == SET);
		break;
	case(BGE):
		beq_to_bra_execute(psw.n ^ psw.v == CLEAR);
		break;
	case(BLT):
		beq_to_bra_execute(psw.n ^ psw.v == SET);
		break;
	case(BRA):
		beq_to_bra_execute(TRUE);
		break;
	case(ADD):
	case(SUB):
	case(ADDC):
	case(SUBC): // ADD to SUBC are all performed in one function
		add_to_subc_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(DADD):
		dadd_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(CMP):
		cmp_execute(dest_value, srccon_value);
		break;
	case(XOR):
		xor_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(AND):
		and_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(OR):
		or_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(BIT):
		bit_execute(dest_value, srccon_value, operand.dst);
		break;
	case(BIC):
		bic_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(BIS):
		bis_execute(dest_value, srccon_value, operand.dst, wordbyte);
		break;
	case(MOV):
		mov_execute(srccon_value, operand.dst, wordbyte);
		break;
	case(SWAP):
		swap_execute(dest_value, srccon_value, operand.dst, operand.srccon);
		break;
	case(SRA):
		sra_execute(dest_value, operand.dst, wordbyte);
		break;
	case(RRC):
		rrc_execute(dest_value, operand.dst, wordbyte);
		break;
	case(SWPB):
		swpb_execute(operand.dst);
		break;
	case(SXT):
		sxt_execute(dest_value, operand.dst);
		break;
	case(SETCC):
		setcc_execute();
		break;
	case(CLRCC):
		clrcc_execute();
		break;
	case (CEX):
		break;
	case(LD):
		ldst_execute(operand.srccon, srcconarray.word[REGISTER][operand.srccon], READ);
		break;
	case(ST):
		ldst_execute(operand.dst, srcconarray.word[REGISTER][operand.dst], WRITE);
		break;
	case(MOVL):
		movl_execute(bytevalue);
		break;
	case(MOVLZ):
		movlz_execute(bytevalue);
		break;
	case(MOVLS):
		movls_execute(bytevalue);
		break;
	case(MOVH):
		movh_execute(bytevalue);
		break;
	case(LDR):
		ldrstr_execute(operand.srccon, srcconarray.word[REGISTER][operand.srccon], READ);
		break;
	case(STR):
		ldrstr_execute(operand.dst, srcconarray.word[REGISTER][operand.dst], WRITE);
		break;
	default:
		break;
	}

	// called debugging functions for ease of marker
#ifdef DEBUG
	reg_display();
	//psw_display();
	printf("\n");
#endif
}