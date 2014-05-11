#include "modeltypes.h"

/*
 * Alloziert, kreiert und macht alle Vorberechnungen f�r ein MODEL,
 * mit welchem die nachfolgenden Methoden verwendet werden k�nnen.
 */
MODEL* createModel();

/*
 * Berechnet f�r die bereits gesetzten Attribute
 * model->elongation1, model->elongation2 (in Metern)
 * die zugeh�rigen Winkel model->pitch, model->bank
 * im Bogenmass.
 */
void computeAngles(MODEL *model);

/*
 * Berechnet f�r die bereits gesetzten Attribute
 * model->pitch, model->bank (im Bogenmass)
 * die zugeh�rigen Zylinderauslenkungen
 * model->elongation1, model->elongation2
 * in Metern.
 */
void computeElongations(MODEL *model);

/*
 * Dealloziert alles, was das Model bei der Initialisierung
 * selber alloziert hat (um Memory-Leaks zu vermeiden).
 */
void deallocateModel(MODEL *model);



