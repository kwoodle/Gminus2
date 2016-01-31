/* 
 * File:   Hyperg.h
 * Author: kwoodle
 *
 * Created on November 12, 2015, 2:12 PM
 * 
    gsl_sf_result result;
    int status = gsl_sf_bessel_J0_e (x, &result);
 */

#ifndef HYPERG_H
#define HYPERG_H

#include <gsl/gsl_sf.h>
#include <gsl/gsl_math.h>
#include <utility>

typedef std::pair<double,double> Dpair;
class Hypg {
public:
    Hypg(){
        res.val = GSL_NAN;
        res.err = GSL_POSINF;
    };
    Dpair operator()(int n, int m, double x);

private:
    gsl_sf_result res;
};

#endif /* HYPERG_H */

