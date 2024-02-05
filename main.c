#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

char* searchFile(const char *filename) {
    char command[256];
    sprintf(command, "powershell -Command \"Get-Command -CommandType Application -Name '%s' | Select-Object -ExpandProperty Definition\"", filename);

    FILE *fp = _popen(command, "r");
    if (fp == NULL) {
        printf("Error executing PowerShell command\n");
        return NULL;
    }

    char path[256];
    if (fgets(path, sizeof(path), fp) != NULL) {
        strtok(path, "\n");

        char *trimmedPath = path;
        while (*trimmedPath == ' ' || *trimmedPath == '\t') {
            trimmedPath++;
        }
        size_t len = strlen(trimmedPath);
        while (len > 0 && (trimmedPath[len - 1] == ' ' || trimmedPath[len - 1] == '\t')) {
            trimmedPath[--len] = '\0';
        }

        // Allocate memory for the path
        char *resultPath = (char*)malloc(strlen(trimmedPath) + 1);
        if (resultPath != NULL) {
            strcpy(resultPath, trimmedPath);
        } else {
            printf("Memory allocation error\n");
            resultPath = NULL;
        }

        _pclose(fp);

        return resultPath;
    } else {
        printf("File not found\n");
        _pclose(fp);
        return NULL;
    }
}

void openProcess(char * path){
    if (path != NULL) {
        PROCESS_INFORMATION pi;
        STARTUPINFO si;
        ZeroMemory(&si, sizeof(si));
        si.cb = sizeof(si);
        ZeroMemory(&pi, sizeof(pi));
        if (!CreateProcess(path, NULL, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
            fprintf(stderr, "Error Creating Process: %lu\n", GetLastError());
        }

    }
}
int main() {
    char filename[256];
    printf("Enter the filename: ");
    scanf(" %255s", filename);

    char* path = searchFile(filename);
    openProcess(path);

    printf("Enter the filename: ");
    scanf(" %255s", filename);
    path = searchFile(filename);
    openProcess(path);

    free(path);
    return 0;
}
