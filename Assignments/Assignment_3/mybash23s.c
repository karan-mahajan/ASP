//@Author: Karan Mahajan

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/wait.h>
#include <fcntl.h>

#define MAX_ARGUMENTS 3
#define MAX_PIPING 3
#define MAX_CONDITIONAL_EXECUTION 3
#define MAX_COMMAND_LENGTH 4096

// Function to check if the current index is escape char or not
int checkEscapeChar(const char *userInput, int currentValue)
{

    if (currentValue > 0 && userInput[currentValue - 1] == '\\')
    {
        int count = 0;
        while (currentValue > 0 && userInput[currentValue - 1] == '\\')
        {
            count++;
            currentValue--;
        }
        return count % 2 != 0;
    }
    return 0;
}

// Function to add extra spaces between the commands and arguments and special characters
char *addSpace(const char *userInput)
{
    const char *listSpecialChar = "<>|&;";
    const char *specialCharRep = ">|&";
    long int i = 0, j = 0;
    long int karanLength = strlen(userInput);
    char *newUpdatedCommandWithSpace = malloc(karanLength * 2 + 1);

    while (i < karanLength)
    {
        if (strchr(listSpecialChar, userInput[i]) != NULL && !checkEscapeChar(userInput, i))
        {

            if (strchr(specialCharRep, userInput[i]) != NULL && i + 1 < karanLength && userInput[i] == userInput[i + 1])
            {
                newUpdatedCommandWithSpace[j++] = ' ';
                newUpdatedCommandWithSpace[j++] = userInput[i++];
                newUpdatedCommandWithSpace[j++] = userInput[i];
                newUpdatedCommandWithSpace[j++] = ' ';
                newUpdatedCommandWithSpace[j++] = ' ';
            }
            else
            {
                newUpdatedCommandWithSpace[j++] = ' ';
                newUpdatedCommandWithSpace[j++] = userInput[i];
                newUpdatedCommandWithSpace[j++] = ' ';
            }
        }
        else
        {

            newUpdatedCommandWithSpace[j++] = userInput[i];
        }
        i++;
    }

    newUpdatedCommandWithSpace[j] = '\0';
    return newUpdatedCommandWithSpace;
}

// Function to trim the space from the string
char *trimInputBySpace(char *input)
{
    int j;
    while (isspace(*input))
    {
        input++;
    }
    for (j = strlen(input) - 1; (isspace(input[j])); j--)
        ;
    input[j + 1] = '\0';

    return input;
}

// Function to check the argument number
int checkArgumentCount(char *singleCommand)
{
    char *newCommand = trimInputBySpace(strdup(singleCommand)); // Removing the space from starting and last
    char *newCommandArray = strtok(newCommand, " ");            // Spliting the values by ' '
    int argCount = 0;

    while (newCommandArray != NULL)
    {
        newCommandArray = strtok(NULL, " ");
        argCount++;
    }
    return argCount <= MAX_ARGUMENTS && argCount > 0; // Returning the argCount && count more than 1
}

// Function to check each command arguments
int validateAndCheckArguments(char **command, int tokenCount)
{
    for (int i = 0; i < tokenCount; i++)
    {
        if (i % 2)
            continue;
        int isValid = checkArgumentCount(command[i]); // Checking the arguments for the command
        if (!isValid)
        {
            printf("Only 2 arguments are allowed per command, please check the command again\n");
            printf("command arg1 arg2\n");
            printf("%s\n", command[i]);
            return 0;
        }
    }
    return 1;
}

// Function to count the command and the special characters (| , || , && , ;)
int countCommandAndOperations(char *userInput, char **tokenInInput)
{
    int karanCount = 0;
    int indexNumber = 0;
    int pipeCount = 0;
    int karanSequentialCount = 0;
    int karanConditionalCount = 0;

    int i = 0;
    while (i < strlen(userInput))
    {
        if (strncmp(userInput + i, "||", 2) == 0) // Checking ||
        {
            tokenInInput[karanCount] = malloc((i - indexNumber + 1) * sizeof(char));
            strncpy(tokenInInput[karanCount++], userInput + indexNumber, i - indexNumber); // Copy the substring from the userInput
            tokenInInput[karanCount++] = "||";
            i++;
            indexNumber = i + 2;
            karanConditionalCount++;
        }
        else if (userInput[i] == '|') // Checking |
        {
            tokenInInput[karanCount] = malloc((i - indexNumber + 1) * sizeof(char));
            strncpy(tokenInInput[karanCount++], userInput + indexNumber, i - indexNumber); // Copy the substring from the userInput
            tokenInInput[karanCount++] = "|";
            indexNumber = i + 1;
            pipeCount++;
        }
        else if (userInput[i] == ';') // Checking ;
        {
            tokenInInput[karanCount] = malloc((i - indexNumber + 1) * sizeof(char));
            strncpy(tokenInInput[karanCount++], userInput + indexNumber, i - indexNumber); // Copy the substring from the userInput
            tokenInInput[karanCount++] = ";";
            indexNumber = i + 1;
            karanSequentialCount++;
        }
        else if (strncmp(userInput + i, "&&", 2) == 0) // Checking &&
        {
            tokenInInput[karanCount] = malloc((i - indexNumber + 1) * sizeof(char));
            strncpy(tokenInInput[karanCount++], userInput + indexNumber, i - indexNumber); // Copy the substring from the userInput
            tokenInInput[karanCount++] = "&&";
            i++;
            indexNumber = i + 2;
            karanConditionalCount++;
        }
        i++;
    }
    tokenInInput[karanCount] = malloc((strlen(userInput) - indexNumber + 1) * sizeof(char));
    strncpy(tokenInInput[karanCount++], userInput + indexNumber, strlen(userInput) - indexNumber); // Copy the substring from the userInput
    if (pipeCount > MAX_PIPING)
    {
        printf("Max 4 piping operations are allowed\n");
        printf("command1 | command2 | command3 | command4\n");
        return -9999;
    }
    if (karanSequentialCount > MAX_CONDITIONAL_EXECUTION || karanConditionalCount > MAX_CONDITIONAL_EXECUTION)
    {
        printf("Max 4 conditional operations combinations are allowed\n");
        printf("command1 || command2 && command3 || command4\n");
        return -9999;
    }
    return karanCount;
}

int runIndiCommand(char *currentCommand, int a)
{

    char *updatedCommand = strdup(currentCommand);
    updatedCommand = trimInputBySpace(updatedCommand); // Trim the spaces from the command

    // Checking if the current command is quit and exit the program
    if (strcmp(updatedCommand, "quit") == 0)
    {
        exit(0);
    }

    if (strcmp(updatedCommand, "cd") == 0)
    {
        chdir(getenv("HOME")); // Changing the directory if the command is CD
        return 0;
    }
    else if (strstr(updatedCommand, "cd ") == updatedCommand)
    {
        // Changing the directory if the command is CD directory Name
        char *dir = updatedCommand + 3;
        dir = trimInputBySpace(dir);
        chdir(dir);
        return 0;
    }

    bool isBackground = false;
    if (updatedCommand[strlen(updatedCommand) - 1] == '&')
    {
        // If the last char is & then it is background command
        isBackground = true;
        updatedCommand[strlen(updatedCommand) - 1] = '\0';
        updatedCommand = trimInputBySpace(updatedCommand);
    }

    char *outputFileName = NULL; // For the output file name
    char *inputFileName = NULL;  // For the input file name
    int i = 0;
    int outputMode = 1; // For the output mode
    char *spaceToken;   // Token array for the space
    char *nextToken;    // Token for the next token after space
    char *arr[10];
    spaceToken = strtok_r(updatedCommand, " ", &nextToken);
    while (spaceToken != NULL)
    {
        // Checking for the redirection
        if (strcmp(spaceToken, "<") == 0)
        {
            // If the token is <
            inputFileName = strtok_r(NULL, " ", &nextToken);
        }
        else if (strcmp(spaceToken, ">") == 0)
        {
            // If the token is >
            outputFileName = strtok_r(NULL, " ", &nextToken);
            outputMode = 1;
        }
        else if (strcmp(spaceToken, ">>") == 0)
        {
            // If the token is >>
            outputFileName = strtok_r(NULL, " ", &nextToken);
            outputMode = 2;
        }
        else
            arr[i++] = spaceToken;
        spaceToken = strtok_r(NULL, " ", &nextToken);
    }
    arr[i] = NULL;
    int child = 0;
    if (a)
    {
        child = fork();
    }
    if (child == 0)
    {
        int inputFd, outputFd;
        if (inputFileName)
        {
            inputFd = open(inputFileName, O_RDONLY); // Opening input file in read only mode
            if (inputFd < 0)
            {
                exit(EXIT_FAILURE);
            }
            dup2(inputFd, STDIN_FILENO);
            close(inputFd);
        }
        if (outputFileName)
        {
            if (outputMode == 1)
            {
                outputFd = open(outputFileName, O_WRONLY | O_CREAT | O_TRUNC, 0666); // Opening output file in write only and truncate mode
            }
            else if (outputMode == 2)
            {
                outputFd = open(outputFileName, O_WRONLY | O_CREAT | O_APPEND, 0666); // Opening output file in write only and append mode
            }
            if (outputFd < 0)
            {
                exit(EXIT_FAILURE);
            }
            dup2(outputFd, STDOUT_FILENO);
            close(outputFd);
        }
        if (execvp(arr[0], arr) < 0)
        {
            // If error while executing the command
            exit(EXIT_FAILURE);
        }
    }
    else if (child < 0)
    {
        exit(EXIT_FAILURE);
    }
    else
    {
        if (!isBackground)
        {
            // If the command is not background command
            int status;
            wait(&status);
            return status;
        }
        else
        {
            return 0;
        }
    }
    if (a == 0)
    {
        // If the command is not background command
        if (!isBackground)
        {
            int status;
            wait(&status);
            return status;
        }
        else
        {
            return 0;
        }
    }
    return 0;
}

// Function to execute the pipe commands
void startPipeProcess(char **command, int arguments)
{

    int i = 0;
    int forkChild = fork(); // Creating a new child
    if (forkChild > 0)
    {
        // Parent Process
        char *currentCommand = strdup(command[arguments - 1]);
        currentCommand = trimInputBySpace(currentCommand);
        if (currentCommand[strlen(currentCommand) - 1] != '&')
        {
            waitpid(forkChild, NULL, 0);
        }
        else
        {
            printf("Command running in the background %s %d\n", *command, forkChild);
        }
    }
    else if (forkChild == 0)
    {
        for (i = 0; i < arguments - 1; i += 2)
        {
            int pipeDescriptor[2];
            if (pipe(pipeDescriptor) < 0)
            {
                exit(EXIT_FAILURE);
            }
            if (!fork())
            {
                dup2(pipeDescriptor[1], 1);
                runIndiCommand(command[i], 0); // Calling individual commnand run
            }
            dup2(pipeDescriptor[0], 0);
            close(pipeDescriptor[1]); // closing file descriptor
        }

        runIndiCommand(command[i], 0); // Calling individual commnand run
    }
    else
    {
        exit(EXIT_FAILURE);
    }
}

void checkAndRunCommands(char **tokenInInput, int count)
{
    int previousResult = 1;
    int currentResult = 0;

    int k = 0;
    while (k < count)
    {
        if (strcmp(tokenInInput[k], "|") == 0) // If the current value of the userInput is "|" go the next string
        {
            k++;
        }
        else if (strcmp(tokenInInput[k], ";") == 0) // If the current value of the userInput is ";" go the next string
        {
            k++;
        }
        else if (strcmp(tokenInInput[k], "&&") == 0) // If the current value of the userInput is "&&" go the previous value
        {
            if (previousResult != 0)
            {
                k++;
                while (k < count && strcmp(tokenInInput[k], "||") != 0 &&
                       strcmp(tokenInInput[k], "|") != 0 &&
                       strcmp(tokenInInput[k], ";") != 0)
                {
                    k++;
                }
                if (k > 0 && k < count && strcmp(tokenInInput[k], "|") == 0)
                    k += 2;
            }
            else
            {
                k++;
            }
        }
        else if (strcmp(tokenInInput[k], "||") == 0) // If the current value of the userInput is "||" go the previous value
        {
            if (previousResult == 0)
            {

                k++;
                while (k < count && strcmp(tokenInInput[k], "&&") != 0 &&
                       strcmp(tokenInInput[k], "|") != 0 &&
                       strcmp(tokenInInput[k], ";") != 0)
                {
                    k++;
                }
                if (k > 0 && k < count && strcmp(tokenInInput[k], "|") == 0)
                    k += 2;
            }
            else
            {
                k++;
            }
        }
        else if (k < count - 1 && strcmp(tokenInInput[k + 1], "|") == 0)
        {
            // Combining the command before the PIPE and executing it
            int start = k;
            while (k < count - 1 && strcmp(tokenInInput[k + 1], "|") == 0)
                k += 2;
            startPipeProcess(tokenInInput + start, k - start + 1);
            k++;
        }
        else
        {
            //  Running the standalone command
            currentResult = runIndiCommand(tokenInInput[k], 1);
            previousResult = currentResult;
            k++;
        }
    }
}

int main()
{
    // Running the while loop for the user input
    while (1)
    {
        char *userInput = malloc(MAX_COMMAND_LENGTH * sizeof(char));
        printf("\nmybash$ "); // Printing the mybash$ on screen
        fflush(stdout);

        if (fgets(userInput, MAX_COMMAND_LENGTH, stdin) == NULL)
        {
            perror("Error while taking the user input");
            exit(EXIT_FAILURE);
        }
        userInput[strcspn(userInput, "\n")] = '\0'; // Replacing the next line with null character
        userInput = trimInputBySpace(userInput);    // Removing the space from the input
        if (strlen(userInput) == 0)
        {
            continue;
        }
        char *updatedInputWithSpace = addSpace(userInput);
        char *tokenInInput[150];                                                         // Array to store the commands
        int tokenCount = countCommandAndOperations(updatedInputWithSpace, tokenInInput); // Total number of special char
        if (tokenCount == -9999)
        {
            // If the special characters are more than specified then continue with the next user input
            continue;
        }
        int isValid = validateAndCheckArguments(tokenInInput, tokenCount);
        if (!isValid)
        {
            // If the arguments are more than specified then continue with the next user input
            continue;
        }
        checkAndRunCommands(tokenInInput, tokenCount);
    }
    return 0;
}