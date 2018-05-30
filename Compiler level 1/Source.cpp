#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct operations
{
	char **names;
	int operations_count;
};

#include "StringNumberUtilities.h"
#include "SymbolTableUtils.h"
#include "IntermediateCodeTableUtils.h"



void getOpCodes(char **opcodes, int n)
{
	FILE *fp = fopen("opcodes.txt", "r");
	char *single_opcode = (char *)malloc(sizeof(char) * 10);
	int i = 0;
	
	while (fscanf(fp, "%s", single_opcode) != -1)
	{
		copyString(single_opcode, opcodes[i]);
		i++;
	}

	fclose(fp);
	free(single_opcode);

}

void drive_interpreter(intcodetable *intcode, int instructions_count, symboltable *symbol, int symbols_count)
{
	//peeep peeeep

	int *registers_memory = (int *)malloc(sizeof(int) * 8);
	int *variables_memory = (int *)malloc(sizeof(int)*(symbol[symbols_count - 1].address + 1));
	int i = 0;
	//for (i = 0; i < symbols_count; i++)
	//{

	//}



	for (i = 0; i < instructions_count - 1; i++)
	{
		switch (intcode[i].opcode)
		{
		case 1://MOV Mem to Reg
			variables_memory[intcode[i].parameters[0]] = registers_memory[intcode[i].parameters[1]];
			printf("->%d\n", variables_memory[intcode[i].parameters[0]]);
			break;
		case 2://MOV Reg to Mem
			registers_memory[intcode[i].parameters[0]] = variables_memory[intcode[i].parameters[1]];
			printf("->%d\n", registers_memory[intcode[i].parameters[0]]);
			break;
		case 3://ADD
			registers_memory[intcode[i].parameters[0]] = registers_memory[intcode[i].parameters[2]] + registers_memory[intcode[i].parameters[1]];
			break;
		case 4://SUB
			registers_memory[intcode[i].parameters[0]] = registers_memory[intcode[i].parameters[2]] - registers_memory[intcode[i].parameters[1]];
			break;
		case 5://MUL
			registers_memory[intcode[i].parameters[0]] = registers_memory[intcode[i].parameters[2]] * registers_memory[intcode[i].parameters[1]];
			break;
		case 13://PRINT
			if (intcode[i].parameters_count == 1)
			{
				//printf("\nValue of %cX is %d\n", intcode[i].parameters[0] + 65, registers_memory[intcode[i].parameters[0]]);
				printf("->%d\n",registers_memory[intcode[i].parameters[0]]);
			}
			else
			{
				//printf("\nValue is %d\n", variables_memory[intcode[i].parameters[0]]);
				printf("->%d\n", variables_memory[intcode[i].parameters[0]]);
			}
			break;
		case 14://READ
			printf("\nEnter Value for %cX\n", intcode[i].parameters[0] + 65);
			scanf("%d", &registers_memory[intcode[i].parameters[0]]);
			//printf("%d\n", registers_memory[intcode[i].parameters[0]]);
			break;
		}
	}
}

int main()
{
	int i;
	char **opcodes = (char **)malloc(sizeof(char *) * 15);
	for (i = 0; i < 15; i++)
		opcodes[i] = (char *)malloc(sizeof(char) * 10);

	char filename[20] = "instructions.txt";
	FILE *fp;

	int memory = 0;
	int symbols_count = 0;
	int instructions_count = 0;

	getOpCodes(opcodes, 15);

	operations *opname_code = (operations *)malloc(sizeof(operations));
	opname_code->names = opcodes;
	opname_code->operations_count = 14;

	symboltable *symbol = (symboltable *)malloc(sizeof(symboltable) * 10);
	intcodetable *intcode = (intcodetable *)malloc(sizeof(intcodetable) * 100);

	fp = writeToSymbolTable(filename, symbol, &memory, &symbols_count);

	writeToIntermediateCodeTable(fp, intcode, &instructions_count, symbol, symbols_count, opname_code);



	for (int i = 0; i < symbols_count; i++)
	{
		printf("Name:%s\n Address:%d\n Size:%d\n", symbol[i].name, symbol[i].address, symbol[i].size);
		printf("\n");
	}

	for (int i = 0; i < instructions_count; i++)
	{
		printf("Opcode:%d\n Parameters Count:%d\n Parameters:\n", intcode[i].opcode, intcode[i].parameters_count);
		for (int j = 0; j < intcode[i].parameters_count; j++)
		{
			printf("%d\t", intcode[i].parameters[j]);
		}
		printf("\n");
	}

	drive_interpreter(intcode, instructions_count, symbol, symbols_count);

	system("pause");
	return 0;
}