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

// this function frees all of the words while it is creating the final string
void semiDestructiveTransformPassword(char** wordList, int numWords, char* transformationTable, char* destinationBuffer, int bufferSize);

void testFile();

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
			FILE* transformationFile = fopen(argv[3], "r");

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

			fclose(transformationFile);

			for (int i = 0; i < 127; i++)
			{
				if (transformationTable[i] == 0)
					transformationTable[i] = i;

				printf("%c (%d) ->  %c (%d)\n", i, i, transformationTable[i], transformationTable[i]);
			}
		}
	}

	if (n <= 0)
	{
		printf("Please pass in a positive number of words\n");
		return 1;
	}

	char*** passwords = getPasswords(n, numPasswords);

	int word, password;

	char* transformedPassword;
	int bufferSize = 10 * n - 1;

	if (transform == 1)
	{
		transformedPassword = calloc(bufferSize, sizeof(char));
	}
		// 9 chars: (8 char max word length + 1 null character + 1 dividing character) - 1 because no dividing character at the end

	for (password = 0; password < numPasswords; password++)
	{
		//if (transform == 1)
		//	transformPassword(passwords[password], transformationTable);

		if (transform == 1)
		{
			//this function frees all the words!
			semiDestructiveTransformPassword(passwords[password], n, transformationTable, transformedPassword, bufferSize);
			printf("%s\n", transformedPassword);
		}
		else
		{
			for (word = 0; word < n; word++)
			{
				printf("%s ", passwords[password][word]);
				free(passwords[password][word]);
			}
			printf("\n");
		}

		free(passwords[password]);
	}

	free(passwords);

	if (transform == 1)
		free(transformedPassword);

	printf("\n");

	return 0;
}

void testFile()
{
	FILE* fp = openFile("WordListFinal", "r");
	char currentChar;
	while ((currentChar = fgetc(fp)) != EOF)
	{
		if (currentChar == 10)
			continue;
		if (!(currentChar >= 'a' && currentChar <= 'z' || currentChar >= 'A' && currentChar <= 'Z'))
			printf("Char: %c", currentChar);
	}
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

void semiDestructiveTransformPassword(char** wordList, int numWords, char* transformationTable, char* destinationBuffer, int bufferSize)
{
	unsigned char transformationCharacter = 0;
	unsigned int currentIndex = 0;
	char transformSpace = transformationTable[' '];
	for (int i = 0; i < numWords; i++)
	{
		//printf("i: %d; Original Word: %s\n", i, wordList[i]);
		if (i != 0)
			if (transformSpace != ' ' && random() % 3 == 0)
				destinationBuffer[currentIndex++] = (transformSpace == -1) ? randomSpecialCharacter() : transformSpace;
			else
				destinationBuffer[currentIndex++] = ' ';
		for (int j = 0; j < strlen(wordList[i]); j++, currentIndex++) // -1 because last character is a null character
		{
			if (random() % 3 == 0)
			{
				transformationCharacter = transformationTable[wordList[i][j]];

				destinationBuffer[currentIndex] =
					(transformationCharacter == -1) ?
						randomSpecialCharacter() : transformationCharacter;
			}
			else
			{
				destinationBuffer[currentIndex] = wordList[i][j];
			}
			//printf("i: %d; destinationBuffer: %s\n", i, destinationBuffer);
		}

		free(wordList[i]);
	}

	for (int i = currentIndex; i < bufferSize; i++)
		destinationBuffer[i] = 0;

	//free(wordList);		already freeing in calling function
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
	do
	{
		fseek(fp, rand() % fileLength, SEEK_SET);
	} while (ftell(fp) + 16 > fileLength);

	char currentChar;
  while ((currentChar = fgetc(fp)) != '\n');

	// 8 because max word size is 8 + 1 because null character?
	char* word = malloc(9 * sizeof(char));
	//13.539 seconds with calloc
	//12.974 seconds with malloc
	int currentIndex = 0;
	while ((currentChar = fgetc(fp)) != '\n')
	{
		word[currentIndex] = currentChar;
		currentIndex++;

		//if (currentIndex == 7)
		//	break;
	}

	word[currentIndex] = 0;
	//word[currentIndex] = 0; // null character

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
	static const char* specialCharacters = "!^*_+|<>~";
	char ret = specialCharacters[random() % (sizeof(specialCharacters) / sizeof(specialCharacters[0]))];
	//printf("Random special character returned: %c (%d)", ret, ret);
	return ret;
}

int lengthOfFile(FILE* fp)
{
	// file pointer, offset, position offset is relative to
	fseek(fp, 0L, SEEK_END);
	return ftell(fp);
}
