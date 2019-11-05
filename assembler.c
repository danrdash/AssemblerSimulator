#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define MAXCHAR 500
#define MEM 1<<16 
#pragma warning(disable:4996)
char** splitline(char* line, char **inputarr, char *token) { //inserts tokens into given input array
	char temp1[MAXCHAR];
	int i = 0;
	strcpy(temp1, line);
	token = strtok(temp1, " ,\n	");
	//printf("%s\n", token);
	strcpy(inputarr[i], token);
	i++;
	while (token != NULL && i < 5)
	{
		token = strtok(NULL, " ,\n	");
		if (strcmp(token, "#") == 0)
			break;
		strcpy(inputarr[i], token);
		//sprintf(*(memo+mindex),"%s", inputarr[i]);
		i++;
	}

	return inputarr;
}
int getOpNum(char opcodes[][5], char *op, int globint) { //get the corresponding integer of an opcode inside opcodes array
	for (globint = 0; globint < 16; globint++) {
		if (strcmp(op, opcodes[globint]) == 0) {
			return globint;
		}
	}
	printf("Did not find a corresponding opcode number\n");
	return -1;
}
int getRegNum(char regs[][6], char *reg, int globint) { //get the corresponding integer of an opcode inside opcodes array
	for (globint = 0; globint < 16; globint++) {
		if (strcmp(reg, regs[globint]) == 0) {
			return globint;
		}
	}

	printf("Did not find a corresponding registiry number\n");
	return -1;
}

typedef struct lbl  //defining suitable structure
{
	int address;
	char label[50];
	struct lbl *nlabel;
} Label;
void printLabels(Label *head) {
	while (head != NULL) {
		//		printf("Label string: %s\nLabel address: %d\n", head->label, head->address);
		head = head->nlabel;
	}
}
int getAdd(Label *head, char *label) {
	while (head != NULL) {
		if (strcmp(label, head->label) == 0) {
			return head->address;
		}
		head = head->nlabel;
	}
	return -1;

}
void freehead(Label* head) {
	Label *temp;
	while (head != NULL)
	{
		temp = head;
		head = head->nlabel;
		free(temp);
	}
}

int main(int argc, const char * argv[]) {
	int x = 0, lindex = 0, regNum = 0, opNum = 0, isImm = 0, isHex = 0, isLarger = 0, sgn = 0, rs = 0, rt = 0, rd = 0, mindex = 0, max_mem = 0, cur_mem = 0;
	FILE *fp = NULL, *fpmem = NULL;
	Label *newl = NULL, *head = NULL;
	char str[MAXCHAR], **tokarr = NULL, *word_add, *word_data, memo[MEM][10], *token = NULL, *temp = NULL;
	char opcodes[][5] = { "add", "sub", "and", "or", "sll", "sra","led", "beq" , "bgt", "ble", "bne", "jal", "lw", "sw", "lhi", "halt" }; // gotta remember that the 6th function is reserved
	char regs[][6] = { "$zero","$imm","$v0","$a0","$a1","$t0","$t1","$t2","$t3","$s0","$s1","$s2","$gp","$sp","$fp","$ra" };

	if (argc < 3) {
		printf("wrong number of arguments!\n");
		exit(1);
	}
	for (x = 0; x<MEM; x++)
		sprintf(*(memo + x), "0000"); //initiate every entrance in memo array to zeroes

	fp = fopen(argv[1], "r");
	token = (char*)calloc(6, sizeof(char));
	if (token == NULL)
		printf("memory allocation failed");
	tokarr = (char**)calloc(6, sizeof(char *)); // init array of pointers to strings
	for (x = 0; x < 6; x++) {
		tokarr[x] = (char*)calloc(81, sizeof(char)); // init string array
	}
	while (fgets(str, MAXCHAR, fp)) { // first run
		if (strchr(str, '.') != NULL) { //the function is .word
			strtok(str, " ");
			word_add = strtok(NULL, " ");
			word_data = strtok(NULL, " ");
			if ((temp = strchr(word_data, '\n')) != NULL) {
				*temp = '\0';
			} 
			//sprintf(*(memo+mindex),"Word address %s ; word Data %s\n", word_add, word_data);
			sprintf(*(memo + atoi(word_add)), "%04hX", atoi(word_data));
			//printf("%s", *(memo + atoi(word_add)));
			cur_mem = atoi(word_add);
			if (max_mem < cur_mem) max_mem = cur_mem;
		}
		if (strchr(str, ':') != NULL) // there is no : or space in the line, then this is a label line
		{
			newl = (Label*)malloc(sizeof(Label)); //allocate new memory
			if (newl == NULL)
				printf("New label failed to initiate");
			x = 0;
			while (*(str + x) != ':') {
				newl->label[x] = *(str + x);
				x++;
			}
			newl->label[x] = '\0'; // locked the label char array with \0
			newl->address = lindex; //counter for location of the label in text file
			newl->nlabel = head; // updating new label
			head = newl; // updating head of the list

		}
		if ((strchr(str, ',') != NULL)) {  //if this is an instruction line then update counter in relevant way
			splitline(str, tokarr, token); // to check if there is an immediate
			for (x = 0; x < 5; x++) {
				//sprintf(*(memo+mindex),"%s\n", tokarr[x]);
			}
			for (x = 0; x < 5; x++) {
				if (strcmp(tokarr[x], "$imm") == 0) { // check if we have an immediate
					isImm = 1;
					break;
				}
				isImm = 0;
			}
			if (isImm == 0)
				lindex++;
			if (isImm == 1)
				lindex = lindex + 2;
		}

	}
	fclose(fp);
	//printf("%s\n", *(memo + 1024));
	printLabels(head);
	
	fp = fopen(argv[1], "r");
	fpmem = fopen(argv[2], "w");

	while (fgets(str, MAXCHAR, fp)) { // second run
		if (strchr(str, ':') != NULL)// there is no : or space in the line, then this is a label line
			continue;
	

		
		if ((strchr(str, ',') != NULL)) {
			splitline(str, tokarr, token);
			//			for (x = 0; x < 5; x++) {
			//				printf("%s ", tokarr[x]);
			//
			//			}
			//			printf("\n");
			for (x = 0; x < 5; x++) {
				if (strcmp(tokarr[x], "$imm") == 0) { // check if we have an immediate
					isImm = 1;
					break;
				}
				isImm = 0;
			}
			opNum = getOpNum(opcodes, tokarr[0], opNum); //aquire numbers before switching immediate
			rd = getRegNum(regs, tokarr[1], rd);
			rs = getRegNum(regs, tokarr[2], rs);
			rt = getRegNum(regs, tokarr[3], rt);
			if (isImm == 0) // easy instructions, lets print to file.
			{ // if there is no immediate
				sprintf(*(memo + mindex), "%01X%01X%01X%01X", opNum, rd, rs, rt);
				//				printf("%s\n", *(memo + mindex));
				mindex++;
				//printf("Finished reading a line\n");
			}
			else if (isImm = 1) { // we have an immediate
				if (*(tokarr[4]) > 64 && (*tokarr[4] < 173)) {//first character is a character, immediate is a label
					_itoa(getAdd(head, tokarr[4]), tokarr[4], 10); //replace label with address we recieved from first run
					sprintf(*(memo + mindex), "%01X%01X%01X%01X", opNum, rd, rs, rt);
					sprintf(*(memo + mindex + 1), "%04hX", atoi(tokarr[4]));
					//					printf("%s\n%s\n", *(memo + mindex), *(memo + mindex + 1));
					mindex = mindex + 2;
					//printf("Finished reading a line\n");
				}
				else if (*tokarr[4] == '0' && *(tokarr[4] + 1) == 'x') { //immediate is written in hex
					sprintf(*(memo + mindex), "%01X%01X%01X%01X", opNum, rd, rs, rt);
					sprintf(*(memo + mindex + 1), "%04hX", atoi(tokarr[4]));
					//					printf("%s\n%s\n", *(memo + mindex), *(memo + mindex + 1));
					mindex = mindex + 2;
					//printf("Finished reading a line\n");
				}
				else { //immediate is a number
					sprintf(*(memo + mindex), "%01X%01X%01X%01X", opNum, rd, rs, rt);
					sprintf(*(memo + mindex + 1), "%04hX", atoi(tokarr[4]));
					//					printf("%s\n%s\n", *(memo + mindex), *(memo + mindex + 1));
					mindex = mindex + 2;
					//printf("Finished reading a line\n");
				}


			}
		}

	}
	//printf("%s\n", *(memo + 1024));
	if (max_mem < mindex) max_mem = mindex;
	if (max_mem < MEM) {
		for (x = 0; x <= max_mem; x++)
			fprintf(fpmem, "%s\n", *(memo + x));
	}
	else {
		for (x = 0; x < MEM; x++)
			fprintf(fpmem, "%s\n", *(memo + x));
	}
	freehead(head);
	for (x = 0; x < 6; x++) {
		free(tokarr[x]);
	}
	free(tokarr);
	free(token);
	fclose(fp);
	//scanf("%d", &x);
	return 0;

}