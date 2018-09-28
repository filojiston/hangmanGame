#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <string.h>


typedef enum inputStates 
{
	IS_PUNCT,
	IS_DIGIT,
	IS_SPACE,
	TOO_LONG,
	ALREADY_ENTERED,
	EMPTY,
	VALID,
}inputStates;


char *pickWord();
void setUserWord(char *userWord, int wordLength);
void printUserWord(const char *userWord);
void updateUserWord(const char *secretWord, char *userWord, char userGuess, int *trueLetters);
char handleUserInput();
inputStates checkUserInput(char *userInput);

int main(int argc, char const *argv[])
{
	srand(time(NULL));

	const char *secretWord = pickWord();
	if (secretWord == NULL) {
		perror("words.txt could not initialize!\n");
		return 1;
	}
	char *userWord = (char*)malloc(sizeof(char) * strlen(secretWord) - 1);
	char userInput;
	int chancesLeft = 10;
	int trueLetters = 0;

	setUserWord(userWord, strlen(secretWord) - 1);
	printf("\nChances left: %d\n", chancesLeft);
	printUserWord(userWord);

	do {
		userInput = handleUserInput();
		updateUserWord(secretWord, userWord, userInput, &trueLetters);
		chancesLeft--;
		if (chancesLeft > 0)
			printf("\nChances left: %d\n\n", chancesLeft);
		printUserWord(userWord);
		
	} while (chancesLeft > 0 && trueLetters != strlen(secretWord) - 1);

	if (chancesLeft > 0)
		printf("\nCongratulations, you won!\n");
	else {
		printf("\nSorry, you lose!\n");
		printf("\nThe secret word was: %s\n", secretWord);
	}

	return 0;
}

//has a little side effect (counts how many words in words.txt)
char *pickWord() 
{
	FILE *ptr_file;

	//open words.txt file
	ptr_file = fopen("words.txt", "r");
	
	//if any errors occurs
	if (!ptr_file) 
		return NULL;

	int wordCount = 0;
	char *buffer = (char*)malloc(sizeof(char) * 100);

	//counts how many words in words.txt
	while (fgets(buffer, 100, ptr_file) != NULL) {
		wordCount++;
	}

	//re-positioning the file pointer
	ptr_file = fopen("words.txt", "r");

	//all words in an array
	char *words[wordCount];
	int i;

	i = 0;

	//fill up the array with words in words.txt
	while (fgets(buffer, 100, ptr_file) != NULL) {
		words[i] = strdup(buffer);
		i++;
	}

	//select a random word in words array
	int selectRandom = (rand() % wordCount) + 1; 

	//close the file and free memory
	fclose(ptr_file);
	free(buffer);

	return words[selectRandom];
}

void setUserWord(char *userWord, int wordLength)
{
	int i;

	for (i = 0; i < wordLength; i++)
		userWord[i] = '_';

	userWord[wordLength] = '\0';
}

void printUserWord(const char *userWord)
{
	int i;

	printf("\n");

	for (i = 0; i < strlen(userWord); i++) 
		printf("%c ", userWord[i]);

	printf("\n");
}

//and counts how many letters guessed right for decide win or lose
void updateUserWord(const char *secretWord, char *userWord, char userGuess, int *trueLetters)
{
	int i;

	for (i = 0; i < strlen(secretWord); i++)
		if (userGuess == secretWord[i]) {
			userWord[i] = userGuess;
			*(trueLetters) += 1;
		}
}

//it gets input from user and checks it until it's valid
//when the input is valid, just returns it
char handleUserInput()
{
	
	inputStates inputValidation = EMPTY;
	char *userInput = (char*)malloc(sizeof(char) * 100);
	char userChar;

	do {
		printf("Please enter your guess: ");
		fgets(userInput, 100, stdin);
		inputValidation = checkUserInput(userInput);

	} while (inputValidation != VALID);

	userChar = userInput[0];
	return userChar;
}

//checks the user input
inputStates checkUserInput(char *userInput)
{
	static char enteredLetters[30];
	static int s = 0;

	int i;

	if (s == 0)
		for (i = 0; i < 30; i++)
			enteredLetters[i] = '-';

	int wordLength = strlen(userInput) - 1;
	userInput[wordLength] = '\0';

	char userChar = userInput[0];

	if (wordLength != 1 && userChar != '\0' && userChar != ' ') {
		printf("\nInput too long, just enter a letter.\n");
		return TOO_LONG;
	}

	if (isdigit(userChar)) {
		printf("\nDon't enter a digit please.\n");
		return IS_DIGIT;
	}
	else if (ispunct(userChar)) {
		printf("\nI can't let you enter a punctuation.\n");
		return IS_PUNCT;
	} 
	else if (userChar == ' ') {
		printf("\nSpace doesn't count in this game mate.\n");
		return IS_SPACE;
	}
	else if (userChar == '\0') {
		printf("\nEmpty entry!\n");
		return EMPTY;
	}

	for (i = 0; i < 30; i++) {
		if (userChar == enteredLetters[i]) {
			printf("\nYou've already guessed this letter.\n");
			return ALREADY_ENTERED;
		}
	}

	enteredLetters[s] = userChar;
	s++;

	return VALID;
}
