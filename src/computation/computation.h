#include "modeltypes.h"

/*
 * Alloziert, kreiert und macht alle Vorberechnungen für ein MODEL,
 * mit welchem die nachfolgenden Methoden verwendet werden können.
 */
MODEL* createModel();

/*
 * Berechnet für die bereits gesetzten Attribute
 * model->elongation1, model->elongation2 (in Metern)
 * die zugehörigen Winkel model->pitch, model->bank
 * im Bogenmass.
 */
void computeAngles(MODEL *model);

/*
 * Berechnet für die bereits gesetzten Attribute
 * model->pitch, model->bank (im Bogenmass)
 * die zugehörigen Zylinderauslenkungen
 * model->elongation1, model->elongation2
 * in Metern.
 */
void computeElongations(MODEL *model);

/*
 * Dealloziert alles, was das Model bei der Initialisierung
 * selber alloziert hat (um Memory-Leaks zu vermeiden).
 */
void deallocateModel(MODEL *model);



