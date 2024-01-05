#define _GNU_SOURCE
#define _XOPEN_SOURCE 500 // For nftw
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ftw.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>

#define MAX_PATH_LENGTH 1024

char *strdup(const char *s);
int copyMoveFiles(const char *file, int isMove, const char *extensionList[], const char *fileName);
int checkFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf);

// Global variables for callback function
char *destinationDir;
int isMove;
const char *extensionList[50] = {NULL};
char sourceAbsolute[MAX_PATH_LENGTH];
char *directoriesToRemove[1000];
int dirCount = 0;
const char *sourceFolderName;

// Function to check if the dir is empty or not
int isDirectoryEmpty(const char *path)
{
    struct dirent *entry;
    DIR *dir = opendir(path);

    if (dir == NULL)
    {
        return 0;
    }

    while ((entry = readdir(dir)))
    {
        if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name, "..DS_Store") == 0)
        {
            printf("\nEntry : %s %s %d", path, entry->d_name, strcmp(entry->d_name, "."));
            closedir(dir);
            return 0; // Directory is not empty
        }
    }

    closedir(dir);
    return 1; // Directory is empty
}

// Function to check if a file has a specified extension
int checkExtension(const char *filename, const char *extension)
{
    const char *dot = strrchr(filename, '.');
    if (dot && strcmp(dot + 1, extension) == 0)
    {
        return 1;
    }
    return 0;
}

// Function to copy or move files and directories
int copyMoveFiles(const char *source, int isMove, const char *extensionList[], const char *fileName)
{
    // printf("Source : %s %s ", source, fileName);
    char *actionType;
    // Assigning the value for printing
    if (isMove)
    {
        actionType = "Moved";
    }
    else
    {
        actionType = "Copied";
    }

    struct stat st;
    // Checking the source file details
    if (lstat(source, &st) == -1)
    {
        perror("\nError while getting the information about the Source Dir ");
        return -1;
    }

    if (S_ISREG(st.st_mode))
    {
        long int len = strlen(destinationDir) + strlen(fileName) + 2;
        char *newFilePath = (char *)malloc(len * sizeof(char));
        if (newFilePath == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        strcat(newFilePath, destinationDir);
        strcat(newFilePath, "/");
        strcat(newFilePath, fileName);
        int sourceFileDescriptor, destinationFileDescriptor, buffer, err;
        unsigned char fileValues[9999];
        sourceFileDescriptor = open(source, O_RDONLY);
        destinationFileDescriptor = open(newFilePath, O_CREAT | O_WRONLY, 0777);

        while ((err = read(sourceFileDescriptor, fileValues, 9999)) > 0)
        {
            buffer = err;

            if (write(destinationFileDescriptor, fileValues, buffer) == -1)
            {
                perror("\nError while tranfer ");
                exit(1);
            }
        }

        if (err == -1)
        {
            printf("Error reading file.\n");
            exit(1);
        }
        printf("\nFile %s - %s\n", actionType, newFilePath);
        close(sourceFileDescriptor);
        close(destinationFileDescriptor);
        // Removing the files if the option is -mv
        if (isMove)
        {
            if (unlink(source) == -1)
            {
                perror("Error while removing the file ");
                return -1;
            }
        }
        free(newFilePath);
    }
    else
    {
        char *oldDestination = (char *)malloc(strlen(destinationDir) * sizeof(char));
        char *oldSource = (char *)malloc(strlen(source) * sizeof(char));
        strcpy(oldSource, source);
        strcpy(oldDestination, destinationDir);

        // Creating the new sub folder
        long int len = strlen(destinationDir) + strlen(fileName) + 2;
        char *newFolderPath = (char *)malloc(len * sizeof(char));
        if (newFolderPath == NULL)
        {
            printf("Memory allocation failed\n");
            return 1;
        }
        // printf("\nFile %s - %s\n", actionType, source);
        strcat(newFolderPath, destinationDir);
        strcat(newFolderPath, "/");
        strcat(newFolderPath, fileName);

        struct stat st = {0};
        // Creating a new directory
        if (stat(newFolderPath, &st) == -1)
        {
            mkdir(newFolderPath, 0777);
        }

        // destinationDir = newFolderPath;
        if (realpath(source, sourceAbsolute) == NULL)
        {
            return 1;
        }
        if (nftw(source, checkFile, 10000, FTW_PHYS) == -1)
        {
            perror("Error while travering the Source sub folders ");
            return 1;
        }
        free(newFolderPath);
        if (realpath(oldSource, sourceAbsolute) == NULL)
        {
            return 1;
        }
        destinationDir = oldDestination;
    }

    return 0;
}

// Function to traverse using nftw()
int checkFile(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    const char *fileName = fpath + ftwbuf->base;
    if (typeflag == FTW_D)
    {
        // Determine if it's the main folder
        if (ftwbuf->level == 0)
        {
            long int len = strlen(destinationDir) + strlen(fileName) + 2;
            char *result = (char *)malloc(len * sizeof(char));
            if (result == NULL)
            {
                printf("Memory allocation failed\n");
                return 1;
            }
            struct stat stNew = {0};
            strcat(result, destinationDir);
            strcat(result, "/");
            strcat(result, fileName);
            if (stat(result, &stNew) == -1)
            {
                mkdir(result, 0777);
            }
            destinationDir = result;
            return 0;
        }
        if (isMove && !extensionList[0])
        {
            int check = rmdir(fpath);
        }
    }
    // Extension list provided, check if the file has an allowed extension
    int i = 0;
    int found = 0;
    if (extensionList[0])
    {
        while (extensionList[i])
        {
            if (checkExtension(fileName, extensionList[i]))
            {
                found = 1;
                break;
            }
            i++;
        }
    }

    struct stat st1;
    if (lstat(fpath, &st1) == -1)
    {
        return -1;
    }

    // Ignoring some files
    if (fileName[0] == '.' || ((!found && extensionList[0]) && S_ISREG(st1.st_mode)) || (ftwbuf->level > 1))
    {
        return 0;
    }

    if (copyMoveFiles(fpath, isMove, extensionList, fileName) == -1)
    {
        perror("\nError while copying/moving the files ");
        return -1; // Error, stop traversal
    }
    return 0;
}

// Function to check the arguments
int checkArguments(int argc, char *argv[], const char *sourceDir, int *isCopy)
{
    if (argc < 3)
    {
        printf("Usage: %s [source_dir] [destination_dir] [options] [extension1 extension2 ...]\n", argv[0]);
        return 0;
    }
    sourceDir = argv[1];
    destinationDir = argv[2];
    *isCopy = 0;
    isMove = 0;

    for (int i = 3; i < argc; i++)
    {
        if (strcmp(argv[i], "-cp") == 0)
        {
            *isCopy = 1;
        }
        else if (strcmp(argv[i], "-mv") == 0)
        {
            isMove = 1;
        }
        else if (i > 3)
        {
            extensionList[i - 4] = argv[i];
        }
    }

    // If correct option is not provided then show the error
    if (!*isCopy && !isMove)
    {
        printf("Please specify either -cp or -mv option.\n");
        return 0;
    }

    // Checking if the arguments are more than 10 then show the error
    if (argc > 10)
    {
        printf("Too many extensions specified. Maximum is 6.\n");
        return 0;
    }

    // Getting the home dir path
    if (chdir(getenv("HOME")) == -1)
    {
        perror("chdir");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    printf("\n\n******************PROCESS STARTED******************\n\n");
    const char *sourceDir = argv[1];
    int isCopy = 0;

    // Validating the arguments
    int isValid = checkArguments(argc, argv, sourceDir, &isCopy);
    if (!isValid)
    {
        return 0;
    }

    // Get absolute paths
    if (realpath(sourceDir, sourceAbsolute) == NULL)
    {
        perror("Please provide the correct Source directory ");
        return 1;
    }

    struct stat st = {0};
    // Creating a new directory if not present
    if (stat(destinationDir, &st) == -1)
    {
        mkdir(destinationDir, 0777);
    }

    // Traversing through the folder
    if (nftw(sourceAbsolute, checkFile, 10000, FTW_PHYS) == -1)
    {
        perror("Error while travering the Source folder ");
        return 1;
    }
    printf("\n\n******************PROCESS COMPLETED******************\n\n");
    return 0;
}