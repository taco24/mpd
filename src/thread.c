/*
 * thread.c
 */
#include "thread.h"
#if IBM
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <math.h>
#include "computation/constants.h"
#include "computation/computation.h"
#include "computation/constants.h"
#include "modeltest.h"
//#include "XPLMUtilities.h"

struct shared_data l_shared_data;
float MPD_MotionData[6];

float limit(float value) {
	return min(max(MIN_ANGLE, value), MAX_ANGLE);
}

float getAngleX(float accelerationX, float accelerationY, float accelerationZ) {
	char Buffer[256];
	sprintf(Buffer,"MPD AngleX: %3.2f, %3.2f, %3.2f\n", accelerationZ*CONSTANTS_TO_RADIANT, accelerationY*CONSTANTS_TO_RADIANT, atan2(-accelerationZ*CONSTANTS_TO_RADIANT, -accelerationY*CONSTANTS_TO_RADIANT));
	printf(Buffer);
	//	XPLMDebugString(Buffer);
	/*	if (accelerationZ == 0.0 && accelerationY == 0.0) {
		return 0.0;
	} else if (accelerationY == 0.0 && accelerationZ > 0) {
		return M_PI/2.0 - QUARTERTURN;
	} else if (accelerationY == 0.0 && accelerationZ < 0) {
		return M_PI/-2.0 - QUARTERTURN;
	}*/
	return limit(atan2(-accelerationZ*CONSTANTS_TO_RADIANT, -accelerationY*CONSTANTS_TO_RADIANT) - QUARTERTURN);
}

float getAngleY(float accelerationX, float accelerationY, float accelerationZ) {
	char Buffer[256];
	sprintf(Buffer,"MPD AngleY: %3.2f, %3.2f, %3.2f\n", accelerationZ*CONSTANTS_TO_RADIANT, -accelerationX*CONSTANTS_TO_RADIANT, atan2(accelerationZ*CONSTANTS_TO_RADIANT, -accelerationX*CONSTANTS_TO_RADIANT));
	printf(Buffer);
	//	XPLMDebugString(Buffer);
/*	if (accelerationX == 0.0 && accelerationZ == 0.0) {
		return 0.0;
	} else if (accelerationX == 0.0 && accelerationZ > 0) {
		return M_PI/2.0 - QUARTERTURN;
	} else if (accelerationX == 0.0 && accelerationZ < 0) {
		return M_PI/-2.0 - QUARTERTURN;
	}*/
	return limit(atan2(accelerationZ*CONSTANTS_TO_RADIANT, -accelerationX*CONSTANTS_TO_RADIANT) - QUARTERTURN);
}


void *run(void *ptr_shared_data) {
	struct shared_data *l_ptr_shared_data;
	float l_pitch = 0;
	float l_bank = 0;
	int l_counter = 0;

	l_ptr_shared_data = (struct shared_data *) ptr_shared_data;
	MODEL* model = createModel();

	// while stop == 0 calculate position.
	while (l_ptr_shared_data->stop == 0) {
		//l_pitch = l_ptr_shared_data->pitch;
		//l_bank = l_ptr_shared_data->bank;
		l_pitch = getAngleX(l_ptr_shared_data->a_axil, l_ptr_shared_data->a_side, l_ptr_shared_data->a_nrml);
		l_bank = getAngleY(l_ptr_shared_data->a_axil, l_ptr_shared_data->a_side, l_ptr_shared_data->a_nrml);
		char Buffer[256];
		sprintf(Buffer,"MPD Thread: %3.2f, %3.2f\n", l_pitch, l_bank);
//		XPLMDebugString(Buffer);

		if (l_pitch != model->pitch || l_bank != model->bank) {
			model->pitch = l_pitch;
			model->bank = l_bank;
			computeElongations(model);
			// wait 10 milliseconds
			printf(
					"thread info : thread #%d, %d - pitch: %f bank: %f / cyl1: %f - cyl2: %f\n",
					l_ptr_shared_data->thread_id, l_counter, model->pitch, model->bank, model->elongation1, model->elongation2);
			l_ptr_shared_data->elongation1 = model->elongation1;
			l_ptr_shared_data->elongation2 = model->elongation2;
		}

	#if IBM
			Sleep(10);
	#endif
	#if LIN
			usleep(10);
	#endif
			l_counter++;
	}
	printf("thread info : stopping thread #%d, %d!\n",
			l_ptr_shared_data->thread_id, l_counter);
	pthread_exit(NULL);
	return 0;
}
