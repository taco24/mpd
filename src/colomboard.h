/*
 * colomboard.h
 */

#ifndef COLOMBOARD_H_
#define COLOMBOARD_H_

struct usb_data {
	int targetValue1;  // target Value 1
	int targetValue2;  // target Value 2
	int boardValue1;  // board Value 1
	int boardValue2;  // board Value 2
};

int writeDevice(void *ptr_usb_data);
int readDevice(void *ptr_usb_data);
char* getVersionDevice();
int initDevice();
int closeDevice();

#endif /* COLOMBOARD_H_ */
