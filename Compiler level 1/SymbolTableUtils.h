typedef struct symboltable
{
	char *name;
	int address;
	int size;
};

void dataDeclaration(symboltable *symbol, char *symbol_name, int symbol_index,int *memory,int size)
{
	symbol[symbol_index].name = (char *)malloc(sizeof(char) * 10);
	copyString(symbol_name, symbol[symbol_index].name);
	symbol[symbol_index].address = *memory;
	symbol[symbol_index].size = size;
	(*memory) = (*memory) + size;

}

void constDeclaration(symboltable *symbol, char *symbol_name, int symbol_index, int *memory, int value)
{
	symbol[symbol_index].name = (char *)malloc(sizeof(char) * 10);
	copyString(symbol_name, symbol[symbol_index].name);
	symbol[symbol_index].address = *memory;
	symbol[symbol_index].size = -value;
	(*memory) = (*memory) + 1;

}


void addToSymbolTable(symboltable *symbol,char *instruction, int symbol_index,int *memory)
{
	char *keyword = (char *)malloc(sizeof(char) * 6);
	char *symbol_name = (char *)malloc(sizeof(char) * 10);
	char *size_str = (char *)malloc(sizeof(char) * 3);
	char *symbol_name_without_size = (char *)malloc(sizeof(char) * 10);
	char *value_str = (char *)malloc(sizeof(char) * 10);
	int i = 0;
	int size = 1;
	int value = 0;

	getString(instruction, &i, ' ', keyword);
	getString(instruction, &i, ' ', symbol_name);

	if (stringComparison(keyword, "DATA"))
	{
		if (findCharacterInString(symbol_name, '['))
		{
			i = 0;
			getString(symbol_name, &i, '[', symbol_name_without_size);
			getString(symbol_name, &i, ']', size_str);
			size = getNumber(size_str, '\0');
			dataDeclaration(symbol, symbol_name_without_size, symbol_index, memory, size);
		}
		else
		{
			dataDeclaration(symbol, symbol_name, symbol_index, memory, size);
		}
	}
	else//CONST
	{
		getString(instruction, &i, ' ', value_str);
		getString(instruction, &i, ' ', value_str);
		value = getNumber(value_str, '\0');
		constDeclaration(symbol, symbol_name, symbol_index, memory, value);
	}

	free(keyword); free(symbol_name);
	free(size_str); free(value_str);
	free(symbol_name_without_size);
}



FILE *writeToSymbolTable(char *filename, symboltable *symbol,int *memory,int *symbol_count)
{
	int i = 0;
	FILE *fp = fopen(filename, "r");
	char *instruction = (char *)malloc(sizeof(char) * 50);



	while (fgets(instruction, 50, fp))
	{
		instruction[strLength(instruction) - 1] = '\0';

		if (stringComparison(instruction, "START:") == 1)
			break;
		(*symbol_count)++;
		addToSymbolTable(symbol, instruction, (*symbol_count) - 1, memory);

	}
	free(instruction);
	return fp;

}