/*
 * settings.c
 */
#include "settings.h"

int parameterX = 0;
int parameterY = 0;
char szKey1[] = "parameterX";
char szKey2[] = "parameterY";

FILE *pSettingsFile = NULL;
char *settingsFileName = SETTINGS_FILE
;


int openSettingsRead(char *szFilename) {
	int rc = 0;
	pSettingsFile = fopen(szFilename, "r+");
	if (pSettingsFile == NULL) {
		printf("settings error: can't open file.\n");
		rc = 1;
	} else {
		printf("settings info : File opened.\n");
	}
	return rc;
}

int openSettings(char *szFilename) {
	int rc = 0;
	pSettingsFile = fopen(szFilename, "w+");
	if (pSettingsFile == NULL) {
		printf("settings error: can't open file.\n");
		rc = 1;
	} else {
		printf("settings info : File opened.\n");
	}
	return rc;
}

int closeSettings() {
	int rc = 0;
	if (pSettingsFile != NULL) {
		fclose(pSettingsFile);
	}
	return rc;
}

int writeSettings(char *text) {
	int rc = 0;
	if (pSettingsFile != NULL) {
		fputs(text, pSettingsFile);
		fflush(pSettingsFile);
	} else {
		rc = -1;
	}
	return rc;
}

int parseParameter(char *string) {
	char * pch;

	pch = strtok(string, " ,.=");
	while (pch != NULL) {
		printf("settings info : %s\n", pch);
		if (strcmp(szKey2, pch) != 0) {
			// param1
			pch = strtok(NULL, " ,.=");
			parameterX = atoi(pch);
		} else if (strcmp(szKey1, pch) != 0) {
			// param2
			pch = strtok(NULL, " ,.=");
			parameterY = atoi(pch);
		}
		pch = strtok(NULL, " ,.=");
	}
	return 0;
}

int loadSettings() {
	int rc = 0;
	char *buf[bufsize], *tok;

	openSettingsRead(settingsFileName);
	if (pSettingsFile != NULL) {
		while (fgets(buf, bufsize, pSettingsFile) != NULL) {
			/*Here we tokenize our string and scan for " \n" characters*/
			for (tok = strtok(buf, " \n"); tok; tok = strtok(0, " \n")) {
				parseParameter(tok);
			}
		} /*Continue until EOF is encoutered*/
		closeSettings();
	} else {
		fputs("File error", stderr);
		rc = -1;
	}
	return rc;
}

int saveSettings() {
	int rc = 0;
	char string[50];
	openSettings(settingsFileName);
	if (pSettingsFile != NULL) {
		rewind(pSettingsFile);
		// Save settings
		sprintf(string, "parameterX=%d\n", parameterX);
		writeSettings(string);
		sprintf(string, "parameterY=%d\n", parameterY);
		writeSettings(string);
	} else {
		rc = -1;
	}
	closeSettings();
	return rc;
}
