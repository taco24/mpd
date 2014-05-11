/*
 * HellWorld.c
 *
 * This plugin implements the canonical first program.  In this case, we will
 * create a window that has the text hello-world in it.  As an added bonus
 * the  text will change to 'This is a plugin' while the mouse is held down
 * in the window.
 *
 * This plugin demonstrates creating a window and writing mouse and drawing
 * callbacks for that window.
 *
 */
#if IBM
#include <windows.h>
#endif
#include <GL/gl.h>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <XPLMProcessing.h>
#include "XPLMDisplay.h"
#include "XPLMDataAccess.h"
#include "XPLMGraphics.h"
#include "XPLMCamera.h"
#include "xplane.h"
#include "colomboard.h"
#include "thread.h"
#include "log.h"
#include <pthread.h>

XPLMDataRef	MPD_DR_groundspeed = NULL;
XPLMDataRef	MPD_DR_fnrml_prop = NULL;
XPLMDataRef	MPD_DR_fside_prop = NULL;
XPLMDataRef	MPD_DR_faxil_prop = NULL;
XPLMDataRef	MPD_DR_fnrml_aero = NULL;
XPLMDataRef	MPD_DR_fside_aero = NULL;
XPLMDataRef	MPD_DR_faxil_aero = NULL;
XPLMDataRef	MPD_DR_fnrml_gear = NULL;
XPLMDataRef	MPD_DR_fside_gear = NULL;
XPLMDataRef	MPD_DR_faxil_gear = NULL;
XPLMDataRef	MPD_DR_m_total = NULL;
XPLMDataRef	MPD_DR_the = NULL;
XPLMDataRef	MPD_DR_psi = NULL;
XPLMDataRef	MPD_DR_phi = NULL;

float g_theta_prev = 0;
float g_phi_prev = 0;
int initialized = 0;
int deviceInitialized = 0;
int emergencyStop = 0;
pthread_t g_thread;
int g_thread_id = 1;
int g_thread_return_code = 0;
struct shared_data g_shared_data;
float g_thread_elongation1 = 0;
float g_thread_elongation2 = 0;
int g_targetValue1;
int g_targetValue2;
int g_boardValue1;
int g_boardValue2;
struct usb_data g_usb_data;

/*
 * Global Variables.  We will store our single window globally.  We also record
 * whether the mouse is down from our mouse handler.  The drawing handler looks
 * at this information and draws the appropriate display.
 *
 */

XPLMWindowID gWindow = NULL;
int MyDrawCallback(XPLMDrawingPhase inPhase, int inIsBefore, void * inRefcon);

void MyDrawWindowCallback(XPLMWindowID inWindowID, void * inRefcon);

void MyHandleKeyCallback(XPLMWindowID inWindowID, char inKey,
		XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon,
		int losingFocus);

int MyHandleMouseClickCallback(XPLMWindowID inWindowID, int x, int y,
		XPLMMouseStatus inMouse, void * inRefcon);

/*
 * XPluginStart
 *
 * Our start routine registers our window and does any other initialization we
 * must do.
 *
 */
PLUGIN_API int DLL_PUBLIC XPluginStart(char * outName, char * outSig, char * outDesc) {
	/* First we must fill in the passed in buffers to describe our
	 * plugin to the plugin-system. */

	strcpy(outName, "Motionplatform - colomboard");
	strcpy(outSig, "colomboard.motionplatform");
	strcpy(outDesc, "colomboard 11.04.14");

	gWindow = XPLMCreateWindow(50, 300, 100, 200, /* Area of the window. */
	1, /* Start visible. */
	MyDrawWindowCallback, /* Callbacks */
	MyHandleKeyCallback, MyHandleMouseClickCallback, NULL); /* Refcon - not used. */

	MPD_DR_groundspeed = XPLMFindDataRef("sim/flightmodel/position/groundspeed");
	MPD_DR_fnrml_prop = XPLMFindDataRef("sim/flightmodel/forces/fnrml_prop");
	MPD_DR_fside_prop = XPLMFindDataRef("sim/flightmodel/forces/fside_prop");
	MPD_DR_faxil_prop = XPLMFindDataRef("sim/flightmodel/forces/faxil_prop");
	MPD_DR_fnrml_aero = XPLMFindDataRef("sim/flightmodel/forces/fnrml_aero");
	MPD_DR_fside_aero = XPLMFindDataRef("sim/flightmodel/forces/fside_aero");
	MPD_DR_faxil_aero = XPLMFindDataRef("sim/flightmodel/forces/faxil_aero");
	MPD_DR_fnrml_gear = XPLMFindDataRef("sim/flightmodel/forces/fnrml_gear");
	MPD_DR_fside_gear = XPLMFindDataRef("sim/flightmodel/forces/fside_gear");
	MPD_DR_faxil_gear = XPLMFindDataRef("sim/flightmodel/forces/faxil_gear");
	MPD_DR_m_total = XPLMFindDataRef("sim/flightmodel/weight/m_total");
	MPD_DR_the = XPLMFindDataRef("sim/flightmodel/position/theta");
	MPD_DR_psi = XPLMFindDataRef("sim/flightmodel/position/psi");
	MPD_DR_phi = XPLMFindDataRef("sim/flightmodel/position/phi");
	g_theta_prev = XPLMGetDataf(MPD_DR_the);
	g_phi_prev = XPLMGetDataf(MPD_DR_phi);
	/* We must return 1 to indicate successful initialization, otherwise we
	 * will not be called back again. */

	return 1;
}

/*
 * XPluginStop
 *
 * Our cleanup routine deallocates our window.
 *
 */
PLUGIN_API void DLL_PUBLIC XPluginStop(void) {
	XPLMDestroyWindow(gWindow);
}

/*
 * XPluginDisable
 *
 * We do not need to do anything when we are disabled, but we must provide the handler.
 *
 */
PLUGIN_API void DLL_PUBLIC XPluginDisable(void) {
	char cTmp[30];
	closeDevice();
	g_shared_data.stop = 1;
    sprintf(cTmp, "mot.platform finished. %d , %d\n", g_usb_data.targetValue1, g_usb_data.targetValue2);
	writeLog(cTmp);
	closeLog();
}

/*
 * XPluginEnable.
 *
 * We don't do any enable-specific initialization, but we must return 1 to indicate
 * that we may be enabled at this time.
 *
 */
PLUGIN_API int DLL_PUBLIC XPluginEnable(void) {
	char cTmp[30];
	deviceInitialized = initDevice();

	g_shared_data.thread_id = g_thread_id;
	g_shared_data.mode = 0;
	g_shared_data.stop = 0;
	g_shared_data.speed = 1.0;
	g_thread_return_code = pthread_create(&g_thread, NULL, run, (void *) &g_shared_data);
	if (g_thread_return_code) {
		printf(
				"xplane plugin error: return code from pthread_create() is %d\n",
				g_thread_return_code);
		return 0;
	}

	initLog();
    sprintf(cTmp, "mot.platform started. %d , %d\n", g_usb_data.targetValue1, g_usb_data.targetValue2);
	writeLog(cTmp);

	return 1;
}

/*
 * XPluginReceiveMessage
 *
 * We don't have to do anything in our receive message handler, but we must provide one.
 *
 */
PLUGIN_API void DLL_PUBLIC XPluginReceiveMessage(XPLMPluginID inFromWho, long inMessage,
		void * inParam) {
}

int checkValue(int value) {
	if (value < 0) {
		return 0;
	}
	if (value > 245) {
		return 245;
	}
	return value;
}

float MPD_fallout(float data, float low, float high)
{
	if (data < low) return data;
	if (data > high) return data;
	if (data < ((low + high) * 0.5)) return low;
    return high;
}

float MPD_fltlim(float data, float min, float max)
{
	if (data < min) return min;
	if (data > max) return max;
	return data;
}

float MPD_fltmax2 (float x1,const float x2)
{
	return (x1 > x2) ? x1 : x2;
}



/*
 * MyDrawingWindowCallback
 *
 * This callback does the work of drawing our window once per sim cycle each time
 * it is needed.  It dynamically changes the text depending on the saved mouse
 * status.  Note that we don't have to tell X-Plane to redraw us when our text
 * changes; we are redrawn by the sim continuously.
 *
 */
void MyDrawWindowCallback(XPLMWindowID inWindowID, void * inRefcon) {
	int l_change;
	int left, top, right, bottom;
	int tempTarget1, tempTarget2;
	float color[] = { 1.0, 1.0, 1.0 }; /* RGB White */
	char cTmp[30];

	/* First we get the location of the window passed in to us. */
	XPLMGetWindowGeometry(inWindowID, &left, &top, &right, &bottom);

	float groundspeed = XPLMGetDataf(MPD_DR_groundspeed);
	float fnrml_prop = XPLMGetDataf(MPD_DR_fnrml_prop);
	float fside_prop = XPLMGetDataf(MPD_DR_fside_prop);
	float faxil_prop = XPLMGetDataf(MPD_DR_faxil_prop);
	float fnrml_aero = XPLMGetDataf(MPD_DR_fnrml_aero);
	float fside_aero = XPLMGetDataf(MPD_DR_fside_aero);
	float faxil_aero = XPLMGetDataf(MPD_DR_faxil_aero);
	float fnrml_gear = XPLMGetDataf(MPD_DR_fnrml_gear);
	float fside_gear = XPLMGetDataf(MPD_DR_fside_gear);
	float faxil_gear = XPLMGetDataf(MPD_DR_faxil_gear);
	float m_total = XPLMGetDataf(MPD_DR_m_total);
	float the = XPLMGetDataf(MPD_DR_the);
	float psi = XPLMGetDataf(MPD_DR_psi);
	float phi = XPLMGetDataf(MPD_DR_phi);

	float ratio = MPD_fltlim(groundspeed*0.2,0.0,1.0);
//	float a_nrml= MPD_fallout(fnrml_prop+fnrml_aero+fnrml_gear,-0.1,0.1)/MPD_fltmax2(m_total,1.0);
//	float a_side= (fside_prop+fside_aero+fside_gear)/MPD_fltmax2(m_total,1.0)*ratio;
//	float a_axil= (faxil_prop+faxil_aero+faxil_gear)/MPD_fltmax2(m_total,1.0)*ratio;

	//	float a_nrml= MPD_fallout(fnrml_prop+fnrml_aero+fnrml_gear,-0.1,0.1)/MPD_fltmax2(m_total,1.0);
	//	float a_side= (fside_prop+fside_aero+fside_gear)/MPD_fltmax2(m_total,1.0)*ratio;
	//	float a_axil= (faxil_prop+faxil_aero+faxil_gear)/MPD_fltmax2(m_total,1.0)*ratio;
	float a_nrml= -9.81;
	float a_side = 0.0;
	float a_axil = 0.0;


	g_shared_data.pitch = -phi * M_PI/ 180.0;
	g_shared_data.bank = -the * M_PI/ 180.0;
	g_shared_data.a_nrml = a_nrml;
	g_shared_data.a_side = a_side;
	g_shared_data.a_axil = a_axil;
	g_thread_elongation1 = g_shared_data.elongation2;
	g_thread_elongation2 = g_shared_data.elongation1;
	tempTarget1 = checkValue(round((g_thread_elongation1 - 0.88) * 500.0));
	tempTarget2 = checkValue(round((g_thread_elongation2 - 0.88) * 500.0));

	sprintf(cTmp, "ax,ay,az  : %3.2f / %3.2f / %3.2f", g_shared_data.a_axil, g_shared_data.a_side, g_shared_data.a_nrml);
	XPLMDrawString(color, left - 20, bottom - 60, cTmp, NULL, xplmFont_Basic);
	sprintf(cTmp, "Pitch,Bank: %3.2f / %3.2f", g_shared_data.pitch, g_shared_data.bank);
	XPLMDrawString(color, left - 20, bottom - 40, cTmp, NULL, xplmFont_Basic);
	sprintf(cTmp, "Position  : %3.2f / %3.2f", g_thread_elongation1, g_thread_elongation2);
	XPLMDrawString(color, left - 20, bottom, cTmp, NULL, xplmFont_Basic);


	if (deviceInitialized < 0) {
		sprintf(cTmp, "USB Status: %d / %d - %d", deviceInitialized, tempTarget1, tempTarget2);
		XPLMDrawString(color, left - 20, bottom - 20, cTmp, NULL, xplmFont_Basic);
		return;
	} else {
		sprintf(cTmp, "USB Status: %d / %d (%d) - %d (%d)", deviceInitialized, tempTarget1, g_usb_data.boardValue1 - tempTarget1, tempTarget2, g_usb_data.boardValue2 - tempTarget2);
		XPLMDrawString(color, left - 20, bottom - 20, cTmp, NULL, xplmFont_Basic);
	}

	l_change = readDevice(&g_usb_data);
	if (l_change < 0) {
		// usb device error
		deviceInitialized = l_change;
		return;
	}

	if (the != g_theta_prev || phi != g_phi_prev) {
		if (g_usb_data.targetValue1 != tempTarget1 || g_usb_data.boardValue2 != tempTarget2) {
			g_usb_data.targetValue1 = tempTarget1;
			g_usb_data.targetValue2 = tempTarget2;
			writeDevice(&g_usb_data);
		    sprintf(cTmp, "mp: %d , %d\n", g_usb_data.targetValue1, g_usb_data.targetValue2);
			writeLog(cTmp);
		}
	} else if (initialized == 0) {
		if (g_usb_data.targetValue1 != tempTarget1 || g_usb_data.boardValue2 != tempTarget2) {
			g_usb_data.targetValue1 = tempTarget1;
			g_usb_data.targetValue2 = tempTarget2;
			writeDevice(&g_usb_data);
		    sprintf(cTmp, "mp: %d , %d\n", g_usb_data.targetValue1, g_usb_data.targetValue2);
			writeLog(cTmp);
		}
		initialized = 1;
	}
	g_theta_prev = the;
	g_phi_prev = phi;
}

/*
 * MyHandleKeyCallback
 *
 * Our key handling callback does nothing in this plugin.  This is ok;
 * we simply don't use keyboard input.
 *
 */
void MyHandleKeyCallback(XPLMWindowID inWindowID, char inKey,
		XPLMKeyFlags inFlags, char inVirtualKey, void * inRefcon,
		int losingFocus) {
	if (emergencyStop) {
		emergencyStop = 0;
	} else {
		emergencyStop = 1;
	}
}

/*
 * MyHandleMouseClickCallback
 *
 * Our mouse click callback toggles the status of our mouse variable
 * as the mouse is clicked.  We then update our text on the next sim
 * cycle.
 *
 */
int MyHandleMouseClickCallback(XPLMWindowID inWindowID, int x, int y,
		XPLMMouseStatus inMouse, void * inRefcon) {
	return 0;
}
