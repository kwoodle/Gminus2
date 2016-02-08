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

//#include <cstdlib>
#include "Torr.h"
#include <cmath>
#include <vector>
//#include <numeric>
#include <string>
#include <iostream>
#include "Hyperg.h"
#include "Trolley.h"

using namespace Tor;
using std::string;
using std::cout;
using std::vector;
using std::endl;

int main(int argc, char** argv) {

	// Create Trolleyrun object using filename
//	string filenm("/home/kim/NetBeansProjects/ToroidCoords/tail.dat");
	string filenm("/home/kim/NetBeansProjects/ToroidCoords/trolley840.dat");
	std::ifstream fs(filenm);
	if(!fs.is_open()){
		std::cerr << "Failed to open " << filenm << endl;
		exit(1);
	} 
	Trolley trl(fs);
	if (!trl.scan()) {
		std::cerr << "Failed to scan " << filenm << endl;
	}
	else
		trl.print_rows(cout);
	
//	if (trl.nbads() > 0)
//		trl.print_bads(cout);
	
	auto rit = trl.get_rowvec(18);
	if (rit != trl.end_rows())
		cout << "size 18 = " << rit->second.size() << endl;
	else
		cout << "no probe 18" << endl;

	return 0;
}