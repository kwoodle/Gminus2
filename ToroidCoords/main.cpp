/* 
 * File:   main.cpp
 * Author: kwoodle
 *
 * Created on October 17, 2015, 2:41 PM
 */

// Magnetic field magnitude B does not! satisfy Laplace's equation
// Expand in toroidial harmonics and solve boundary value problem
// pkg-config --cflags gsl broken
// gsl-config --cflags => -I/usr/include

#include <cstdlib>
#include "Torr.h"
#include <cmath>
#include <vector>
//#include <numeric>
#include <string>
#include <iostream>
#include <sstream>
#include "Hyperg.h"
#include "Trolley.h"
#include "Probe.h"

using namespace Tor;
using std::string;
using std::cout;
using std::vector;
using std::endl;

int main(int argc, char** argv) {
	
	string datdir;
	
	if (const char* env_h = std::getenv("HOME")) {
		datdir = env_h;
		datdir += "/NetBeansProjects/ToroidCoords/";
	} else {
		std::cerr << "Failed to get HOME";
		exit(1);
	}
	// Create Trolleyrun object
	//
	string tfilenm = datdir;
	tfilenm += "trolley840.dat";
//	tfilenm += "tail.dat";

	std::ifstream tfs(tfilenm);
	if(!tfs.is_open()){
		std::cerr << "Failed to open " << tfilenm << endl;
		exit(1);
	} 
	Trolley trl(tfs);
	if (!trl.scan()) {
		std::cerr << "Failed to scan " << tfilenm << endl;
	}
	else
		trl.print_rows(cout);
	
//	if (trl.nbads() > 0)
//		trl.print_bads(cout);
	
/*	auto rit = trl.get_rowvec(18);
	if (rit != trl.end_rows())
		cout << "size 18 = " << rit->second.size() << endl;
	else
		cout << "no probe 18" << endl;
*/
	
	// create Fixed probe file object
	//
	string ffile = datdir;
	ffile += "nmr_3_29_101_20_48.dat";
	std::ifstream ffs(ffile);
	if(!ffs.is_open()) {
		std::cerr << "Failed to open " << ffile << endl;
		exit(1);
	}
	Fixed fix(ffs);
	if(!fix.scan()) {
		std::cerr << "Failed to scan " << ffile << endl;
	}
	else {
		cout << "Read " << fix.lines() << " lines and " << fix.preads() <<
			" probes \nfrom " << ffile << endl;
		fix.print_rows(cout);
		fix.print_bads(cout);
	}
	
//	cout << fix.nbads() << " bad fixed probe readings." << endl;
//	int nmod {1}, nprb{2};
//	int nmod {2}, nprb{8};
	int nmod {10}, nprb{4};
	bool chk = fix.make_fplot(nmod, nprb, datdir);

	return 0;
}