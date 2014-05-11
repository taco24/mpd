/*
 ============================================================================
 Name        : motionplatform.c
 Author      : colomboard
 Version     :
 Copyright   : colomboard
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "motionplatform.h"
#include <unistd.h>
#include "modeltest.h"

int STOP_PROGRAM = FALSE;

struct shared_data l_shared_data;

int main(void) {
	// local variables
	pthread_t l_thread;
	int l_thread_id = 1;
	int l_thread_return_code = 0;
	int l_counter = 0;
	int l_result = 0;
	double pi = 3.14159265358979323846;


	puts("motionplatform info : started.");
	testModelOnly();
	l_result = loadSettings();
	l_result = initLog();
	printf("motionplatform info : setting loaded %d - %d - %d\n", l_result,
			parameterX, parameterY);
	printf("motionplatform info : creating thread %d\n", l_thread_id);
	l_shared_data.thread_id = l_thread_id;
	l_shared_data.mode = 0;
	l_shared_data.stop = 0;
	l_shared_data.speed = 1.0;
	l_thread_return_code = pthread_create(&l_thread, NULL, run, (void *) &l_shared_data);
	if (l_thread_return_code) {
		printf(
				"motionplatform error: return code from pthread_create() is %d\n",
				l_thread_return_code);
		exit(-1);
	}

	while (STOP_PROGRAM == FALSE) {
#if IBM
		Sleep(WAIT_TIME);
#endif
#if LIN
		sleep(WAIT_TIME / 1000);
#endif

		// update pitch and bank
		l_shared_data.pitch = (3.4 - l_counter / 10.0) * pi/180.0;
		l_shared_data.bank = (2.4 + l_counter / 10.0) * pi/180.0;

		l_counter++;
		printf("motionplatform info : running %d\n", l_counter);
		printf(
				"motionplatform info : pitch: %f bank: %f / cyl1: %f - cyl2: %f\n",
				l_shared_data.pitch, l_shared_data.bank, l_shared_data.elongation1, l_shared_data.elongation2);
		if (l_counter > 10) {
			STOP_PROGRAM = TRUE;
		}
	}

	l_shared_data.stop = 1;
	l_result = saveSettings();
	printf("motionplatform info : save settings: %d\n", l_result);
	l_result = closeLog();
	printf("motionplatform info : exit program.\n");

	return EXIT_SUCCESS;
}
