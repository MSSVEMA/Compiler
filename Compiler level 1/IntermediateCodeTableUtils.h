typedef struct intcodetable
{
	int opcode;
	int parameters_count;
	int *parameters;
};

typedef struct labeltable
{
	char label_name[10];
	int instruction_line;
};

labeltable label[10];
int label_table_index = 0;


int getOpCode(operations *opname_code,char *keyword)
{
	int i = 0;
	for (i = 0; i < opname_code->operations_count; i++)
	{
		if (stringComparison(opname_code->names[i], keyword) == 1)
			return i;
	}
}

int checkRegisterNames(char *register_name)
{
	int i = 0;
	char **registers_names = (char **)malloc(sizeof(char *) * 10);
	for (i = 0; i < 10; i++)
		registers_names[i] = (char *)malloc(sizeof(char) * 10);

	FILE *fp = fopen("registers_names.txt", "r");
	char *buffer = (char *)malloc(sizeof(char) * 10);
	i = 0;
	while (fscanf(fp, "%s", buffer) != -1)
	{
		copyString(buffer, registers_names[i]);
		i++;
	}

	for (i = 0; i < 6; i++)
	{
		if (stringComparison(register_name, registers_names[i]))
			return i;
	}

	fclose(fp);
	free(registers_names); free(buffer);
}

int findInLabelTable(char *label_name)
{
	int i = 0;
	for (i = 0; i < label_table_index; i++)
	{
		if (stringComparison(label[i].label_name, label_name))
			return label[i].instruction_line;
	}
}

int checkVariableNames(symboltable *symbol, int symbols_count, char *variable_name)
{
	int i = 0;
	for (i = 0; i < symbols_count; i++)
	{
		if (stringComparison(symbol[i].name, variable_name))
		{
			return symbol[i].address;
		}
	}
}

int checkOperrator(char *operrator)
{
	if (stringComparison("LT", operrator))
		return 1;
	if (stringComparison("LTEQ", operrator))
		return 2;
	if (stringComparison("EQ", operrator))
		return 3;
	if (stringComparison("GTEQ", operrator))
		return 4;
	if (stringComparison("GT", operrator))
		return 5;
}

void fillIntermediateTable(intcodetable *intcode, int instructions_count,int opcode,char *parameters,symboltable *symbol,int symbols_count)
{
	char *register_name = (char *)malloc(sizeof(char) * 10);
	char *variable_name = (char *)malloc(sizeof(char) * 10);
	char *buffer = (char *)malloc(sizeof(char) * 10);
	char *param1 = (char *)malloc(sizeof(char) * 2);
	char *param_without_braces = (char *)malloc(sizeof(char) * 2);
	char *param2 = (char *)malloc(sizeof(char) * 2);
	char *str_number = (char *)malloc(sizeof(char) * 10);
	char *operrator = (char *)malloc(sizeof(char) * 2);
	int register_index, parameters_count, variable_index, operrator_index;
	int i = 0, j;
	int index=0;
	int *if_else_endif_stack = (int *)malloc(sizeof(int) * 10);

	if (opcode == 1)
	{
		getString(parameters, &i, ' ', param1);
		param1[strLength(param1) - 1] = '\0';
		getString(parameters, &i, ' ', param2);

		if (param2[strLength(param2)-1] == 'X')
		{
			opcode = 1;
		}
		else
			opcode = 2;
	}

	switch (opcode)
	{
	case 1://MOVE Reg to Mem
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].opcode = 1;
		intcode[instructions_count].parameters_count = 2;
		if (findCharacterInString(param1, '['))
		{
			getStrNumberBetweenBraces(param1, str_number);
			index = getNumber(str_number, '\0');
		}
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		i = 0;
		getString(param1, &i, '[', param_without_braces);
		variable_index = checkVariableNames(symbol, symbols_count, param_without_braces);
		register_index = checkRegisterNames(param2);
		intcode[instructions_count].parameters[0] = variable_index + index;
		intcode[instructions_count].parameters[1] = register_index;
		break;
	case 2://MOVE Mem to Reg
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].opcode = 2;
		intcode[instructions_count].parameters_count = 2;
		if (findCharacterInString(param1, '['))
		{
			getStrNumberBetweenBraces(param2, str_number);
			index = getNumber(str_number, '\0');
		}
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		register_index = checkRegisterNames(param1);
		i = 0;
		getString(param1, &i, '[', param_without_braces);
		variable_index = checkVariableNames(symbol, symbols_count, param_without_braces);
		intcode[instructions_count].parameters[0] = register_index;
		intcode[instructions_count].parameters[1] = variable_index + index;
		break;
	case 3://ADD
		j = 0;
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].parameters_count = parameters_count;
		intcode[instructions_count].opcode = opcode;
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		while (j<parameters_count)
		{
			getString(parameters, &i, ',', buffer);
			trimSpaces(buffer);
			register_index = checkRegisterNames(buffer);
			intcode[instructions_count].parameters[j] = register_index;
			j++;
		}
		break;
	case 4://SUB
		j = 0;
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].parameters_count = parameters_count;
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		intcode[instructions_count].opcode = opcode;
		while (j<parameters_count)
		{
			getString(parameters, &i, ',', buffer);
			trimSpaces(buffer);
			register_index = checkRegisterNames(buffer);
			intcode[instructions_count].parameters[j] = register_index;
			j++;
		}
		break;
	case 5://MUL
		j = 0;
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].parameters_count = parameters_count;
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		intcode[instructions_count].opcode = opcode;
		while (j<parameters_count)
		{
			getString(parameters, &i, ',', buffer);
			register_index = checkRegisterNames(buffer);
			intcode[instructions_count].parameters[j] = register_index;
			j++;
		}
		break;
	case 6://JMP
		parameters_count = 1;
		getString(parameters, &i, ' ', param1);
		intcode[instructions_count].parameters_count = parameters_count;
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*(parameters_count + 1));
		intcode[instructions_count].parameters[0] = findInLabelTable(param1);
		break;
	case 7://IF
		parameters_count = 3; i = 0;
		getString(parameters, &i, ' ', param1);
		getString(parameters, &i, ' ', operrator);
		getString(parameters, &i, ' ', param2);
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*(parameters_count + 1));
		intcode[instructions_count].parameters_count = 3;
		register_index = checkRegisterNames(param1);
		intcode[instructions_count].parameters[0] = register_index;
		operrator_index = checkOperrator(operrator);
		intcode[instructions_count].parameters[1] = operrator_index;
		register_index = checkRegisterNames(param2);
		intcode[instructions_count].parameters[2] = register_index;
		//instruction_stack[]
		break;
	case 8://EQ
		break;
	case 9://LT
		break;
	case 10://GT
		break;
	case 11://LTEQ
		break;
	case 12://GTEQ
		break;
	case 13://PRINT
		parameters_count = countDelimiter(parameters, ',');
		intcode[instructions_count].parameters_count = parameters_count;
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		getString(parameters, &i, ',', buffer);
		trimSpaces(buffer);
		if (buffer[strLength(buffer) - 1] == 'X')
		{
			intcode[instructions_count].parameters_count = 1;//1 for Registers
			register_index = checkRegisterNames(buffer);
			intcode[instructions_count].parameters[0] = register_index;
			intcode[instructions_count].opcode = opcode;
		}
		else
		{
			intcode[instructions_count].parameters_count = 0;//0 for Variables
			if (findCharacterInString(buffer, '['))
			{
				getStrNumberBetweenBraces(buffer, str_number);
				index = getNumber(str_number, '\0');
			}
			i = 0;
			getString(buffer, &i, '[', param_without_braces);
			variable_index = checkVariableNames(symbol, symbols_count, param_without_braces);
			intcode[instructions_count].parameters[0] = variable_index + index;
			intcode[instructions_count].opcode = opcode;
		}
		
		
		break;
	case 14://READ
		parameters_count = countDelimiter(parameters, ',');
		getString(parameters, &i, '\0', register_name);
		register_index = checkRegisterNames(register_name);
		intcode[instructions_count].parameters = (int *)malloc(sizeof(int)*parameters_count);
		intcode[instructions_count].parameters[0] = register_index;
		intcode[instructions_count].opcode = opcode;
		intcode[instructions_count].parameters_count = parameters_count;
		break;
	case 15://ENDIF
		break;
	}

	//free(register_name); free(variable_name); free(buffer);
	//free(param1); free(param2);
}

void writeToIntermediateCodeTable(FILE *fp, intcodetable *intcode,int *instructions_count, symboltable *symbol, int symbols_count,operations *opname_code)
{
	int i = 0;
	char *keyword = (char *)malloc(sizeof(char) * 6);
	char *instruction = (char *)malloc(sizeof(char) * 50);
	char *parameters = (char *)malloc(sizeof(char) * 20);
	char *label_name = (char *)malloc(sizeof(char) * 10);
	int opcode;

	while (fgets(instruction, 50, fp))
	{
		i = 0;

		if (stringComparison(instruction, "END") == 1)
			break;
		(*instructions_count)++;

		//check for label

		if (instruction[strLength(instruction) - 1] == ':')//Label Exists
		{
			getString(instruction, &i, ':', label_name);
			copyString(label[label_table_index].label_name, label_name);
			label[label_table_index].instruction_line = (*instructions_count);
			label_table_index++;
		}
		else
		{
			getString(instruction, &i, ' ', keyword);
			opcode = getOpCode(opname_code, keyword) + 1;

			getString(instruction, &i, '\0', parameters);

			fillIntermediateTable(intcode, (*instructions_count) - 1, opcode, parameters, symbol, symbols_count);
		}

	}

}