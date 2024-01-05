#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <sys/wait.h>

// Variable to store the option
char *option;

// Function to check if the given targetId is part of the parent ID
bool isDescendant(int targetPID, int parentPID)
{
    char procPath[128];
    char statusPath[128];
    int currentPID = targetPID;

    while (currentPID != 1)
    {
        snprintf(procPath, sizeof(procPath), "/proc/%d/status", currentPID); // Concatinating the values in the procPath
        FILE *statusFile = fopen(procPath, "r");

        if (statusFile == NULL)
        {
            return false;
        }

        int ppid = -1;
        char line[256];
        char name[64];

        while (fgets(line, sizeof(line), statusFile) != NULL)
        {
            if (sscanf(line, "PPid: %d", &ppid) == 1)
            {
                // Extract the parent process ID (PPid)
                if (ppid == parentPID)
                {
                    // Found the parent process
                    fclose(statusFile);
                    return true;
                }
            }
        }

        fclose(statusFile);

        if (ppid <= 1)
        {
            return false;
        }
        currentPID = ppid;
    }

    return false;
}

// Function of check if the given string is a number or not
bool isNumber(const char *str)
{
    // Check if the string is empty
    if (*str == '\0')
    {
        return false;
    }

    // Check each character in the string
    for (int i = 0; str[i] != '\0'; i++)
    {
        if (!isdigit(str[i]))
        {
            return false;
        }
    }

    return true;
}

// Function to get the parent id of the given process id
int fetchParentProcessID(int processId)
{
    char procPathStore[100];
    snprintf(procPathStore, sizeof(procPathStore), "/proc/%d/status", processId); // Concatinating the values in the procPathStore

    // opening the file in Read mode
    FILE *fd = fopen(procPathStore, "r");
    if (fd == NULL)
    {
        return 1;
    }

    char arr[128];
    int ppid = 0;

    // storing each line read from the process status file.
    while (fgets(arr, sizeof(arr), fd) != NULL)
    {
        if (strncmp(arr, "PPid:", 5) == 0)
        {
            sscanf(arr + 5, "%d", &ppid);
            break;
        }
    }
    fclose(fd);
    return ppid;
}

// Function of find the direct descendants of the given child id
void findDirectDescendants(int childId)
{
    char procPathStore[256];
    snprintf(procPathStore, sizeof(procPathStore), "pstree -p %d", childId); // Concatinating the values in the procPathStore
    FILE *statFile = popen(procPathStore, "r");
    if (statFile != NULL)
    {
        char processLines[256];
        char *token;
        while (fgets(processLines, sizeof(processLines), statFile) != NULL)
        {
            char *token = strtok(processLines, "-"); // spliting the string by '-'
            while (token != NULL)
            {
                // Searching for the "programname(processid)" pattern
                char *open = strchr(token, '(');
                char *close = strrchr(token, ')');

                if (open != NULL && close != NULL && open < close)
                {
                    int childPid = atoi(open + 1);
                    if (childPid != childId && fetchParentProcessID(childPid) == childId)
                    {
                        printf("%d\n", childPid);
                    }
                }
                token = strtok(NULL, "-");
            }
        }
    }
}

// Function of find the non direct descendants of the given child id
void findNonDirectDescendants(int parentId, int childId)
{
    char procPathStore[256];
    snprintf(procPathStore, sizeof(procPathStore), "pstree -p %d", childId);
    FILE *statFile = popen(procPathStore, "r");
    if (statFile != NULL)
    {
        char processLines[256];
        char *token;
        while (fgets(processLines, sizeof(processLines), statFile) != NULL)
        {
            char *token = strtok(processLines, "-"); // spliting the string by '-'
            while (token != NULL)
            {
                // Searching for the "programname(processid)" pattern
                char *open = strchr(token, '(');
                char *close = strrchr(token, ')');

                if (open != NULL && close != NULL && open < close)
                {
                    int childPid = atoi(open + 1);
                    if (childPid != childId)
                    {
                        findDirectDescendants(childPid);
                    }
                }
                token = strtok(NULL, "-");
            }
        }
        fclose(statFile);
    }
}

// Function of find the siblings of the given child id
void findSibling(int child)
{
    // Finding the parent of the given id
    int parentOfChild = fetchParentProcessID(child);
    char procPathStore[256];
    snprintf(procPathStore, sizeof(procPathStore), "pstree -p %d", parentOfChild); // Concatinating the values in the procPathStore
    FILE *statFile = popen(procPathStore, "r");
    if (statFile != NULL)
    {
        char processLines[256];
        char *token;
        while (fgets(processLines, sizeof(processLines), statFile) != NULL)
        {
            char *token = strtok(processLines, "-");
            while (token != NULL)
            {
                // Searching for the "programname(processid)" pattern
                char *open = strchr(token, '(');
                char *close = strrchr(token, ')');

                if (open != NULL && close != NULL && open < close)
                {
                    int childPid = atoi(open + 1);
                    if (childPid != parentOfChild && fetchParentProcessID(childPid) == parentOfChild && childPid != child)
                    {
                        printf("%d\n", childPid);
                    }
                }
                token = strtok(NULL, "-");
            }
        }
    }
    fclose(statFile);
}

// Function to find the defunct sibling of the given id
void findDefunctSibling(int child)
{
    // Finding the parent of the given child id
    int parentOfChild = fetchParentProcessID(child);
    char procPathStore[256];
    snprintf(procPathStore, sizeof(procPathStore), "pstree -p %d", parentOfChild); // Concatinating the values in the procPathStore
    FILE *statFile = popen(procPathStore, "r");
    if (statFile != NULL)
    {
        char processLines[256];
        char *token;
        while (fgets(processLines, sizeof(processLines), statFile) != NULL)
        {
            char *token = strtok(processLines, "-");
            while (token != NULL)
            {
                // Searching for the "programname(processid)" pattern
                char *open = strchr(token, '(');
                char *close = strrchr(token, ')');

                if (open != NULL && close != NULL && open < close)
                {
                    int child_pid = atoi(open + 1);
                    if (child_pid != parentOfChild && fetchParentProcessID(child_pid) == parentOfChild && child_pid != child)
                    {
                        char procPath[256];
                        char statusPath[256];
                        char line[256];

                        // Building the path to the process status file in /proc
                        snprintf(procPath, sizeof(procPath), "/proc/%d", child_pid);            // Concatinating the values in the procPath
                        snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", child_pid); // Concatinating the values in the statusPath
                        // Check if the /proc/<PID> directory exists
                        if (access(procPath, F_OK) == 0)
                        {
                            FILE *statusFile = fopen(statusPath, "r");
                            if (statusFile)
                            {
                                while (fgets(line, sizeof(line), statusFile))
                                {
                                    if (strstr(line, "State:") && strstr(line, "zombie"))
                                    {
                                        printf("%d\n", child_pid); // Process is a defunct process
                                    }
                                }
                            }
                            fclose(statusFile);
                        }
                    }
                }
                token = strtok(NULL, "-");
            }
        }
    }
    fclose(statFile);
}

// Function of find the grand child of the given id
void findGrandChild(int child)
{
    char command[100];
    // Command to get the child process of the given process id
    snprintf(command, sizeof(command), "ps -o pid,ppid -e | awk '$2 == %d { print $1 }'", child);

    FILE *fp = popen(command, "r");
    if (fp == NULL)
    {
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp) != NULL)
    {
        // Finding the child of current process
        int child_pid = atoi(line);
        char command[100];
        // Command to get the child process of the given child id
        snprintf(command, sizeof(command), "ps -o pid,ppid -e | awk '$2 == %d { print $1 }'", child_pid);

        FILE *fp = popen(command, "r");
        if (fp == NULL)
        {
            return;
        }

        while (fgets(line, sizeof(line), fp) != NULL)
        {
            // Finding the grandchild now
            int grandChildPid = atoi(line);
            printf("%d\n", grandChildPid);
        }

        pclose(fp);
    }
    pclose(fp);
}

// Function to find the if the given id is defunct process or not
void findDefunct(int child)
{
    char procPath[256];
    char statusPath[256];
    char line[256];
    bool check;

    // Build the path to the process status file in /proc
    snprintf(procPath, sizeof(procPath), "/proc/%d", child);
    snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", child);

    // Checking if the /proc/<PID> directory exists
    if (access(procPath, F_OK) == 0)
    {
        FILE *statusFile = fopen(statusPath, "r");
        if (statusFile)
        {
            while (fgets(line, sizeof(line), statusFile))
            {
                if (strstr(line, "State:") && strstr(line, "zombie"))
                {
                    check = true;
                    printf("DEFUNCT\n"); // Process is a zombie
                }
            }
        }
        fclose(statusFile);
    }
    if (!check)
    {
        printf("NOT DEFUNCT\n");
    }
}

// Function of find the non direct descendants defunct processes
void findDirectDescendantsDefunct(int child)
{
    char procPathStore[256];
    snprintf(procPathStore, sizeof(procPathStore), "pstree -p %d", child); // Concatinating the values in the procPathStore
    FILE *statFile = popen(procPathStore, "r");
    if (statFile != NULL)
    {
        char processLines[256];
        char *token;
        while (fgets(processLines, sizeof(processLines), statFile) != NULL)
        {
            char *token = strtok(processLines, "-"); // spliting the string by '-'
            while (token != NULL)
            {
                // Searching for the "programname(processid)" pattern
                char *open = strchr(token, '(');
                char *close = strrchr(token, ')');
                if (open != NULL && close != NULL && open < close)
                {
                    int childPid = atoi(open + 1);
                    if (childPid != child && fetchParentProcessID(childPid) == child)
                    {
                        char procPath[256];
                        char statusPath[256];
                        char line[256];

                        // Build the path to the process status file in /proc
                        snprintf(procPath, sizeof(procPath), "/proc/%d", childPid);
                        snprintf(statusPath, sizeof(statusPath), "/proc/%d/status", childPid);

                        // Check if the /proc/<PID> directory exists
                        if (access(procPath, F_OK) == 0)
                        {
                            FILE *statusFile = fopen(statusPath, "r");
                            if (statusFile)
                            {
                                while (fgets(line, sizeof(line), statusFile))
                                {
                                    if (strstr(line, "State:") && strstr(line, "zombie"))
                                    {
                                        printf("%d\n", childPid); // Process is a defunct process
                                    }
                                }
                            }
                            fclose(statusFile);
                        }
                    }
                }
                token = strtok(NULL, "-");
            }
        }
    }
}

// Function to perform the action according the given option
void performOperation(int rootProcess, int child)
{
    // Checking if the root process exists of not
    char rootP[256];
    sprintf(rootP, "/proc/%d", rootProcess);
    DIR *rootPath = opendir(rootP);
    if (rootPath == NULL)
    {
        return;
    }
    closedir(rootPath);

    // Checking if the child process exists of not
    char childProces[256];
    sprintf(childProces, "/proc/%d", child);
    DIR *childPath = opendir(childProces);
    if (childPath == NULL)
    {
        return;
    }
    closedir(childPath);

    // Printing the value of process id and parent process id
    printf("%d %d\n", child, fetchParentProcessID(child));

    // Checking the option value and performing the task
    if (strcmp(option, "-id") == 0)
    {
        findDirectDescendants(child);
    }
    else if (strcmp(option, "-dn") == 0)
    {
        findNonDirectDescendants(rootProcess, child);
    }
    else if (strcmp(option, "-gp") == 0)
    {
        findGrandChild(child);
    }
    else if (strcmp(option, "-lp") == 0)
    {
        findSibling(child);
    }
    else if (strcmp(option, "-sz") == 0)
    {
        findDefunctSibling(child);
    }
    else if (strcmp(option, "-zz") == 0)
    {
        findDefunct(child);
    }
    else if (strcmp(option, "-zc") == 0)
    {
        findDirectDescendantsDefunct(child);
    }
}

// Function to check the arguments length and values
int checkArguments(int argc, char *argv[])
{
    // Checking the arguments length
    if (argc < 2)
    {
        printf("Usage: prctree [root_process] [process_id1] [process_id2]… [process_id(n)] [OPTION]\n");
        return 0;
    }
    char *tempAction = argv[argc - 1];
    int length;
    if (isNumber((tempAction)))
    {
        if (argc > 7)
        {
            printf("Length of the process IDs cannot be more than 6\n");
            return 0;
        }
    }
    else
    {
        if (argc > 8)
        {
            printf("Length of the process IDs cannot be more than 6\n");
            return 0;
        }
    }
    if (strcmp(tempAction, "-dn") == 0 || strcmp(tempAction, "-sz") == 0 || strcmp(tempAction, "-gp") == 0 || strcmp(tempAction, "-id") == 0 || strcmp(tempAction, "-lp") == 0 || strcmp(tempAction, "-zz") == 0 || strcmp(tempAction, "-zc") == 0 || strcmp(tempAction, "-zx") == 0)
    {
        option = tempAction;
    }
    else if (isNumber(tempAction) == 1)
    {
        option = "";
    }
    else
    {
        printf("\nPlease provide the correct option\n");
        printf("\nAvailable options are : -dn, -id, -lp, -sz, -gp, -zz, -zc, -zx\n");
        return 0;
    }
    return 1;
}

int main(int argc, char *argv[])
{
    // Validating the arguments
    int isValid = checkArguments(argc, argv);
    if (!isValid)
    {
        return 0;
    }
    int rootProcess = atoi(argv[1]);
    int processID1 = atoi(argv[2]);
    int childPids[5];
    int j = 0;
    bool isZx = false;

    // If the option is not 'zx
    if (strcmp(option, "-zx") != 0)
    {
        if (isDescendant(processID1, rootProcess))
        {
            performOperation(rootProcess, processID1);
        }
    }
    else
    {
        isZx = true;
        for (int i = 2; i < argc - 1 && i < 7; i++)
        {
            if (isNumber((argv[i])) && isDescendant(atoi(argv[i]), rootProcess))
            {
                // Checking if the root process exists of not
                char rootP[256];
                sprintf(rootP, "/proc/%d", rootProcess);
                DIR *rootPath = opendir(rootP);
                if (rootPath == NULL)
                {
                    continue;
                }
                closedir(rootPath);

                // Checking if the child process exists of not
                char childProces[256];
                sprintf(childProces, "/proc/%d", atoi(argv[i]));
                DIR *childPath = opendir(childProces);
                if (childPath == NULL)
                {
                    continue;
                }
                closedir(childPath);

                printf("\n%d %d\n", atoi(argv[i]), fetchParentProcessID(atoi(argv[i])));
                findDirectDescendantsDefunct(atoi(argv[i]));
            }
            j++;
        }
    }

    j = 0;
    if (!isZx)
    {
        for (int i = 3; i < argc && i < 7; i++)
        {
            childPids[j] = atoi(argv[i]);
            if (isNumber((argv[i])) && isDescendant(childPids[j], rootProcess))
            {
                printf("\n%d %d\n", childPids[j], fetchParentProcessID(childPids[j]));
            }
            j++;
        }
    }
    return 0;
}