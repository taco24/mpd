/*
 * constants.c
 *
 *  Created on: 27.06.2010
 *      Author: matkauf
 */
#include <math.h>

// Ersatz für C-malloc
// Verwendung: STRUCTTYP *variablenname = new(STRUCTTYP) statt wie in Java STRUCTTYP variablenname = new STRUCTTYP()
#define new(x) ((x*) malloc (sizeof(x)))


// Konversionsfunktionen:
#define CONSTANTS_TO_RADIANT					M_PI/180.0
#define CONSTANTS_TO_DEGREE						180.0/M_PI
#define CONSTANTS_G								9.81
#define CONSTANTS_TO_G							1.0/CONSTANTS_G
#define CONSTANTS_TO_M_PER_SECOND_SQUARE		CONSTANTS_G

// 90-Grad-Drehung im Bogenmass
#define QUARTERTURN 90.0 * CONSTANTS_TO_RADIANT
#define MAX_ANGLE QUARTERTURN
#define MIN_ANGLE -MAX_ANGLE
//#define MAX_ANGLE 1.57079632679489661923
//#define MIN_ANGLE -1.57079632679489661923


// Grundplatteneinheit:
#define CONSTANTS_GROUND_THICKNESS				0.01 // 1cm dicke Grundplatte, sodass Zylindergrundgelenk auf 13cm über Boden zu stehen kommt
#define CONSTANTS_GROUND_SIZE_X					0.85 // 0.75m hat Grundplatte in alle Richtungen
#define CONSTANTS_GROUND_SIZE_Y					0.85
#define CONSTANTS_DIST_GROUND_CYLINDERJOINT_X	0.4 // 0.75m hat Grundplatte in alle Richtungen
#define CONSTANTS_DIST_GROUND_CYLINDERJOINT_Y	0.5 // 0.75m hat Grundplatte in alle Richtungen
#define CONSTANTS_SOCKEL_SIZE 					0.1 // Sockel ist 10cm dick
#define CONSTANTS_DIST_MAINJOINT_GROUNDJOINT  	(0.46-0.08-0.05)  // Vertikale Distanz zwischen Haupt- u. Grundplattenzylindergelenk
#define CONSTANTS_DIST_MAINJOINT_FLOOR			0.46 // Vertikale Distanz zwischen Hauptgelenkmitte und Boden (nicht Grundplattenoberseite!)
#define CONSTANTS_RADIUS_MAIN_JOINT				0.05 // Hauptkugelgelenk hat Radius 5cm

// Platformeinheit:
#define CONSTANTS_PLATFORM_SIZE_X				0.5 // 2m breite Platformplatte
#define CONSTANTS_PLATFORM_SIZE_Y				0.5 // 2m breite Platformplatte
#define CONSTANTS_PLATFORM_THICKNESS			0.05 // 5cm dicke Platformplatte
#define CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_X	0.4 // Abstand von Hauptgelenk- zum Zylindernebengelenk in X-Richtung in horizontale Richtung
#define CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_Y	0.5 // Abstand von Hauptgelenk- zum Zylindernebengelenk in Y-Richtung
#define CONSTANTS_DIST_MAINJOINT_UPPERJOINT		0.80 // Vertikaler Abstand zwischen Hauptgelenk und oberem Zylindergelenk
#define CONSTANTS_PLATFORM_GAP_SIZEX			0.3 // horizontaler Einschnitt in die 1m x 1m-Hauptplatte in X-Richtung (Zylinderaussparung)
#define CONSTANTS_PLATFORM_GAP_SIZEY			0.2 // horizontaler Einschnitt in die 1m x 1m-Hauptplatte in Y-Richtung (Zylinderaussparung)
#define CONSTANTS_RADIUS_CYLINDER_JOINT			0.05 // Zylindergelenk-Radius

// Zylinder:
#define CONSTANTS_RADIUS_CYLINDER				0.05 // Zylinderdicke
#define CONSTANTS_CYLINDER_MIN_LENGTH			0.88 // Zylinderlänge eingefahren, Gelenkmittelpunkt bis Gelenkmittelpunkt
#define CONSTANTS_CYLINDER_DIFF					.50 // Ausfahrlänge
#define CONSTANTS_CYLINDER_MAX_LENGTH			(CONSTANTS_CYLINDER_MIN_LENGTH+CONSTANTS_CYLINDER_DIFF) // Zylinderlaenge ausgefahren

// Fussstütze
#define CONSTANTS_FOOTREST_SIZEX				0.46  // Breite
#define CONSTANTS_FOOTREST_HEIGHT				0.38 // Höhe

// Modell allgemein:
#define CONSTANTS_MODEL_MAXANGLE				40.0 * CONSTANTS_TO_RADIANT // max. Kippgrad (+-40°)


// Lookuptabellen-Grösse
#define CONSTANTS_PRECOMPUTESIZE				30
#define CONSTANTS_LOOKUPLENGTH					((2*CONSTANTS_PRECOMPUTESIZE+1)*(2*CONSTANTS_PRECOMPUTESIZE+1))


