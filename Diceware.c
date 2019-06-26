#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

char*** getPasswords(int passwordLength, int numPasswords);
char* getRandomWord(FILE* fp, int fileLength);
FILE* openFile(char* fileName, char* openType);
int lengthOfFile(FILE* fp);
void transformPassword(char** password, char* transformationTable);
char randomSpecialCharacter();

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
	signed char transformationTable[127];
	char transform = 0; //not transforming

	for (int i = 0; i < 127; i++)
		transformationTable[i] = 0;

	if (argc > 2)
	{
		numPasswords = strtol(argv[2], (char**) NULL, 10);
		
		if (argc > 3)
		{
			FILE* transformationFile;
			transformationFile = fopen(argv[3], "r");
			
			if (transformationFile == NULL)
			{
				printf("Could not open transformation file.");
				return 1;
			}

			transform = 1; //transforming

			char currentChar;
			//currentChar = fgetc(transformationFile);
			//fseek(transformationFile, 1, SEEK_CUR);
			char buffer[3];
			int currentPos = 0;
			char temp;
			while ((currentChar = fgetc(transformationFile)) != EOF && currentChar != 0)
			{
				printf("Current Char: %c (%d)\n", currentChar, currentChar);
				fseek(transformationFile, 1, SEEK_CUR); //skip the colon
				while ((temp = fgetc(transformationFile)) != '\n')
					buffer[currentPos++] = temp;
				if (currentPos == 1)
				{
					transformationTable[currentChar] = buffer[0];
					printf("Mapped: %c -> %c\n", currentChar, buffer[0]);
				}
				else
				{
					// if -1, then random special character
					if (strcmp(buffer, "{_}") == 0)
					{
						printf("buffer = {_}\n");
						transformationTable[currentChar] = -1;
					}
				}

				currentPos = 0;
			}
		}
	}

	for (int i = 0; i < 127; i++)
	{
		if (transformationTable[i] == 0)
			transformationTable[i] = i;

		printf("%c (%d) ->  %c (%d)\n", i, i, transformationTable[i], transformationTable[i]);
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
		if (transform == 1)
			transformPassword(passwords[password], transformationTable);
		
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

void transformPassword(char** password, char* transformationTable)
{
	for (int i = 0; i < sizeof(password) / sizeof(password[0]); i++)
	{
		if (random() % 3 == 0) // 33% chance
		{
			//printf("Character that random rolled for: %c (%d) -> (%d)\n", *password[i], *password[i], transformationTable[*password[i]]);
			if (*password[i] == -1)
				*password[i] = randomSpecialCharacter();
			else
				*password[i] = transformationTable[*password[i]];
		}
	}
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

char randomSpecialCharacter()
{
	static const char* specialCharacters = "!@#$%^&*()_+{}|:<>?~`";
	char ret = specialCharacters[random() % (sizeof(specialCharacters) / sizeof(specialCharacters[0]))];
	printf("Random special character returned: %c (%d)", ret, ret);
	return ret;
}

int lengthOfFile(FILE* fp)
{
	// file pointer, offset, position offset is relative to
	fseek(fp, 0L, SEEK_END);
	return ftell(fp);
}
