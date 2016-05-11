/* 
 * File:   Funcs.h
 * Author: kim
 *
 * Created on April 6, 2016, 7:18 PM
 */

#ifndef FUNCS_H
#define FUNCS_H

#include <vector>
#include <map>
#include <cmath>

#include <boost/math/constants/constants.hpp>

constexpr float pi = boost::math::constants::pi<float>();

using std::vector;
using std::map;

extern float Avg (vector<float>);
extern float Rms (vector<float>);
extern float Rms (vector<float>, float av);
extern float a_m (vector<float> v, vector<float> d, int m);
extern float b_m (vector<float> v, vector<float> d, int m);
extern float raw_am (map<float, float>, int m);
extern float raw_bm (map<float, float>, int m);
extern vector<float> unslope(vector<float>);
extern vector<float> sum_to_m(vector<float> v, vector<float> d, int m);

#endif /* FUNCS_H */

