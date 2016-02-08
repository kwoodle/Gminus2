//	Trolley.cpp

#include "Trolley.h"
#include <iostream>
#include <sstream>
#include <algorithm>

// (cos,sin)
//								(0,1)
//			(+.50,+.86)				    (-.50,+.86)

//		(+.86,+.50)			(0,1)				   (-.86,+.50)						12
//																					13			  11
//																				 14		4		 10
// (1,0)		 (1,0)		(x,x)		  (-1,0)			(-1,0)		15		5	1	3		9
//																				 16		2		  8
//																					17				 7
//	  (+.86,-.50)			(0,-1)				(-.86,-.50)						6

//		  (+.50,-.86)					  (-.50,-.86)
//								(0,-1)

constexpr double pi = boost::math::constants::pi<double>();
constexpr double rt3_2 = sqrt(3.)/2.;
const double hz = 61.74E6;		// mixing frequency
const double Probe::r1 = 1.75;
const double Probe::rp = 3.5;

DMap Probe::make_radii() {
	DMap v;
	v[1] = 0.;
	for (int i : {2,3,4,5})
		v[i] = r1;
	for (int i=6; i==17; ++i)
		v[i] = rp;
	return v;
}

DMap Probe::radii = Probe::make_radii();

DMap Probe::make_csns() {
	DMap v;
	for (int i : {1, 2, 4, 6, 2})
		v[i] = 0.;
	v[15] = v[5] = 1.;
	v[14] = v[16] = rt3_2;
	v[13] = v[17] = 0.5;
	v[11] = v[7] = -0.5;
	v[10] = v[8] = -rt3_2;
	v[3] = v[9] = -1.;

	return v;
}
DMap Probe::csns = Probe::make_csns();

DMap Probe::make_sns() {
	DMap v;
	for (int i : {1, 3, 5, 9, 15})
		v[i] = 0.;
	v[4] = v[12] = 1.;
	v[13] = v[11] = rt3_2;
	v[14] = v[10] = 0.5;
	v[16] = v[8] = -0.5;
	v[17] = v[7] = -rt3_2;
	v[2] = v[6] =-1.;
	
	return v;
}

DMap Probe::sns = Probe::make_sns();

/*
4516 251 31628.61 12 3 2001 118 38 4000 1000 1 25 125 16 1 28983 22 6408 5429 339658426 0 4972 5266
4517 251 31628.66 12 3 2001 118 38 4000 1000 1 25 125 17 1 27848 21 6406 5429 339658473 0 4974 5267
4518 251 31628.73 12 3 2001 118 38 4000 1000 1 25 125 18 256 0 0 3203 2714 339658557 0 4974 5270
4519 252 31628.79 12 3 2001 118 38 4000 1000 1 25 125 1 1 28637 22 6406 5429 339658607 0 4977 5270
4520 252 31628.84 12 3 2001 118 38 4000 1000 1 25 125 2 1 28929 22 6406 5429 339658654 0 4979 5272
 * s >> 4250 >> 252 >> 31628.79 >> 12 >> 3 >> 2001 >>
 *       118 >> 38 >> 4000 >> 1000 >> 1 >> 25 >> 125 >>
 *       1 >> 1 >> 28637 >> 22 >> 6406 >> 5429 >>
 *			339658607 
 */
  

TRow::TRow(std::string l) {
	double interval, zcs;  // zero crossing interval and crossings
	double dum;				  // dummy
	std::stringstream s(l);
	s >> idx >> dum >> time >> dum >> dum >> dum >>
		dum >> dum >> dum >> dum >> dum >> dum >> dum >>
		pb.ipb >> chk >> interval >> zcs >> dum >> dum >>
		uns >> dum >> encl >> encr;
//	pb.ppm = (zcs>0 && interval>0) ? hz*zcs/interval : -1;
	pb.ppm = (zcs>0 && interval>0) ? zcs/interval*1.e6 : -1;
}

std::ostream& operator<<(std::ostream& os, const TRow& R) {
	return os << "idx = " << R.idx << " time = " << R.time <<
		" ipb = " << R.pb.ipb << " chk = " << R.chk << " ppm = " << R.pb.ppm <<
		" un sec = " << R.uns << " encodl = " << R.encl << " encodr = " << R.encr;
}

void Trolley::print_bads(std::ostream& os) const {
	for (auto b : bads) {
		os << "Bad TRow!! " << b << std::endl;
	}
}
void Trolley::print_rows(std::ostream& os) const {
	for (auto it = rows.begin(); it != rows.end(); ++it) {
		os << "size rows( " << it->first << ") = " << it->second.size() << std::endl;
	}
}
bool Trolley::scan() {
   if (fs.is_open()) {
		std::string line;
//	   while (!fs.eof()) {
		while (getline(fs, line)){ 
//       std::cout << line << std::endl;
			TRow r(line);
//			std::cout << r << std::endl;
			if (r.pb.ppm < 0. || r.chk != 1)
				bads.push_back(r);
			else
				rows[r.pb.ipb].push_back(r);
      }
   fs.close();
   }
	else
		return false;
	return true;
}

