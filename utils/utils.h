#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <limits.h>
#include <unistd.h>

void listFunction(char buffer[]); // list function

void pwdFunction(char buffer[]); // pwd function

void cwdFunction(char buffer[], char temp[]); // cwd function

void mkdFunction(char buffer[], char temp[]); // mkd function

void removeDirectory(const char path[], char buffer[]); // remove directory

void rmdFunction(char buffer[], char temp[]); // rmd function

void storFunction(char buffer[], char temp[]); // stor function

void retrFunction(char buffer[], char temp[]); // retr function

void callFunction(int returnValue, char buffer[]); // call function

void mainloop(int acceptOutput);

#endif
