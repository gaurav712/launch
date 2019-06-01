/*
 * An alternative to xdg-open.
 * Might be redundant to you but I have my reasons to write it.
 *
 * Copyright (c) 2019 Gaurav Kumar Yadav <gaurav712@protonmail.com>
 * for license and copyright information, see the LICENSE file distributed with this source
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#include "config.h"

#define EXT_MAX     10 /* Max length for file extentions */
#define PROG_MAX    20 /* Max length for program name */

/* Show usage instructions(it's nothing actually) */
static void showHelp(void);
/* Extract file extentions from their names */
static void getExtention(char *fileName, char *ext);
/* Fetches preferred program for a file-type */
static void getProg(char *ext, char *prog);
/* Extract program name from it's path */
static void getProgName(char *prog, char *progName);

int main(int argc, char *argv[]) {

    pid_t childPID;
    int status;
    char fileExt[EXT_MAX], prog[256], progName[PROG_MAX];

    if(argc != 2) {
        showHelp();
        exit(1);
    }

    /* Get file extention and decide what program to launch it with */
    getExtention(argv[1], fileExt);
    getProg(fileExt, prog);
    if(!(strcmp(prog, "ERROR"))) {
        perror("No program found for specified file!");
        exit(1);
    }

    /* Execute prog with argv[1] */
    if((childPID = fork()) < 0) {
        perror("fork() failed!");
        exit(1);
    }

    if(!childPID) {

        /* This is the child */

        getProgName(prog, progName);

        /* Uncomment if you don't want garbage on the terminal */
        /*int fd = open("/dev/null", O_WRONLY);
        dup2(fd, 1);
        dup2(fd, 2);
        close(fd);*/

        execl(prog, progName, argv[1], NULL);
        exit(0);
    } else {

        /* This is the parent */

        while (waitpid(childPID, &status, 0) == -1);

        if(WIFEXITED(status)) {
            printf("Child exited normally.\n");
        } else {
            perror("Child exited with an error!");
            fprintf(stderr, "exited with status %d!\n", status);
            exit(1);
        }
    }

    return 0;
}

void
showHelp(void) {

    fprintf(stderr, "\nUSAGE:\n\tlaunch [filename]\n\n");
}

void
getExtention(char *fileName, char *ext) {

    short count = 0, temp = 0;

    while(fileName[count] != '\0')
        count++;

    while(fileName[count] != '.') {
        count--;
        /* If you can't find a '.' and count gets down to 0, it means the file has no extention */
        if(!count) {
            strcpy(ext, "UNKNOWN");
            return;
        }
    }

    /* To make sure the extention doesn't hold the '.' with it */
    count++;

    while(fileName[count] != '\0') {
        ext[temp] = fileName[count];
        temp++;
        count++;
    }

    ext[temp] = '\0';
}

void
getProg(char *ext, char *prog) {

    short count = 0;

    while(strcmp(fileAssociations[count], "ENDALL")) {

        /* Get program name */
        strcpy(prog, fileAssociations[count]);
        count++;

        /* Check for extention */
        while(strcmp(fileAssociations[count], "END")) {

            if(!(strcmp(fileAssociations[count], ext)))
                return;
            count++;
        }

        count++;
    }

    /* No program found for specified file-type */
    strcpy(prog, "ERROR");
}

void
getProgName(char *prog, char *progName) {

    short count = 0, temp = 0;

    while(prog[count] != '\0')
        count++;

    while(prog[count] != '/')
        count--;

    count++;
    while(prog[count] != '\0') {
        progName[temp] = prog[count];
        temp++;
        count++;
    }
    progName[temp] = '\0';
}

