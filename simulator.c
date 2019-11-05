#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include <math.h>
#include "string.h"

#pragma warning(disable:4996)
#define NUM_OF_REGISTERS 16
#define SIZE_OF_MEMORY 65536

// defined an enum type for operations, for improved readability
typedef enum operation {
	add, addi, sub, subi, and, or , sll, sra, beq, bgt,
	ble, bne, jal, lw, sw, lhi, halt
} operation;

// this array will be used as the "memory" during run-time, as it is easier
// to manipulate than text files
typedef int registers[NUM_OF_REGISTERS];
int memory[SIZE_OF_MEMORY] = { 0x0000 };

// generic error-handler
int handle_error() {
	perror("ERROR");
	exit(errno);
}

// evaluates a hex string and returns an integer that represents te same value
int evaluate_hex_string(char str[5]) {
	str[4] = '\0';
	return strtol(str, NULL, 16);
}

// initialize memory array to contents of memin
int memory_init(int *memory, FILE *memin) {
	char buff[6];
	int ind = 0, val = 0;

	// iterate over memin file, reading a single row at a time and assigning
	// its content as an int into the memory array in the appropriate index
	while (fgets(buff, 6, memin)) {
		val = evaluate_hex_string(buff);
		memory[ind] = val;
		ind++;
	}

	return 0;
}

// assign the proper op-code given the op number
operation assign_op(int code, bool is_imm) {
	switch (code) {
	case 0:
		if (is_imm)
			return addi;
		return add;
	case 1:
		if (is_imm)
			return subi;
		return sub;
	case 2:
		return and;
	case 3:
		return or ;
	case 4:
		return sll;
	case 5:
		return sra;
	case 6:
		printf("invalid opcode!");
		handle_error();
	case 7:
		return beq;
	case 8:
		return bgt;
	case 9:
		return ble;
	case 10:
		return bne;
	case 11:
		return jal;
	case 12:
		return lw;
	case 13:
		return sw;
	case 14:
		return lhi;
	case 15:
		return halt;
	default:
		errno = EINVAL;
		printf("Invalid opcode!\n");
		handle_error();
		return add;
		break;
	}
}

// validate that the program was used with the proper amount of inputs
void validate_inputs(int argc) {
	if (argc != 6) {
		errno = EINVAL;
		printf("Please input 5 text files in the following order:\n"
			"memin memout regout trace count");
		handle_error();
	}
}

// evaluate str[num] as a hex char and return an integer of the same value
int get_hex_int(const char* str, int num) {
	char s[2];
	s[1] = '\0';
	s[0] = str[num];
	return strtol(s, NULL, 16);
}

// writes the memout file from the state of the memory at the end of the program
// returns the number of rows written
int memout_write(FILE *memout, int memory[]) {
	// iterate over memory and write a memory line into memout for every memory address
	for (int row = 0; row<SIZE_OF_MEMORY; row++) {
		char buff[5];
		sprintf(buff, "%04X", memory[row]);
		if (fputs(buff, memout) < 0)
			handle_error();
		fputs("\n", memout);
	}
	return 0;
}

// execute the current instruction
int execute_inst(operation op, registers* reg_arr, int rd, int rs,
	int rt, int imm, int* pc, bool* is_jump, bool* is_imm, FILE* memout) {
	// setting the correct address for word instructions
	int address = (rs == 1 ? imm : (*reg_arr)[rs]) + (rt == 1 ? imm : (*reg_arr)[rt]);

	switch (op) {
	case addi:
		(*reg_arr)[rd] = ((rt == 1) ? imm : (*reg_arr)[rt]) + ((rs == 1) ? imm : (*reg_arr)[rs]);
		break;
	case add:
		(*reg_arr)[rd] = (*reg_arr)[rs] + (*reg_arr)[rt];
		break;
	case subi:
		(*reg_arr)[rd] = ((rs == 1) ? imm : (*reg_arr)[rs]) - ((rt == 1) ? imm : (*reg_arr)[rt]);
		break;
	case sub:
		(*reg_arr)[rd] = (*reg_arr)[rs] - (*reg_arr)[rt];
		break;
	case and:
		(*reg_arr)[rd] = (*reg_arr)[rs] & (*reg_arr)[rt];
		break;
	case or :
		(*reg_arr)[rd] = (*reg_arr)[rs] | (*reg_arr)[rt];
		break;
	case sll:
		(*reg_arr)[rd] = (*reg_arr)[rs] << (*reg_arr)[rt];
		break;
	case sra:
		(*reg_arr)[rd] = (*reg_arr)[rs] >> (*reg_arr)[rt];
		break;
	case beq:
		if ((*reg_arr)[rs] == (*reg_arr)[rt]) {
			*pc = (*reg_arr)[rd] & 0xFFFF;
			*is_jump = true;
		}
		break;
	case bgt:
		if ((*reg_arr)[rs] > (*reg_arr)[rt]) {
			*pc = (*reg_arr)[rd] & 0xFFFF;
			*is_jump = true;
		}
		break;
	case ble:
		if ((*reg_arr)[rs] <= (*reg_arr)[rt]) {
			*pc = (*reg_arr)[rd] & 0xFFFF;
			*is_jump = true;
		}
		break;
	case bne:
		if ((*reg_arr)[rs] != (*reg_arr)[rt]) {
			*pc = (*reg_arr)[rd] & 0xFFFF;
			*is_jump = true;
		}
		break;
	case jal:
		(*reg_arr)[15] = is_imm ? *pc + 2 : *pc + 1;
		// if instruction is i-type, set the pc to the immediate value
		*pc = ((rd == 1) ? imm : ((*reg_arr)[rd] & 0xFFFF));
		*is_jump = true;
		break;
	case lw:
		(*reg_arr)[rd] = memory[address];
		break;
	case sw:
		memory[address] = (*reg_arr)[rd] & 0xFFFF;
		break;
	case lhi:
		(*reg_arr)[rd] &= 0xFFFF;
		(*reg_arr)[rd] += ((*reg_arr)[rs] << 16);
		break;
	case halt:
		printf("halting!\n");
		break;
	}
	return 0;
}

// write the number of executed instructions into count as required
void write_opcount(int opcount, FILE* count) {
	//char cnt[(opcount % 10) + 2];
	int bytes_needed = (opcount % 10) + 2;
	char* cnt= (char*) malloc(bytes_needed * sizeof(char));
	sprintf(cnt, "%d", opcount);
	fputs(cnt, count);

	free(cnt);
	cnt = NULL;
}

// write the contents of the registers as required
void write_regout(int *reg_arr, FILE* regout) {
	char buff[9];
	for (int i = 2; i<16; i++) {
		sprintf(buff, "%08X", reg_arr[i]);
		fputs(buff, regout);
		fputs("\n", regout);
	}
}

// write a line into the trace file
void write_trace_line(FILE* trace, int pc, int* reg_arr, bool is_imm, int imm) {
	char buff[9];

	// write pc
	sprintf(buff, "%08X", pc);
	fputs(buff, trace);
	fputs(" ", trace);
	// write inst, which is memory[pc] - if its an i-type, write imm in first 4 hex digits
	sprintf(buff, "%04X%04X", is_imm ? memory[pc + 1] : 0, memory[pc]);
	fputs(buff, trace);
	fputs(" ", trace);
	// write the zero register
	sprintf(buff, "%08X", 0);
	fputs(buff, trace);
	fputs(" ", trace);
	// write the imm register
	sprintf(buff, "%08X", imm);
	fputs(buff, trace);
	fputs(" ", trace);

	// write registers 2-14
	for (int i = 2; i<15; i++) {
		sprintf(buff, "%08X", reg_arr[i]);
		fputs(buff, trace);
		fputs(" ", trace);
	}

	// write last register and newline
	sprintf(buff, "%08X", reg_arr[15]);
	fputs(buff, trace);
	fputs("\n", trace);
}

int main(int argc, char* argv[]) {
	setvbuf(stdout, NULL, _IONBF, 0);
	printf("started\n");
	validate_inputs(argc);

	////////////////////////////////////////////////////////////////
	////////////////           Set-up Stage         ////////////////
	////////////////////////////////////////////////////////////////

	// Declare variables
	int pc = 0, imm = 0, opcode = 0, rd = 0, rs = 0, rt = 0, opcount = 0;
	registers register_arr = { 0 };
	FILE *memin, *memout, *regout, *trace, *count;
	char inst[5], trace_path[257];
	operation op;
	bool is_imm, is_jump;

	// Initialize variables
	inst[4] = '\0';
	for (int i = 0; i<NUM_OF_REGISTERS; i++)
		register_arr[i] = 0;
	if (!(strcpy(trace_path, argv[4])))
		handle_error();

	//  OPEN ALL THE FILES!!!
	if (!(memin = fopen(argv[1], "r")))
		handle_error();
	if (!(memout = fopen(argv[2], "w+")))
		handle_error();
	if (!(regout = fopen(argv[3], "w+")))
		handle_error();
	if (!(trace = fopen(argv[4], "w+")))
		handle_error();
	if (!(count = fopen(argv[5], "w+")))
		handle_error();

	if (memory_init(memory, memin) < 0)
		handle_error();

	// Let the Fetch-Decode-Execute loop begin!
	while (opcode != 15) {

		// Getting ready for new iteration
		is_imm = false;
		is_jump = false;

		////////////////////////////////////////////////////////////////
		////////////////           Fetch Stage          ////////////////
		////////////////////////////////////////////////////////////////

		// fetch next instruction from memory[pc]
		if ((sprintf(inst, "%04X", memory[pc]) < 0))
			handle_error();

		////////////////////////////////////////////////////////////////
		////////////////          Decode Stage          ////////////////
		////////////////////////////////////////////////////////////////

		opcode = get_hex_int(inst, 0);
		rd = get_hex_int(inst, 1);
		rs = get_hex_int(inst, 2);
		rt = get_hex_int(inst, 3);

		// If the instruction uses an immediate
		if (rd == 1 || rs == 1 || rt == 1) {
			// Get the immediate word from memory and assign it to imm
			imm = (short)memory[pc + 1];
			register_arr[1] = imm;
			is_imm = true;
		}
		else {
			imm = 0;
			register_arr[1] = 0;
		}

		// get the proper op enum for the opcode
		op = assign_op(opcode, is_imm);

		// write line into trace
		write_trace_line(trace, pc, register_arr, is_imm, imm);

		////////////////////////////////////////////////////////////////
		////////////////          Execute Stage         ////////////////
		////////////////////////////////////////////////////////////////

		execute_inst(op, &register_arr, rd, rs, rt, imm, &pc, &is_jump, &is_imm, memout);

		// End of iteration - make the necessary updates
		if (!is_jump) {
			if (is_imm)
				pc++;
			pc++;
		}
		opcount++;
	}

	////////////////////////////////////////////////////////////////
	////////////////          Wrap-up Stage         ////////////////
	////////////////////////////////////////////////////////////////

	// write memory image into memout as required
	if (memout_write(memout, memory) < 0)
		handle_error();

	write_opcount(opcount, count);
	write_regout(register_arr, regout);

	fclose(memin);
	fclose(memout);
	fclose(regout);
	fclose(trace);
	fclose(count);

	return 0;
}