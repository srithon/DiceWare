#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char*** getPasswords(int passwordLength, int numPasswords);
char* getRandomWord(FILE* fp, int fileLength);
FILE* openFile(char* fileName, char* openType);
int lengthOfFile(FILE* fp);

// pass in number of words
int main(int argc, char ** argv)
{
	if (argc == 1)
	{
		printf("Pass in the number of words wanted!\n");
		return 1;
	}
	
	// this only reads the first digit
	//int n = *argv[1] - '0';
	

	int n = strtol(argv[1], (char**) NULL, 10);
	int numPasswords = 1;

	if (argc > 2)
	{
		numPasswords = strtol(argv[2], (char**) NULL, 10);
	}

	if (n < 0)
	{
		printf("Please pass in a positive number of words\n");
		return 1;
	}

	char*** passwords = getPasswords(n, numPasswords);

	int word, password;

	for (password = 0; password < numPasswords; password++)
	{
		for (word = 0; word < n; word++)
		{
			printf("%s ", passwords[password][word]);
			free(passwords[password][word]);
		}

		free(passwords[password]);
		printf("\n");
	}

	free(passwords);

	printf("\n");

	return 0;
}

char*** getPasswords(int n, int numPasswords)
{
	FILE* fp = openFile("WordListFinal", "r");
        int fileLength = lengthOfFile(fp);
	srand(time(0));
	//char** words = malloc(n * sizeof(char*));
	char*** passwords = malloc(numPasswords * sizeof(char**));

	int i, j;
	for (j = 0; j < numPasswords; j++)
	{
		passwords[j] = malloc(n * sizeof(char*));
		for (i = 0; i < n; i++)
		{
			passwords[j][i] = getRandomWord(fp, fileLength);
			//printf(words[i] + " ");
			//free(words[i]);
		}
	}
	fclose(fp);
	return passwords;
}

/*
 * Consider weighting words
 * so that words in the beginning
 * are prioritized.
 * Words at the end are less common
 * and thus harder to remember
 */
char* getRandomWord(FILE* fp, int fileLength)
{
	fseek(fp, rand() % fileLength, SEEK_SET);
	char currentChar;
       	while ((currentChar = fgetc(fp)) != '\n')
		if (currentChar == EOF)
			fseek(fp, ftell(fp) - (rand() % 100), SEEK_SET);
	// 8 because max word size is 8
	char* word = malloc(8 * sizeof(char));
	int currentIndex = 0;
	while ((currentChar = fgetc(fp)) != '\n')
	{
		word[currentIndex] = currentChar;
		currentIndex++;
		if (currentIndex == 7)
			break;
	}
	return word;
}

FILE* openFile(char* fileName, char* openType)
{
	FILE* file = fopen(fileName, openType);
	
	if (file == NULL)
	{
		printf("File could not be opened.");
		exit(1);
	}

	return file;
}

int lengthOfFile(FILE* fp)
{
	// file pointer, offset, position offset is relative to
	fseek(fp, 0L, SEEK_END);
	return ftell(fp);
}
