/* 
 * File:   Hyperg.cpp
 * Author: kwoodle
 * 
 * Created on November 12, 2015, 2:12 PM
 */

#include "Hyperg.h"

Dpair Hypg::operator()(int n, int m, double x) {
    double a = (n+m)/2. + 0.25;
    double b = a + 0.50;
    double c = n + 1;
	if (gsl_sf_hyperg_2F1_e(a,b,c,x,&res) == GSL_SUCCESS)
		return Dpair(res.val, res.err);
	else
		return Dpair(GSL_NAN, GSL_POSINF);
}
