#include "utils.h"

void command_noop()
{
    printf("\n200 NOOP called, acknowledged.\n");
}

void command_cwd(char bffer[], char tmp[])
{
    if (tmp[3] != ' ') // no specified parameter
    {
        strcat(bffer, "504 Error:Command not found for given parameter\n");
        return;
    }
    int charLength = strlen(tmp); // length of the parameter
    charLength = charLength - 4;
    char filePath[charLength];
    strncpy(filePath, tmp + 4, charLength);
    filePath[charLength] = '\0'; // null terminate the string
    if (chdir(filePath) != 0)
    {
        strcat(bffer, "425 Error:No file or directory found\n");
        return;
    }
}

void command_pwd(char bffer[])
{
    char cwd[PATH_MAX]; // PATH_MAX is defined in limits.h
    if (getcwd(cwd, sizeof(cwd)) == NULL)
    {
        strcpy(bffer, "425 Error:Can not access current directory\n");
        return;
    }
    else
    {
        strcat(bffer, "250 Current working directory\t:\t");
        strcat(bffer, cwd);
        strcat(bffer, "\n");
    }
}

void command_list(char bffer[])
{
    DIR *dirObj;              // directory object
    struct dirent *directory; // struct dirent is a structure that contains information about a directory entry
    dirObj = opendir(".");    // open current directory
    strcat(bffer, "250 Files/Directories in current directory\n");
    if (dirObj)
    {
        while ((directory = readdir(dirObj)) != NULL) // read directory entries
        {
            strcat(bffer, directory->d_name);
            strcat(bffer, "\n");
        }
    }
}

void command_mkd(char bffer[], char tmp[])
{
    if (tmp[3] != ' ') // no specified parameter
    {
        strcat(bffer, "504 Error:Command not found for given parameter\n");
        return;
    }
    int charLength = strlen(tmp); // get length of the string
    charLength = charLength - 4;  // remove the first 4 characters
    char filePath[charLength];    // create a buffer to store the file path
    int j, k;
    for (j = 0; j < charLength; j++)
    {
        if (tmp[j + 4] == ' ')
        {
            strcat(bffer, "504 Error:Invalid filePath\n");
            return;
        }
        if (tmp[j + 4] == '/' && j != charLength)
        {
            k = j;
        }
        filePath[j] = tmp[j + 4];
    }
    filePath[k] = '\0';
    char dirName[charLength - k];
    strncpy(dirName, tmp + 5 + k, charLength - k);
    if (chdir(filePath) != 0)
    {
        strcat(bffer, "425 Error:No file or directory found for given path\n");
        return;
    }
    command_pwd(bffer); // get the current working directory
    if (mkdir(dirName, 0777) != -1)
    {
        strcat(bffer, "250 Directory created\n");
        command_pwd(bffer); // get the current working directory
    }
    else
    {
        strcat(bffer, "425 Error:Can not create directory\n");
        return;
    }
}

void removeDirectory(const char filePath[], char bffer[])
{
    size_t pathLength;                // length of the file path
    char *fullPath;                   // full path of the file
    DIR *directory;                   // directory object
    struct stat stPath, stEntry;      // struct stat is a structure that contains information about a file
    struct dirent *interation;        // struct dirent is a structure that contains information about a directory entry
    stat(filePath, &stPath);          // get the information about the file
    if (S_ISDIR(stPath.st_mode) == 0) // if the file is not a directory
    {
        strcat(bffer, "425 Error:No file or directory found!\n");
        return;
    }
    if ((directory = opendir(filePath)) == NULL)
    {
        strcat(bffer, "520 Error:Can not open directory\n");
        return;
    }
    pathLength = strlen(filePath);                    // get the length of the file path
    while ((interation = readdir(directory)) != NULL) // iterate through each entry in directory
    {
        if (!strcmp(interation->d_name, ".") || !strcmp(interation->d_name, "..")) // if entry is . or .. skip it
        {
            continue;
        }
        fullPath = calloc(pathLength + strlen(interation->d_name) + 1, sizeof(char)); // allocate memory for the full path
        strcpy(fullPath, filePath);                                                   // copy the file path to the full path
        strcat(fullPath, "/");                                                        // append a slash to the full path
        strcat(fullPath, interation->d_name);                                         // append the entry name to the full path
        stat(fullPath, &stEntry);                                                     // get the information about the entry
        if (S_ISDIR(stEntry.st_mode) != 0)                                            // if entry is directory
        {
            removeDirectory(fullPath, bffer);
            continue;
        }
        if (unlink(fullPath) == 0) // Check unlink status
        {
            strcat(bffer, "250 Requested file action okay, removed.\t\t:\t");
            strcat(bffer, fullPath);
        }
        else
        {
            strcat(bffer, "550 Error:File unavailable.\t:\t");
            strcat(bffer, fullPath);
        }
    }
    if (rmdir(filePath) == 0) // Check rmdir status
    {
        strcat(bffer, "250 Directory removed\t:\t");
        strcat(bffer, filePath);
    }
    else
    {
        strcat(bffer, "520 Error:Can not remove given directory\t:\t");
        strcat(bffer, fullPath);
    }
    closedir(directory); // close the directory
}

void command_rmd(char bffer[], char tmp[])
{
    if (tmp[3] != ' ') // no specified parameter
    {
        strcat(bffer, "504 Error:No Command found for empty parameter.\n");
        return;
    }
    int charLength = strlen(tmp);           // get length of the string
    charLength = charLength - 4;            // remove the first 4 characters
    char filePath[charLength];              // create a buffer to store the file path
    strncpy(filePath, tmp + 4, charLength); // copy the file path to the buffer
    filePath[charLength] = '\0';            // append null character to the file path
    removeDirectory(filePath, bffer);       // remove the directory
}

void command_dele(char bffer[], char tmp[])
{
    if (tmp[3] != ' ')
    {
        strcat(bffer, "504 Error:No Command found for empty parameter.\n");
        return;
    }
    int charLength = strlen(tmp);           // get length of the string
    charLength = charLength - 4;            // remove the first 4 characters
    char filePath[charLength];              // create a buffer to store the file path
    strncpy(filePath, tmp + 4, charLength); // copy the file path to the buffer
    filePath[charLength] = '\0';            // append null character to the file path
    if (unlink(filePath) == 0)
    {
        strcat(bffer, "250 Requested file action okay, removed.\t\t:\t");
        strcat(bffer, filePath);
    }
    else
    {
        strcat(bffer, "550 Error:Given file is unavailable.\t:\t");
        strcat(bffer, filePath);
    }
}

char fileN[50];
void command_stor(char bffer[], char tmp[])
{
    int charLength = strlen(tmp) - 5;
    strncpy(fileN, tmp + 5, charLength);
    fileN[charLength] = '\0';    // append null character to the file path
    FILE *filePtr;               // File pointer for file creation
    filePtr = fopen(fileN, "w"); // Open file for writing
    if (filePtr == NULL)         // Error Check
    {
        strcat(bffer, fileN);
        strcat(bffer, "520 \t:\t does not exist\n");
        return;
    }
    strcpy(bffer, "225 Data connection opened and no transfer in progress.\n"); // Reply to client
    fclose(filePtr);                                                            // Close the file
}

void command_retr(char bffer[], char tmp[])
{
    int charLength = strlen(tmp) - 5;
    strncpy(fileN, tmp + 5, charLength);
    fileN[charLength] = '\0';
    FILE *filePtr;               // File pointer for file reading
    filePtr = fopen(fileN, "r"); // Open file for reading
    if (filePtr == NULL)         // Error Check
    {
        strcat(bffer, fileN);
        strcat(bffer, "520 Error\t:\t does not exist\n");
        return;
    }
    strcpy(bffer, "226 Data connection opened and no transfer in progress.\n"); // Reply to client
    fclose(filePtr);                                                            // Close the file
}

int find(char *str1) // To parse the command which will be passed from client
{
    // Match the command with the list of commands and return the index of the command in the list
    if (str1[0] == 'U' && str1[1] == 'S' && str1[2] == 'E' && str1[3] == 'R')
    {
        return 1;
    }
    else if (str1[0] == 'P' && str1[1] == 'W' && str1[2] == 'D')
    {
        return 2;
    }
    else if (str1[0] == 'M' && str1[1] == 'K' && str1[2] == 'D')
    {
        return 3;
    }
    else if (str1[0] == 'C' && str1[1] == 'W' && str1[2] == 'D')
    {
        return 4;
    }
    else if (str1[0] == 'R')
    {
        if (str1[1] == 'E' && str1[2] == 'T' && str1[3] == 'R')
        {
            return 7;
        }
        else if (str1[1] == 'M' && str1[2] == 'D')
        {
            return 5;
        }
        else
        {
            return 0;
        }
    }
    else if (str1[0] == 'S' && str1[1] == 'T' && str1[2] == 'O' && str1[3] == 'R')
    {
        return 8;
    }
    else if (str1[0] == 'L' && str1[1] == 'I' && str1[2] == 'S' && str1[3] == 'T')
    {
        return 9;
    }
    else if (str1[0] == 'A' && str1[1] == 'B' && str1[2] == 'O' && str1[3] == 'R')
    {
        return 10;
    }
    else if (str1[0] == 'N' && str1[1] == 'O' && str1[2] == 'O' && str1[3] == 'P')
    {
        return 11;
    }
    else if (str1[0] == 'D' && str1[1] == 'E' && str1[2] == 'L' && str1[3] == 'E')
    {
        return 12;
    }
    else if (str1[0] == 'Q' && str1[1] == 'U' && str1[2] == 'I' && str1[3] == 'T')
    {
        return 13;
    }
    return 0;
}

int authFlag = 0;
void commandCaller(int commandValue, char bffer[])
{
    char tmp[500] = "";
    strcpy(tmp, bffer);
    strcpy(bffer, "\n200 Command is okay.\n");
    if (commandValue == 1)
    {
        // Implement Authentication Flags according to conditions
        // 1 -> Correct Username
        // 2 -> Incorrect Password
        authFlag = 2;
        printf("230 User logged in, proceed.\n");
    }
    else if (commandValue == 3)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }

        command_mkd(bffer, tmp);
    }
    else if (commandValue == 4)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        command_pwd(bffer);
        command_cwd(bffer, tmp);
        command_pwd(bffer);
    }
    else if (commandValue == 5)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        command_rmd(bffer, tmp);
    }
    else if (commandValue == 2)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        if (tmp[3] == ' ')
        {
            strcat(bffer, "504 No Command found for empty parameter.\n");
            return;
        }
        if (strlen(tmp) > 3)
        {
            strcpy(bffer, "503 Error:Bad sequence of commands.\n");
            return;
        }
        command_pwd(bffer);
    }
    else if (commandValue == 7)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        if (tmp[4] != ' ')
        {
            strcpy(bffer, "503 Error:Bad sequence of commands.\n");
            return;
        }
        command_retr(bffer, tmp);
    }
    else if (commandValue == 8)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        if (tmp[4] != ' ')
        {
            strcpy(bffer, "503 Error:Bad sequence of commands.\n");
            return;
        }
        command_stor(bffer, tmp);
    }
    else if (commandValue == 9)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        if (tmp[4] == ' ')
        {
            strcat(bffer, "504 Error:No Command found for empty parameter.\n");
            return;
        }
        if (strlen(tmp) > 4)
        {
            strcpy(bffer, "503 Error:Bad sequence of commands.\n");
            return;
        }
        command_list(bffer);
    }
    else if (commandValue == 10)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 Error:User not logged in.\n");
            return;
        }
        if (tmp[4] == ' ')
        {
            strcat(bffer, "504 Error:No Command found for empty parameter.\n");
            return;
        }
        if (strlen(tmp) > 4)
        {
            strcat(bffer, "503 Error:Bad sequence of commands.\n");
            return;
        }
        strcat(bffer, "550 Requested action aborted.\n");
    }
    else if (commandValue == 11)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 User not logged in.\n");
            return;
        }
        command_noop();
    }
    else if (commandValue == 12)
    {
        if (authFlag == 0 || authFlag == 1)
        {
            strcat(bffer, "530 User not logged in.\n");
            return;
        }
        command_dele(bffer, tmp);
    }
    else if (commandValue == 13)
    {
        strcpy(bffer, "221 Service closing control connection. Logged out if appropriate.\n");
        return;
    }
}

void mainloop(int output)
{
    char bffer[10000] = "";
    char tmp[10000] = " ";
    while (1)
    {
        if (strcmp(tmp, "225 Data connection open; no transfer in progress.\n") == 0)
        {
            FILE *filePtr;
            filePtr = fopen(fileN, "w");
            if (filePtr == NULL)
            {
                printf("520 Error:Can not open file %s \n", fileN);
            }
            else
            {
                bzero(bffer, sizeof(bffer));
                while (1)
                {
                    read(output, bffer, sizeof(bffer));
                    fputs(bffer, filePtr);
                    if (strlen(bffer) - 1 != sizeof(bffer))
                    {
                        break;
                    }
                    bzero(bffer, sizeof(bffer));
                }
                printf("\n200 File has received\n");
                fclose(filePtr);
                bzero(tmp, sizeof(tmp));
            }
        }
        bzero(bffer, sizeof(bffer));
        read(output, bffer, sizeof(bffer));
        printf("\n200 Message Received:\t%s\n", bffer);

        int commandValue = find(bffer);
        if (commandValue == 0)
        {
            strcpy(bffer, "502 Error:Command is not created\n");
        }
        else
        {
            commandCaller(commandValue, bffer);
        }
        write(output, bffer, strlen(bffer));
        if (strcmp("\n502 Connection Termination\n", bffer) == 0)
        {
            printf("%s", bffer);
            exit(0);
        }
        printf("%s", bffer);
        strcpy(tmp, bffer);

        if (commandValue == 13 && authFlag == 2)
        {
            if (strcmp(tmp, "221 Service closing control connection. Logged out if appropriate.\n") == 0)
            {
                authFlag = 0;
                printf("\n200 User Logged Out Successfully\n");
                break;
            }
            exit(0);
        }

        if (strcmp(tmp, "\n226 Data connection is now open but there is no transfer which is currently in progress.\n") == 0)
        {
            printf("\n200 File is getting send\n");
            sleep(3);
            FILE *filePtr;
            filePtr = fopen(fileN, "r");
            if (filePtr == NULL)
            {
                printf("520 Error:Can not open file: %s \n", fileN);
            }
            else
            {
                char ch;
                bzero(bffer, sizeof(bffer));
                int iterate = 0;
                while (1)
                {
                    while (((ch = fgetc(filePtr)) != EOF) && (strlen(bffer) - 1 != sizeof(bffer)))
                    {
                        bffer[iterate] = ch;
                        iterate++;
                    }
                    bffer[iterate] = '\0';
                    if (ch == EOF)
                    {
                        write(output, bffer, strlen(bffer));
                        break;
                    }
                    if (strlen(bffer) == sizeof(bffer))
                    {
                        write(output, bffer, strlen(bffer));
                        bzero(bffer, sizeof(bffer));
                        iterate = 0;
                    }
                }
                printf("\n250 File Sent!\n");
                fclose(filePtr);
            }
        }
    }
}
