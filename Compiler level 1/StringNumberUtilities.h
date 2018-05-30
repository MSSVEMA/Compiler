int strLength(char *string)
{
	int i = 0;
	while (string[i] != '\0')
		i++;
	return i;
}

int findCharacterInString(char *string,char character)
{
	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] == character)
			return 1;
		i++;
	}
	return 0;
}

void getStrNumberBetweenBraces(char *source, char *destination)
{
	int i = 0, k = 0;
	while (source[i] != '[')
	{
		i++;
	}
	i++;
	while (source[i] != ']')
	{
		destination[k] = source[i];
		i++;
		k++;
	}
	destination[k] = '\0';
}

int getNumber(char *number, char delimiter)
{
	int i = 0, j, temp = 0;

	while (number[i] != delimiter&&number[i] != '\0')
	{
		j = number[i] - '0';
		temp = (temp * 10) + j;
		i++;
	}
	return temp;
}

void getString(char *source, int *ptr, char delimiter, char *destination)
{
	int k = 0;

	while (source[*ptr] != delimiter&&source[*ptr] != '\0'&&source[*ptr] != '\n')
	{
		destination[k] = source[*ptr];
		k++;
		(*ptr)++;
	}
	destination[k] = '\0';
	(*ptr)++;
}


int countDelimiter(char *string, char delimiter)
{
	int i = 0, count = 0;
	while (1)
	{
		if (string[i] == delimiter)
			count++;
		else if (string[i] == '\0')
		{
			count++;
			break;
		}
		i++;
	}
	return count;
}



void copyString(char *source, char *destination)
{
	int i = 0;
	while (source[i] != '\0')
	{
		destination[i] = source[i];
		i++;
	}
	destination[i] = '\0';
}

void getSubString(char *source, int start, int end, char *destination)
{
	int i, k = 0;
	for (i = start; i <= end; i++)
	{
		destination[k] = source[i];
		k++;
	}
	destination[k] = '\0';
}

void trimSpaces(char *string)
{
	int i = 0, l = strLength(string);
	int start, end;
	char  *substring;

	if (l == 0)
	{
		return;
	}
	
	while (string[i] == ' '&&i >= 0 && i < l)
		i++;

	if (i == l)//If everything is space
	{
		substring = (char *)malloc(sizeof(char)*1);
		substring[0] = '\0';
		copyString(substring, string);
		return;
	}

	start = i;

	i = l - 1;
	while (string[i] == ' '&&i >= 0 && i < l)
		i--;
	end = i;

	substring = (char *)malloc(sizeof(char)*(end - start + 2));
	getSubString(string, start, end, substring);
	copyString(substring, string);
}

void strConcat(char *trimmed_string, int *i, char *splitted_marks)
{
	int k = 0;
	while (splitted_marks[k] != '\0')
	{
		trimmed_string[*i] = splitted_marks[k];
		(*i)++;
		k++;
	}
	trimmed_string[*i] = ',';
	(*i)++;
}

void stringLower(char *string)
{
	int i = 0;
	while (string[i] != '\0')
	{
		if (string[i] >= 65 && string[i] <= 90)
			string[i] = string[i] + 32;
		i++;

	}
}

int stringComparison(char *str1, char *str2)
{
	int i = 0, l1 = strLength(str1), l2 = strLength(str2);

	if (l1 == 0 || l2 == 0 || l1 != l2)
		return 0;

	while (str1[i] != '\0'&&str2[i] != '\0')
	{
		if (str1[i] == str2[i])
			i++;
		else
			return 0;
	}
	return 1;
}