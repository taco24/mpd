/*
 * modeltypes.h
 *
 *  Created on: 27.06.2010
 *      Author: matkauf
 */

#include "constants.h"

typedef struct {
	double alpha;
	double beta;
} ANGLES;

typedef struct {
	double x;
	double y;
	double z;
} VECTOR3D;

typedef struct {
	double m00;
	double m01;
	double m02;
	double m10;
	double m11;
	double m12;
	double m20;
	double m21;
	double m22;
} MATRIX3D;

typedef struct {
	double cylinderelongation1;
	double cylinderelongation2;
} ELONGATIONS;

typedef struct {
	ELONGATIONS elongations;
	ANGLES angles;
} LOOKUPVALUE;

typedef struct {
	double radiusGelenk;
	double radiusCylinder;
	double minLength;
	double maxLength;
	MATRIX3D rotation;
	double elongation;
} HYDRAULICCYLINDER;



typedef struct {
	// in/out-Werte
	double pitch;
	double bank;
	double elongation1;
	double elongation2;
	// Die folgenden Felder sind nicht für die Schnittstelle nach aussen gedacht.
	ANGLES angles;
	ELONGATIONS elongations;
	HYDRAULICCYLINDER cylinder1;
	HYDRAULICCYLINDER cylinder2;
	LOOKUPVALUE lookupValues[CONSTANTS_LOOKUPLENGTH];
	LOOKUPVALUE lookupValue;
} MODEL;

