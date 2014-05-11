#include <stdlib.h>
#include <math.h>
#include "computation.h"

MODEL model;

double adjust(double min, double real, double max) {
	return fmax(fmin(max, real), min);
}

double cylinderRecomputeTransformation(double zylx, double zyly, double zylz, VECTOR3D *pleuelVector) {
	double t1 = (pleuelVector->x - zylx);
	double t2 = (pleuelVector->y - zyly);
	double t3 = (pleuelVector->z - zylz);
	return sqrt(t1*t1 + t2*t2 + t3*t3);
}


void getRotation(MATRIX3D *m, double alphaUmXAchse, double betaUmYAchse) {
	double cosx = cos(alphaUmXAchse);
	double sinx = sin(alphaUmXAchse);
	double cosy = cos(betaUmYAchse);
	double siny = sin(betaUmYAchse);
	m->m00 = cosy; m->m01 = sinx*siny; m->m02 = cosx*siny;
	m->m10 = 0.0;  m->m11 = cosx;      m->m12 = -sinx;
	m->m20 =-siny; m->m21 = sinx*cosy; m->m22 = cosx*cosy;
}


void turn(MATRIX3D *m, VECTOR3D *v) {
	double d1 = m->m00 * v->x + m->m01 * v->y + m->m02 * v->z;
	double d2 = m->m10 * v->x + m->m11 * v->y + m->m12 * v->z;
	v->z      = m->m20 * v->x + m->m21 * v->y + m->m22 * v->z;
	v->y = d2;
	v->x = d1;
}


void getElongations(LOOKUPVALUE *lookupValue) {
	MATRIX3D platformRotation;
	VECTOR3D v;
	double alpha = lookupValue->angles.alpha;
	double beta = lookupValue->angles.beta;
	getRotation(&platformRotation, alpha, beta);
	v.x = -CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_X;
	v.y = CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_Y;
	v.z = CONSTANTS_DIST_MAINJOINT_UPPERJOINT;
	turn(&platformRotation, &v);
	lookupValue->elongations.cylinderelongation1 = cylinderRecomputeTransformation(
			-CONSTANTS_DIST_GROUND_CYLINDERJOINT_X,
			 CONSTANTS_DIST_GROUND_CYLINDERJOINT_Y,
			-CONSTANTS_DIST_MAINJOINT_GROUNDJOINT,
			&v);

	v.x = -CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_X;
	v.y = -CONSTANTS_DIST_PLATFORM_CYLINDERJOINT_Y;
	v.z = CONSTANTS_DIST_MAINJOINT_UPPERJOINT;
	turn(&platformRotation, &v);
	lookupValue->elongations.cylinderelongation2 = cylinderRecomputeTransformation(
			-CONSTANTS_DIST_GROUND_CYLINDERJOINT_X,
			-CONSTANTS_DIST_GROUND_CYLINDERJOINT_Y,
			-CONSTANTS_DIST_MAINJOINT_GROUNDJOINT,
			&v);
}

MODEL* createModel() {
	int count = 0, i, j;
	for (i=-CONSTANTS_PRECOMPUTESIZE; i<=CONSTANTS_PRECOMPUTESIZE; i++) {
		for (j=-CONSTANTS_PRECOMPUTESIZE; j<=CONSTANTS_PRECOMPUTESIZE; j++) {
			LOOKUPVALUE *v = &(model.lookupValues[count]);
			v->angles.alpha = ((double) (i*CONSTANTS_MODEL_MAXANGLE)) / ((double) CONSTANTS_PRECOMPUTESIZE);
			v->angles.beta = ((double) (j*CONSTANTS_MODEL_MAXANGLE)) / ((double) CONSTANTS_PRECOMPUTESIZE);
			getElongations(v);
			count++;
		}
	}
	return &model;
}


void computeAngles(MODEL *model) {
	return;
}

void computeElongations(MODEL *model) {
	LOOKUPVALUE *lv;
	lv=  &(model->lookupValue);
	lv->angles.alpha = adjust(-CONSTANTS_MODEL_MAXANGLE, model->pitch, CONSTANTS_MODEL_MAXANGLE);
	lv->angles.beta = adjust(-CONSTANTS_MODEL_MAXANGLE,  model->bank, CONSTANTS_MODEL_MAXANGLE);
	getElongations(lv);
	model->elongation1 = adjust(-CONSTANTS_CYLINDER_MIN_LENGTH, lv->elongations.cylinderelongation1, CONSTANTS_CYLINDER_MAX_LENGTH);
	model->elongation2 = adjust(-CONSTANTS_CYLINDER_MIN_LENGTH, lv->elongations.cylinderelongation2, CONSTANTS_CYLINDER_MAX_LENGTH);
}


