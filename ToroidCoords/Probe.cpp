/* 
 * File:   Probe.cpp
 * Author: kwoodle
 *
 * Created on February 9, 2016, 3:56 PM
 */

#include "Probe.h"
#include <iostream>
#include <sstream>
//#include <ctime>
#include <boost/math/constants/constants.hpp>



constexpr double pi = boost::math::constants::pi<double>();
const double Mhz = 61.74;		// reference frequency*10e-6

const int npol = 36;		// number of top or bottom pole pieces


// Hogan Nguyen, ppt 31 Mar 2014
//		Inflector service lines at yoke-yoke boundary (15 degrees?)
//		Injection point at 12.35 degrees (clockwise from center of pole A?)
//		Inflector vacuum chamber (viewed from top, right to left) 
//
//		xdeg - 3 - 4.15deg - 2 - 5deg - 3 - 5deg - 2 - 5deg - 3 - 5deg - 2
//
//		shows probe pairs at pole boundaries and triplets at pole centers
//		For 30 degree yoke this give x = 30 - 24.15 = 5.85 deg, so betw ctrs
//
//		ctr - 9.15deg - ctr - 10deg - ctr - 10.85deg -
//
//	Joe Grange and Ernst ppts mapping probe locations to modules
//		3 probes at pole centers and 2 at ends, as above, except near inflector
//		(yoke A) and kickers? (yokes D, E). Trolley start/stop at center of pole
//		4 (bottom), left pole of yoke B.
//		Take center of pole 4 as zero degrees. 
//
//	BOTTOM POLES
//		Center of pole n = degrees = (n-4)*10
//		pole(module, probe) = degrees
//
//		1(1,2)=-30; 330		2(1,14)=-20; 340 
//		3(2,8)=-10; 350		4(3,2)=0;
//		5(3,13)=10;				6(4,2)=20;
//		7(4,12)=30;				8(5,2)=40;
//		9(5,12)=50;				10(6,2)=60;
//		11(6,14)=70;			12(7,6)=80;
//		13(7,18)=90;			14(8,10)=100;
//		15(9,2)=110;			16(9,14)=120;
//		17(10,4)=130;			18(10,14)=140;
//		19(11,6)=150;			20(11,16)=160;

PMap FProbe::make_phis() {
	PMap pm;
	
	// bottom pole probes
	pm[{1,2}] = 330.;
	pm[{1,14}] = 340.;
	pm[{2,8}] = 350.;
	pm[{3,2}] = 0.;
	pm[{3,12}] = 10.;
	pm[{4,2}] = 20.;
	pm[{4,12}] = 30.;	//7
	pm[{5,2}] = 40.;	//8
	pm[{5,12}] = 50.;	//9
	pm[{6,2}] = 60.;	//10
	pm[{6,14}] = 70.;	//11
	pm[{7,6}] = 80.;	//12
	pm[{7,18}] = 90.;	//13
	pm[{8,10}] = 100.;	//14
	pm[{9,2}] = 110.;		//15
	pm[{9,14}] = 120.;	//16
	pm[{10,4}] = 130.;	//17
	pm[{10,14}] = 140.;	//18
	pm[{11,6}] = 150.;	//19
	pm[{11,16}] = 160.;	//20
	pm[{12,6}] = 170.;	//21
	pm[{12,16}] = 180.;	//22
	pm[{13,6}] = 190.;	//23
	pm[{13,16}] = 200.;	//24
	pm[{14,6}] = 210.;	//25
	pm[{15,2}] = 220.;	//26
	pm[{15,12}] = 230.;	//27
	pm[{16,2}] = 240.;	//28
	pm[{16,12}] = 250.;	//29
	pm[{17,2}] = 260.;	//30
	pm[{17,12}] = 270.;	//31
	pm[{18,6}] = 280.;	//32
	pm[{18,16}] = 290.;	//33
	pm[{19,6}] = 300.;	//34
	pm[{19,16}] = 310.;	//35
	pm[{20,6}] = 320.;	//36
	
	return pm;
}

PMap FProbe::phis = FProbe::make_phis();

// From Ernst fixedprobes.c
// sscanf ...  month, day, year, seconds, hClock
//		for i = 0 to N_SYNCHRONOUS(=20)
//			sscanf ... module, probe, nZero, frequency
// Fixed probe -- nmr_head.dat from nmr_2_12_101_20_2.dat
//
// 02 12 01 72126.41 2256005896 ..
//		01 01 0151 046539.58 02 01 0003 043383.95 ..
//		03 01 0114 049031.20 04 01 0005 046576.62 ..
//		05 01 0069 047419.42 06 01 0002 031570.64 ..
//		07 01 0004 057971.01 08 01 0079 051275.39 ..
//		09 01 0153 052228.23 10 01 0001 050377.83 ..
//		11 01 0005 063171.19 12 01 0034 056059.36 ..
//		13 01 0003 057915.06 14 01 0002 055248.62 ..
//		15 01 0001 039761.43 16 01 0000 000000.00 ..
//		17 01 0094 051213.60 18 01 0000 000000.00 ..
//		19 01 0004 069025.02 20 01 0002 033726.81

std::ostream& operator<<(std::ostream& os, const FProbe& FP) {
	os << "imod=" << FP.imod << "  ip=" << FP.iprobe <<
		" time=" << FP.time <<  " nz=" << FP.nz << " f=" << FP.f;
}

void Fixed::print_bads(std::ostream& os) const {
	for (auto it = bads.begin(); it != bads.end(); ++it) {
		os << "size badFrows(" << it->first.first << "," << it->first.second <<
			") = " << it->second.size() << std::endl;
	}
}
void Fixed::print_rows(std::ostream& os) const {
	PMap wanted = FProbe::phis;
	int nprobes = 0;
	for (auto it = rows.begin(); it != rows.end(); ++it) {
		os << "size Frows(" << it->first.first << "," << it->first.second <<
			") = " << it->second.size() << std::endl;
		++nprobes;
		wanted.erase(it->first);
	}
	os << "Total of " << nprobes << " probes in map." << std::endl;
	for (auto itp = wanted.begin(); itp != wanted.end(); ++itp) {
		os << "Missing probe(" << itp->first.first << "," << itp->first.second <<
			")" << std::endl;
	}
}

constexpr int secspday = 24*3600;
bool Fixed::scan() {
	if (fs.is_open()) {
		std::string line;
		long dummy;
		bool first(true);
		while (getline(fs, line)){ 
//			std::cout << line << std::endl;
			++nlines;
			std::tm tmline;
			std::stringstream s(line);
			s >> tmline.tm_mon >> tmline.tm_mday >> tmline.tm_year;
			date datel = date_from_tm(tmline);
			FProbe prb;
			s >> prb.time >> dummy;
			if(first) {
				date0 = datel;
				first = false;
			}
			else {
				date_duration dd = datel - date0;
				prb.time += dd.days()*secspday;
			}
			while (s >> prb.imod >> prb.iprobe >> prb.nz >> prb.f ) {
				++npread;
				auto pb_pair = std::make_pair(prb.imod, prb.iprobe);
				if (FProbe::find_prb(pb_pair) == FProbe::phis_end())
					continue;
				if (prb.f == 0) {
					bads[pb_pair].push_back(prb);
				}
				else {
//					std::cout << prb << std::endl;
					rows[pb_pair].push_back(prb);
				}
			}
      }
   fs.close();
   }
	else
		return false;
	return true;
}

bool Fixed::make_fplot(int nmod, int nprb, const std::string& basnm) const {
	std::ostringstream plotfnm(basnm, std::ios_base::ate);
	plotfnm << "nmrpl_" << nmod <<"_"<<nprb<<".out";
	std::ofstream os(plotfnm.str());
	if(!os.is_open()) {
		std::cerr << "Failed to open plotfile!" << 
			plotfnm.str() << std::endl;
		return false;
	}
	else
		std::cout << "Opened plotfile " << plotfnm.str() << std::endl;
	
	auto pb_pair = std::make_pair(nmod, nprb);
	if (rows.find(pb_pair) == rows.end())
		return false;
	for (auto pitr=rows.at(pb_pair).begin(); pitr != rows.at(pb_pair).end(); ++pitr) {
		os << pitr->gettime() << " " << pitr->freq() <<
			" " << pitr->getnz() << std::endl;
	}
	os.close();
	return true;
}
