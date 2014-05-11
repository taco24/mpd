/*
 * thread.h
 *
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <stdio.h>
#include <stdlib.h>

struct shared_data {
	float pitch;   // pitch of airplane
	float bank;    // bank of airplane
	float a_nrml;
	float a_side;
	float a_axil;
	float elongation1;  // position of cylinder 1
	float elongation2;  // position of cylinder 2
	int isRunning;  // is calculation finished
	int thread_id; 	// id of the thread
	float speed;   // speed of movement (easy, normal, difficult)
	int mode;       // mode (ground, airborne)
	int stop;       // stop thread
};

void *run(void *ptr_shared_data);

#endif /* THREAD_H_ */
