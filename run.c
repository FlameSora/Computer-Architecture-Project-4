/***************************************************************/
/*                                                             */
/*   MIPS-32 Instruction Level Simulator                       */
/*                                                             */
/*   CS311 KAIST                                               */
/*   run.c                                                     */
/*                                                             */
/***************************************************************/

#include <stdio.h>

#include "util.h"
#include "run.h"
#include "cache.h"
/***************************************************************/
/*                                                             */
/* Procedure: get_inst_info                                    */
/*                                                             */
/* Purpose: Read insturction information                       */
/*                                                             */
/***************************************************************/
instruction* get_inst_info(uint32_t pc) { 
    return &INST_INFO[(pc - MEM_TEXT_START) >> 2];
}


/***************************************************************/
/*                                                             */
/* Procedure: process_instruction                              */
/*                                                             */
/* Purpose: Process one instrction                             */
/*                                                             */
/***************************************************************/
void process_instruction(){
	/** Your implementation here */
	printf("%x cache 0 0: \n",Cache[0][0]);
	WB_Stage();
	MEM_Stage();
	EX_Stage();
	ID_Stage();
	IF_Stage();
	if (miss_penalty != 0){
		miss_penalty -= 1;
	}
}

void IF_Stage(){
//	printf("current pc is: %x\n",CURRENT_STATE.PC);
	if(miss_penalty>0&&miss_penalty!=31){
		CURRENT_STATE.IF_ID_NPC = CURRENT_STATE.IF_ID_NPC;
	}
	else if (CURRENT_STATE.PC - MEM_TEXT_START >= 4*NUM_INST) {
//		printf("1rrrr");
		CURRENT_STATE.IF_ID_NPC = CURRENT_STATE.PC;
		CURRENT_STATE.PIPE[0] = 0;
	}
	else if(CURRENT_STATE.MEM_WB_FORWARD_REG==1){
//		printf("2ssss");
		CURRENT_STATE.PIPE[0] = CURRENT_STATE.PIPE[0];
		CURRENT_STATE.PC = CURRENT_STATE.IF_PC+4;
		CURRENT_STATE.IF_PC = 0;
	}
	else {
//		printf("3tttt");	
		if (CURRENT_STATE.IF_ID_INST ==1){
			CURRENT_STATE.PIPE[0] = 0;
			CURRENT_STATE.IF_ID_INST =0;
			CURRENT_STATE.IF_ID_NPC = 0;
		}
		else{
			if(CURRENT_STATE.JUMP_PC !=0){
				CURRENT_STATE.IF_ID_NPC = 0;
				CURRENT_STATE.PIPE[0] = CURRENT_STATE.PC;
				CURRENT_STATE.PC = CURRENT_STATE.JUMP_PC;
				CURRENT_STATE.JUMP_PC = 0;
			}else {
				CURRENT_STATE.PIPE[0] = CURRENT_STATE.PC;
				CURRENT_STATE.IF_ID_NPC = CURRENT_STATE.PC;
				if(CURRENT_STATE.IF_PC==0){
					CURRENT_STATE.PC = CURRENT_STATE.PC +4;
				}
			}
		}
	}
//	printf("current pc is: %x\n",CURRENT_STATE.PC);
}
void ID_Stage(){
	
//	printf("current pc is: %x\n",CURRENT_STATE.PC);
	if(miss_penalty>0&&miss_penalty!=31){
		CURRENT_STATE.ID_EX_NPC= CURRENT_STATE.ID_EX_NPC;
	}
	else if (CURRENT_STATE.IF_ID_NPC - MEM_TEXT_START >= 4*NUM_INST) {
		CURRENT_STATE.ID_EX_NPC = CURRENT_STATE.IF_ID_NPC;
		CURRENT_STATE.PIPE[1] = 0;
	}
	else if (CURRENT_STATE.MEM_WB_FORWARD_REG==1){
		CURRENT_STATE.PIPE[1] = CURRENT_STATE.PIPE[1];
	}	

	else if (CURRENT_STATE.IF_ID_NPC != 0) {

		instruction* instrp; 
		instrp = get_inst_info(CURRENT_STATE.IF_ID_NPC);
		CURRENT_STATE.PIPE[1] = CURRENT_STATE.IF_ID_NPC;
		CURRENT_STATE.ID_EX_NPC = CURRENT_STATE.IF_ID_NPC;
		CURRENT_STATE.ID_EX_REG1 = RS(instrp);
		CURRENT_STATE.ID_EX_REG2 = RT(instrp);
		CURRENT_STATE.ID_EX_IMM = IMM(instrp);
		if (OPCODE(instrp)  == 2){
			CURRENT_STATE.JUMP_PC = TARGET(instrp)*4;
			CURRENT_STATE.ID_EX_DEST = 1;
		}
		else if(OPCODE(instrp) == 3){
			CURRENT_STATE.REGS[31] = CURRENT_STATE.PC+4;
			CURRENT_STATE.JUMP_PC = TARGET(instrp)*4;
			CURRENT_STATE.ID_EX_DEST = 1;
		}
		else if(OPCODE(instrp) == 0 &&FUNC(instrp) == 8){
			CURRENT_STATE.JUMP_PC = CURRENT_STATE.REGS[RS(instrp)];
			CURRENT_STATE.ID_EX_DEST = 1;
		}
		else{
			CURRENT_STATE.ID_EX_DEST = 0;
		}
	} else {
		CURRENT_STATE.PIPE[1] = 0;
		CURRENT_STATE.ID_EX_NPC = 0;
	}
}
void EX_Stage(){
//	CURRENT_STATE.IF_PC = 0;
//	printf("current pc is: %x\n",CURRENT_STATE.PC);
	if(miss_penalty>0&&miss_penalty!=31){
		CURRENT_STATE.EX_MEM_NPC = CURRENT_STATE.EX_MEM_NPC;
	}
	else if (CURRENT_STATE.ID_EX_NPC - MEM_TEXT_START >= 4*NUM_INST) {
		CURRENT_STATE.EX_MEM_NPC = CURRENT_STATE.ID_EX_NPC;
		CURRENT_STATE.PIPE[2] = 0;
	}
	
	else if (CURRENT_STATE.ID_EX_NPC != 0) {

		instruction* inst;
		inst = get_inst_info(CURRENT_STATE.ID_EX_NPC);
		CURRENT_STATE.PIPE[2] = CURRENT_STATE.ID_EX_NPC;
		CURRENT_STATE.EX_MEM_NPC = CURRENT_STATE.ID_EX_NPC;
			
		short op = OPCODE(inst);
		short func = FUNC(inst);
		uint32_t reg1 = CURRENT_STATE.REGS[CURRENT_STATE.ID_EX_REG1]; // rs
		uint32_t reg2 = CURRENT_STATE.REGS[CURRENT_STATE.ID_EX_REG2]; // rt
		short imm = CURRENT_STATE.ID_EX_IMM;
		unsigned char sh = SHAMT(inst);
		if (CURRENT_STATE.MEM_WB_FORWARD_REG !=1){
			  if(CURRENT_STATE.MEM_WB_FORW==1){
				if(CURRENT_STATE.EX_MEM_FORW ==1){
					if(CURRENT_STATE.ID_EX_REG1 == RD(inst)){
						reg1 = CURRENT_STATE.EX_MEM_ALU_OUT;
					}
					else if (CURRENT_STATE.ID_EX_REG2 == RD(inst)){
						reg2 = CURRENT_STATE.EX_MEM_ALU_OUT;
					}
					else {
						if(CURRENT_STATE.ID_EX_REG1 == CURRENT_STATE.MEM_WB_DEST){
							reg1 = CURRENT_STATE.MEM_WB_MEM_OUT;
							CURRENT_STATE.MEM_WB_FORWARD_REG = 1;
						}
						if (CURRENT_STATE.ID_EX_REG2 == CURRENT_STATE.MEM_WB_DEST){
							reg2 = CURRENT_STATE.MEM_WB_MEM_OUT;
							CURRENT_STATE.MEM_WB_FORWARD_REG = 1;
						}
					}
				}
				else{ 
					if(CURRENT_STATE.ID_EX_REG1 == CURRENT_STATE.MEM_WB_DEST){
						reg1 = CURRENT_STATE.MEM_WB_MEM_OUT;
						CURRENT_STATE.MEM_WB_FORWARD_REG = 1;
					}	
					if (CURRENT_STATE.ID_EX_REG2 == CURRENT_STATE.MEM_WB_DEST){
						reg2 = CURRENT_STATE.MEM_WB_MEM_OUT;
						CURRENT_STATE.MEM_WB_FORWARD_REG = 1;
					}
				}
				
			}
			else if(CURRENT_STATE.EX_MEM_FORW ==1){
				if(CURRENT_STATE.ID_EX_REG1 == CURRENT_STATE.EX_MEM_DEST){
					reg1 = CURRENT_STATE.EX_MEM_ALU_OUT;
				}
				if (CURRENT_STATE.ID_EX_REG2 == CURRENT_STATE.EX_MEM_DEST){
					reg2 = CURRENT_STATE.EX_MEM_ALU_OUT;
				}
			}
		}
		else{
			CURRENT_STATE.MEM_WB_FORWARD_REG = 0;
		}
		if (CURRENT_STATE.MEM_WB_FORWARD_REG != 1){
			if(OPCODE(inst) != 4 && OPCODE(inst) != 5&& OPCODE(inst)!=43&&OPCODE(inst)!= 2&&OPCODE(inst) != 3 &&~(OPCODE(inst)==0 && FUNC(inst) ==8)){
				CURRENT_STATE.EX_MEM_FORW = 1;
			}
			else{
				CURRENT_STATE.EX_MEM_FORW = 0;
			}
			// Addiu
			if (op == 9) {
				CURRENT_STATE.EX_MEM_ALU_OUT = reg1 + imm;
				CURRENT_STATE.EX_MEM_DEST = RT(inst);
			}
			// Andi
			if (op == 12) {
				CURRENT_STATE.EX_MEM_ALU_OUT = reg1 & imm;
				CURRENT_STATE.EX_MEM_DEST = RT(inst);
			}
			// Beq
			if (op == 4) {
				if (reg1 == reg2) {
					CURRENT_STATE.EX_MEM_BR_TARGET = CURRENT_STATE.ID_EX_NPC + 4 + imm*4;
					CURRENT_STATE.EX_MEM_BR_TAKE = TRUE;
				}
			}
			// Bne
			if (op == 5) {
				if (reg1 != reg2) {
					CURRENT_STATE.EX_MEM_BR_TARGET = CURRENT_STATE.ID_EX_NPC + 4 + imm*4;
					CURRENT_STATE.EX_MEM_BR_TAKE = TRUE;
				}
			}
			// Lui
			if (op == 15) {
				uint32_t data = (uint32_t) imm;
				CURRENT_STATE.EX_MEM_ALU_OUT = data << 16;
				CURRENT_STATE.EX_MEM_DEST = RT(inst);
			}
			// Lw
			if (op == 35) {
				CURRENT_STATE.EX_MEM_ALU_OUT = reg1 + imm;
				CURRENT_STATE.EX_MEM_DEST = RT(inst);
			}
			// Ori
			if (op == 13) {
				CURRENT_STATE.EX_MEM_ALU_OUT = reg1 | imm;
				CURRENT_STATE.EX_MEM_DEST = RT(inst);
			}
			// Sltiu
			if (op == 11) {
				uint32_t data = (uint32_t) imm;
				if (reg1 < data) {
					CURRENT_STATE.EX_MEM_ALU_OUT = 1;
					CURRENT_STATE.EX_MEM_DEST = RT(inst);
				} else {
					CURRENT_STATE.EX_MEM_ALU_OUT = 0;
					CURRENT_STATE.EX_MEM_DEST = RT(inst);
				}
			}
			// Sw
			if (op == 43) {
				CURRENT_STATE.EX_MEM_ALU_OUT = reg1 + imm;
				CURRENT_STATE.EX_MEM_W_VALUE = reg2;
			}
			// R types
			if (op == 0) {
				// Addu
				if (func == 33) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg1 + reg2;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// And
				if (func == 36) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg1 & reg2;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// Nor
				if (func == 39) {
					CURRENT_STATE.EX_MEM_ALU_OUT = ~(reg1 | reg2);
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// Or
				if (func == 37) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg1 | reg2;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// Sltu
				if (func == 43) {
					if (reg1 < reg2) {
						CURRENT_STATE.EX_MEM_ALU_OUT = 1;
						CURRENT_STATE.EX_MEM_DEST = RD(inst);
					} else {
						CURRENT_STATE.EX_MEM_ALU_OUT = 0;
						CURRENT_STATE.EX_MEM_DEST = RD(inst);
					}
				}
				// Sll
				if (func == 0) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg2 << sh;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// Srl
				if (func == 2) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg2 >> sh;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}
				// Subu
				if (func == 35) {
					CURRENT_STATE.EX_MEM_ALU_OUT = reg1 - reg2;
					CURRENT_STATE.EX_MEM_DEST = RD(inst);
				}

			}
			if (op == 35) {
				instruction* instrpT; 
				instrpT = get_inst_info(CURRENT_STATE.IF_ID_NPC);
		//		if(OPCODE(instrpT) != 4 && OPCODE(instrpT) != 5&& OPCODE(instrpT)!=43&&OPCODE(instrpT)!= 2&&OPCODE(instrpT) != 3 &&~(OPCODE(instrpT)==0 && FUNC(instrpT) ==8)){
		//			printf("hahah");
		//		}
				if(RS(instrpT) == CURRENT_STATE.EX_MEM_DEST || RT(instrpT) == CURRENT_STATE.EX_MEM_DEST){
					CURRENT_STATE.IF_PC = CURRENT_STATE.PC;
				}
			}
		}else{
			CURRENT_STATE.PIPE[2] = 0;
			CURRENT_STATE.EX_MEM_NPC = 0;
		}
	} else {
		CURRENT_STATE.PIPE[2] = 0;
		CURRENT_STATE.EX_MEM_NPC = 0;
	}
}

void MEM_Stage(){

//	printf("current pc is: %x\n",CURRENT_STATE.PC);
	if(miss_penalty>1){
		CURRENT_STATE.MEM_WB_NPC = 0;
	}
	else if (miss_penalty ==1){
		CURRENT_STATE.MEM_WB_NPC = CURRENT_STATE.MEM_STALL_NPC;
		Cache[CURRENT_STATE.MEM_INDEX_BIT][CURRENT_STATE.MEM_J][CURRENT_STATE.MEM_BLOCK] = mem_read_32(CURRENT_STATE.MEM_STALL_ALU_OUT);
		if(CURRENT_STATE.MEM_BLOCK ==1){
			Cache[CURRENT_STATE.MEM_INDEX_BIT][CURRENT_STATE.MEM_J][0] = mem_read_32(CURRENT_STATE.MEM_STALL_ALU_OUT-4);
		}
		else{
			Cache[CURRENT_STATE.MEM_INDEX_BIT][CURRENT_STATE.MEM_J][1] = mem_read_32(CURRENT_STATE.MEM_STALL_ALU_OUT+4);
		}
	}
	else if (CURRENT_STATE.EX_MEM_NPC - MEM_TEXT_START >= 4*NUM_INST) {
		CURRENT_STATE.MEM_WB_NPC = CURRENT_STATE.EX_MEM_NPC;
		CURRENT_STATE.PIPE[3] = 0;
	}	
	else if(CURRENT_STATE.EX_MEM_NPC != 0){
		instruction* instrp; 
		instrp = get_inst_info(CURRENT_STATE.EX_MEM_NPC);
		CURRENT_STATE.PIPE[3] = CURRENT_STATE.EX_MEM_NPC;
		CURRENT_STATE.MEM_WB_NPC = CURRENT_STATE.EX_MEM_NPC;
		if(miss_penalty == 1){
			CURRENT_STATE.MEM_WB_NPC = CURRENT_STATE.MEM_STALL_NPC;
		}
		CURRENT_STATE.MEM_WB_ALU_OUT = CURRENT_STATE.EX_MEM_ALU_OUT;
		if(OPCODE(instrp) == 43){
		//sw 
			mem_write_32(CURRENT_STATE.EX_MEM_ALU_OUT, CURRENT_STATE.EX_MEM_W_VALUE);
		}
		else if(OPCODE(instrp) == 35){
		//lw
			CURRENT_STATE.MEM_WB_MEM_OUT = mem_read_32(CURRENT_STATE.EX_MEM_ALU_OUT);
		       	uint32_t Index_Bit = (CURRENT_STATE.EX_MEM_ALU_OUT<<28)>>31;
			uint32_t Block_Offset = (CURRENT_STATE.EX_MEM_ALU_OUT<<29)>>31;	
			uint32_t TAG= CURRENT_STATE.EX_MEM_ALU_OUT>>4;
			int tempV = 0;
			printf("ADD in LW: %x\n", CURRENT_STATE.EX_MEM_ALU_OUT);
			printf("index bit is: %x\n",Index_Bit);	
			printf("block offset is: %x\n", Block_Offset);
			printf("mem read is : %x\n",CURRENT_STATE.MEM_WB_MEM_OUT);
			printf("TAG is: %x\n",TAG);
			int i,j;
			//checking cache hit
			for (j = 0; j<4 ; j++){
				if(Cache_info[Index_Bit][j][1] ==  TAG && Cache_info[Index_Bit][j][0] ==1){
					CURRENT_STATE.MEM_WB_MEM_OUT = Cache[Index_Bit][j][Block_Offset];
					for(i = 0;i<4;i++){
						if(Cache_info[Index_Bit][i][2]< Cache_info[Index_Bit][j][2]&& Cache_info[Index_Bit][i][0] ==1){
							Cache_info[Index_Bit][i][2] +=1;
						} 
					}
					Cache_info[Index_Bit][j][2] = 0;
					tempV = 1;
				}	
			}
			if(tempV==0){
				//when cannot find the data from the cache:
				//first find whether is a empty space:
				miss_penalty = 31;
				CURRENT_STATE.MEM_STALL_NPC = CURRENT_STATE.MEM_WB_NPC;
				CURRENT_STATE.MEM_STALL_ALU_OUT = CURRENT_STATE.EX_MEM_ALU_OUT;
				int tempV2 = 0;
				for(j = 0; j<4; j++){
					if(Cache_info[Index_Bit][j][0] ==0&&tempV2==0){
						Cache_info[Index_Bit][j][0] =1;
						Cache_info[Index_Bit][j][1] = TAG;
						//Cache[Index_Bit][j][Block_Offset] = CURRENT_STATE.MEM_WB_MEM_OUT;
						CURRENT_STATE.MEM_INDEX_BIT = Index_Bit;
						CURRENT_STATE.MEM_J = j;
						CURRENT_STATE.MEM_BLOCK = Block_Offset;
					//	if(Block_Offset ==1){
					//		Cache[Index_Bit][j][0] = mem_read_32(CURRENT_STATE.EX_MEM_ALU_OUT-4);
					//	}
					//	else{
					//		Cache[Index_Bit][j][1] = mem_read_32(CURRENT_STATE.EX_MEM_ALU_OUT+4);
					//	}
						tempV2 =1;
					}
				}
				//LRU 
				if(tempV2 ==0){
					
					int tempJ =0;
					int tempOrder = Cache_info[Index_Bit][0][2];
					for(j =0;j<4;j++){
						if(Cache_info[Index_Bit][j][0] == 1 && Cache_info[Index_Bit][j][2] >tempOrder){
							tempJ = j;
							tempOrder = Cache_info[Index_Bit][j][2];
						} 
					}
					CURRENT_STATE.MEM_INDEX_BIT = Index_Bit;
					CURRENT_STATE.MEM_J = tempJ;
					CURRENT_STATE.MEM_BLOCK = Block_Offset;
					//Cache[Index_Bit][tempJ][Block_Offset] =CURRENT_STATE.MEM_WB_MEM_OUT;
					Cache_info[Index_Bit][tempJ][1] = TAG;
					Cache_info[Index_Bit][tempJ][2] = 0;
				//	if(Block_Offset ==1){
				//		Cache[Index_Bit][tempJ][0] = mem_read_32(CURRENT_STATE.EX_MEM_ALU_OUT-4);
				//	}
				//	else{
				//		Cache[Index_Bit][tempJ][1] = mem_read_32(CURRENT_STATE.EX_MEM_ALU_OUT+4);
				//	}

				}
			}
		}
		if(CURRENT_STATE.EX_MEM_BR_TAKE ==1){
			//flush
			CURRENT_STATE.IF_ID_INST =1;
			CURRENT_STATE.IF_ID_NPC = 0;
			CURRENT_STATE.PIPE[0] = 0;
			CURRENT_STATE.ID_EX_NPC = 0;
			CURRENT_STATE.PIPE[1]=0;
			CURRENT_STATE.EX_MEM_NPC=0;
			CURRENT_STATE.PIPE[2] =0; 
			CURRENT_STATE.PC = CURRENT_STATE.EX_MEM_BR_TARGET;
			CURRENT_STATE.PIPE[0] = 0;
			CURRENT_STATE.PIPE[1] = 0;
			CURRENT_STATE.PIPE[2] = 0;
		}
		
		if(OPCODE(instrp) ==35){
			CURRENT_STATE.MEM_WB_FORW = 1;	
		}
		else{
			CURRENT_STATE.MEM_WB_FORW = 0;
		}
		CURRENT_STATE.MEM_WB_BR_TAKE = CURRENT_STATE.EX_MEM_BR_TAKE;
		CURRENT_STATE.MEM_WB_DEST  = CURRENT_STATE.EX_MEM_DEST; 
		CURRENT_STATE.EX_MEM_BR_TAKE =0;
	} else {
		CURRENT_STATE.PIPE[3] = 0;
		CURRENT_STATE.MEM_WB_NPC = 0;
	}	
}


void WB_Stage(){
	
//	printf("current pc is: %x\n",CURRENT_STATE.PC);
	if(miss_penalty>0){
		CURRENT_STATE.PIPE[4] = 0;
	}
	else if(CURRENT_STATE.MEM_WB_NPC!=0){
		instruction* instrp; 
		instrp = get_inst_info(CURRENT_STATE.MEM_WB_NPC);
		CURRENT_STATE.PIPE[4] = CURRENT_STATE.MEM_WB_NPC;
		if(OPCODE(instrp) == 35){
			CURRENT_STATE.REGS[CURRENT_STATE.MEM_WB_DEST] = CURRENT_STATE.MEM_WB_MEM_OUT;
		}
		else if(OPCODE(instrp) != 4 && OPCODE(instrp) != 5&& OPCODE(instrp)!=43&&OPCODE(instrp)!= 2&&OPCODE(instrp) != 3 &&~(OPCODE(instrp)==0 && FUNC(instrp) ==8)){
			CURRENT_STATE.REGS[CURRENT_STATE.MEM_WB_DEST] = CURRENT_STATE.MEM_WB_ALU_OUT;
		}
		if ((CURRENT_STATE.EX_MEM_NPC - MEM_TEXT_START >= 4*NUM_INST) && (CURRENT_STATE.EX_MEM_NPC != 0)) {
			RUN_BIT = FALSE;
		}
		INSTRUCTION_COUNT++;
	} else {
		CURRENT_STATE.PIPE[4] = 0;
	}	
}   

