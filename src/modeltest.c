/*
 * modelonlytest.c
 *
 *  Created on: 27.06.2010
 *      Author: matkauf
 */
#include <math.h>
#include "computation/constants.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "computation/computation.h"
#include "modeltest.h"


void assertSimilar(double desired, double real) {
	if (fabs(desired - real) > 0.000001) {
		printf("   >  values are not equal: %lf <-> %lf \n", desired, real);
	}
}


void testElongation(MODEL *m, double alpha, double beta, double desired1, double desired2) {
	m->pitch = alpha;
	m->bank = beta;
	computeElongations(m);
	printf("elongation1= %lf\n", m->elongation1);
	printf("elongation2= %lf\n", m->elongation2);
	assertSimilar(desired1, m->elongation1);
	assertSimilar(desired2, m->elongation2);
}


void testAngle(MODEL *m, double elong1, double elong2, double desiredAlpha,
		double desiredBeta) {
	/*
	MODEL* m = new(MODEL);
	ANGLES* a = getAngles(m, elong1, elong2);
	log("alpha=" + a->alpha);
	log(" beta=" + a->beta);
	assertSimilar(desiredAlpha, a.getAlpha());
	assertSimilar(desiredBeta, a.getBeta());
	*/
}


void testModelOnly() {
	MODEL* model = createModel();
	printf("\n\nstart tests...\n");
	// Wenn Unittest fehlschl�gt, kontrollieren, dass die Werte f�r die Auslenkungen
	// �berhaupt m�glich sind (wegen zus�tzlichen Checks).
	testElongation(model, 0.17, -0.11, 1.1663910358789642, 0.996819364056);
	testElongation(model, -0.09,   0.02, 1.092014967456264, 1.1819679808191124);
	testAngle(model, 1.1, 1.05, 0.04886921905584121, -0.13264502315156887);
	testAngle(model, 1.15, 1.3, -0.15126186850617515, 0.26761715197246383);
	printf("tests finished.\n");
	return;
}



