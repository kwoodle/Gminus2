/*
Funcs.cpp
*/

#include "Funcs.h"
#include <iostream>

float Avg (vector<float> v) {
	float a{0.};
	auto s = v.size();
	for (auto it=v.begin(); it!=v.end(); ++it) {
		a += *it/s; 
	}
	return a;
}
float Rms (vector<float> v) {
	return Rms(v, Avg(v));
}

float Rms(vector<float> v, float a) {
	float r{0.};
	for (auto it=v.begin(); it!=v.end(); ++it) {
		float t = *it - a;
		r += t*t/v.size();
	}
	return r;
}

float a_m(vector<float> v, vector<float> d, int m) {
	if (v.size() != d.size()) {
		std::cout << "Mismatch in a_m!" << std::endl;
		return -99999.;
	}
	float a{0.};
	auto id=d.begin();
	for (auto it=v.begin(); it!=v.end(); ++it, ++id){
		a += (*it)*sin((*id)*m*pi/180.);
	}
	a *= 2./v.size();
	return a;
}

float b_m(vector<float> v, vector<float> d, int m) {
	if (v.size() != d.size()) {
		std::cout << "Mismatch in b_m!" << std::endl;
		return -99999.;
	}
	float a{0.};
	auto id=d.begin();
	for (auto it=v.begin(); it!=v.end(); ++it, ++id){
		a += (*it)*cos((*id)*m*pi/180.);
	}
	a *= 2./v.size();
	return a;
}

// assume vector v has data from 0 and 2pi. assume linear slope
// don't return value at 2pi
vector<float> unslope(vector<float> v) {
	vector<float> r;
	float slope = (v[v.size()-1] -v[0])/v.size();
	for (int i=0; i<v.size()-1; ++i) {
		r.push_back(v[i]-slope*i);
	}
	return r;
}

// sum the first m multipoles
//
vector<float> sum_to_m(vector<float> v, vector<float> d, int m) {
	if (v.size() != d.size()) {
		std::cout << "Mismatch in sum_to_m!" << std::endl;
		exit(1);
	}
	vector<float> r(v.size(), 0.);
	//	step thru m
	for (int rm=0; rm<=m; ++rm) {
		float am = a_m(v, d, rm);
		float bm = b_m(v, d, rm);
		// step thru vec
		auto id = d.begin();
		for (int i=0; i<v.size(); ++i) {
			float phi = d[i];
			float s = am*sin(rm*phi*pi/180.);
			s += bm*cos(rm*phi*pi/180.);
			if (rm == 0) {
				s /= 2.;
			}
			r[i] += s;
		}
	}
	return r;
}

// calculate a_m for raw data file which has unevenly spaced phis
// data are map<phi,bee>
float raw_am(map<float, float> dat, int im) {
	float r;
	
	return r;
}