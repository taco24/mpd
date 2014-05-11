/*
 * colomboard.c
 *
 */
#include <usb.h>
#include <stdio.h>
#include "thread.h"
#include "colomboard.h"

#define VERSION "0.1.0"
#define MY_VID 0xF055
#define MY_PID 0x0001
//#define MY_VID 0x10CF
//#define MY_PID 0x5500
//#define MY_VID 0x03EB
//#define MY_PID 0x20CC
/* the device's endpoints */
#define EP_IN 0x81
#define EP_OUT 0x02

#define BUF_SIZE 8

usb_dev_handle *dev;
int writeCounter = 0;
char tmp[BUF_SIZE];
char tmp2[BUF_SIZE];

int usb_status = 0;
int returnCode = 0;

int initDevice();
int closeDevice();
int readDevice(void *ptr_usb_data);
int writeDevice(void *ptr_usb_data);

void fatalError(const char *why) {
	fprintf(stderr, "Fatal error> %s\n", why);
	//	exit(17);
}

usb_dev_handle *find_colomboard_libusb();

usb_dev_handle* setup_libusb_access() {
	usb_dev_handle *colomboard_libusb;
	usb_set_debug(255);
	usb_init();
	usb_find_busses();
	usb_find_devices();

	if (!(colomboard_libusb = find_colomboard_libusb())) {
		printf("Couldn't find the USB device, Exiting\n");
		return NULL;
	}
#if LIN
	// linux detach kernel driver
	if (usb_detach_kernel_driver_np(colomboard_libusb, 0) < 0) {
		printf("Could not detach kernel driver: %d\n", 0);
	}
#endif

	if (usb_set_configuration(colomboard_libusb, 1) < 0) {
		printf("Could not set configuration 1 : \n");
		return NULL;
	}
	if (usb_claim_interface(colomboard_libusb, 0) < 0) {
		printf("Could not claim interface: %d\n", 0);
		return NULL;
	}
	return colomboard_libusb;
}

usb_dev_handle *find_colomboard_libusb() {
	struct usb_bus *bus;
	struct usb_device *dev;

	for (bus = usb_get_busses(); bus; bus = bus->next) {
		for (dev = bus->devices; dev; dev = dev->next) {
			if (dev->descriptor.idVendor == MY_VID && dev->descriptor.idProduct
					== MY_PID) {
				return usb_open(dev);
			}
		}
	}
	return NULL;
}

int writeDevice(void *ptr_usb_data) {
	if (usb_status < 0) {
		return usb_status;
	}
	struct usb_data *l_ptr_usb_data;
	l_ptr_usb_data = (struct usb_data *) ptr_usb_data;
	// write colomboard
	tmp[0] = l_ptr_usb_data->targetValue1 / 255;
	tmp[1] = l_ptr_usb_data->targetValue1 % 255;
	tmp[2] = l_ptr_usb_data->targetValue2 / 255;
	tmp[3] = l_ptr_usb_data->targetValue2 % 255;
	tmp[4] = l_ptr_usb_data->boardValue1 / 255;
	tmp[5] = l_ptr_usb_data->boardValue1 % 255;
	tmp[6] = l_ptr_usb_data->boardValue2 / 255;
	tmp[7] = l_ptr_usb_data->boardValue2 % 255;
	if ((usb_status = usb_interrupt_write(dev, EP_OUT, tmp, sizeof(tmp), 5000)) != sizeof(tmp)) {
		printf("error: interrupt write failed\n");
	}
	return 0;
}

int getValue(unsigned char a, unsigned char b) {
	int result = ((int) a) * 255 + ((int) b);
	return result;
}

int readDevice(void *ptr_usb_data) {
	int countBytes = 0;
	if (usb_status < 0) {
		return usb_status;
	}
	struct usb_data *l_ptr_usb_data;
	l_ptr_usb_data = (struct usb_data *) ptr_usb_data;
	// read colomboard
	signed char result = 0;
	countBytes = usb_interrupt_read(dev, EP_IN, tmp2, sizeof(tmp2), 5000);
	if (countBytes != sizeof(tmp2)) {
		printf("error: interrupt read failed\n");
		printf("%d\n", countBytes);
		result = -1;
	} else {
		l_ptr_usb_data->targetValue1 = getValue(tmp2[0], tmp2[1]);
		l_ptr_usb_data->targetValue2 = getValue(tmp2[4], tmp2[5]);
		l_ptr_usb_data->boardValue1 = getValue(tmp2[2], tmp2[3]);
		l_ptr_usb_data->boardValue2 = getValue(tmp2[6], tmp2[7]);
	}
	return result;
}

char* getVersionDevice() {
	return VERSION;
}

int initDevice() {
	if ((dev = setup_libusb_access()) == NULL) {
		usb_status = -1;
		return -1;
	}
	usb_status = 0;
	return 0;
}

int closeDevice() {
	if (dev == NULL) {
		usb_status = -1;
		return -1;
	} else {
		usb_release_interface(dev, 0);
		usb_close(dev);
	}
	usb_status = 0;
	return 0;
}
