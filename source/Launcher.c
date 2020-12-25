#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LINE_BUFFER_LENGTH 512
#define COMMAND_BUFFER_LENGTH 2048

#ifdef _WIN32
	const char* COMMON_FILE_PATH = "./launcher/Windows/python_executable.txt";
	const char* COMMAND_PREFIX = "ECHO OFF && ";
	const char* DEFAULT_QUOTE = "\"";
#elif defined(__unix__)
	const char* COMMON_FILE_PATH = "./launcher/Linux/python_executable.txt";
	const char* COMMAND_PREFIX = "";
	const char* DEFAULT_QUOTE = "'";
#endif

const char* DEFAULT_LAUNCHER_SCRIPT = "./launcher/launcher.py";
const char* FALLBACK_LAUNCHER_SCRIPT = "./source/launcher.py";

typedef struct {
    int readSuccess; // Set to 1 when all obligatory files are found
	char pythonExecutable[LINE_BUFFER_LENGTH];
	char launcherScript[LINE_BUFFER_LENGTH];
} Common;

void copyStringChunk(char* source, char* target, const char from, const char to) {
	int started = 0;

	while (*source != from && *source != '\0') {
		source++;
		if (*source == from) {
			started = 1;
			source++;
			break;
		}
	}
	while (started && *source != '\0' && *source != to) {
		*target = *source;
		target++;
		source++;

		if (*source == to) {
			*target = '\0';
			started = 0;
		}
	}
}

void getPathFromCommon(Common* common) {
	FILE* filePointer = fopen(COMMON_FILE_PATH, "r");
	char buffer[LINE_BUFFER_LENGTH];

	if (filePointer != NULL) {
        while(fgets(buffer, LINE_BUFFER_LENGTH, filePointer) != NULL) {
            if (strstr(buffer, "PYTHON_EXECUTABLE") != NULL)
                copyStringChunk(buffer, common->pythonExecutable, *DEFAULT_QUOTE, *DEFAULT_QUOTE);
        }
        fclose(filePointer);
	}
	else {
        return;
	}
	filePointer = fopen(DEFAULT_LAUNCHER_SCRIPT, "r");

	if (filePointer) {
		strcpy(common->launcherScript, DEFAULT_LAUNCHER_SCRIPT);
		fclose(filePointer);
	}
	else {
        filePointer = fopen(FALLBACK_LAUNCHER_SCRIPT, "r");
        if (filePointer) {
            strcpy(common->launcherScript, FALLBACK_LAUNCHER_SCRIPT);
            fclose(filePointer);
        }
        else {
            return;
        }
	}
	common->readSuccess = 1;
}

int main(int argc, char** argv) {
	Common common;
	char command[COMMAND_BUFFER_LENGTH];

	// Cleanup strings
	strcpy(common.launcherScript, "");
	strcpy(common.pythonExecutable, "");
	strcpy(command, "");

	common.readSuccess = 0;

	getPathFromCommon(&common);

	if (common.readSuccess) {
        printf("> Python path: %s\n> Launcher script path: %s\n\n", common.pythonExecutable, common.launcherScript);

        strcat(command, COMMAND_PREFIX);
        strcat(command, DEFAULT_QUOTE);
        strcat(command, common.pythonExecutable);
        strcat(command, DEFAULT_QUOTE);
        strcat(command, " ");
        strcat(command, DEFAULT_QUOTE);
        strcat(command, common.launcherScript);
        strcat(command, DEFAULT_QUOTE);

        printf("Command: %s\n", command);
        system(command);
	}
	else {
        printf("X Could not read python executable or launcher path!");
	}
}
