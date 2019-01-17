/* 
 * File:   main.cpp
 * Author: kim
 *
 * Created on April 5, 2016, 8:07 PM
 */

#include <cstdlib>
#include <vector>
//#include <numeric>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "Funcs.h"


using namespace std;

int main(int argc, char** argv) {

	string datdir;

    if (const char* env_h = std::getenv("PWD")) {
		datdir = env_h;
        datdir += "/../data/";
	} else {
        cerr << "Failed to get PWD";
		exit(1);
	}

	string filenm = datdir;
	filenm += "dipole_022.csv";

	ifstream fs(filenm);
	if(!fs.is_open()){
		std::cerr << "Failed to open " << filenm << endl;
		exit(1);
	} 

	vector<float> phis;
	vector<float> bees;
	string line;
	getline(fs, line);
	float wd;
	stringstream s(line);
	while (s>>wd)
		phis.push_back(wd);
	cout << "Pushed back " << phis.size() << " phis from " << filenm << endl;
	
	getline(fs, line);
	stringstream ops(line);
	while (ops>>wd) {
		bees.push_back(wd);
	}
	cout << "Pushed back " << bees.size() << " bees from " << filenm << endl;
	
	if (phis.size() != bees.size()) {
		cerr << "Size of phis NE size of bees!";
		exit(1);
	}
	float avg = Avg(bees);
	cout << "Avg(bees) = " << avg << endl;
	float rmsbs = Rms(bees);
	cout << "Rms(bees) = " << rmsbs << endl;
	
	string pltnm = datdir;
	pltnm += "dipole_022.plt";
	ofstream pltfil(pltnm);
	if(!pltfil.is_open()) {
		cerr << "Failed to open plotfile " << pltnm << endl;
		exit(1);
	}
	auto itb = bees.begin();
	for (auto it=phis.begin(); it != phis.end(); ++it, ++itb) {
		pltfil << *it << " " << *itb << endl;
	}
	pltfil.close();

	vector<float> bs2 = unslope(bees);
	for (int i=0; i<bees.size(); ++i) {
//		cout << i << " " << bs2[i] << " " << bees[i] << endl;
	}
	
	bees = bs2;
	rmsbs = Rms(bees);
	cout << "Rms(unslope(bees)) = " << rmsbs << endl;
	phis.pop_back();
	vector<float>ams;
	vector<float>bms;
	vector<float>cms;
	vector<float>pms;
	
	int mmax=121;
	mmax = bees.size()/2;
	for (int i=0; i<mmax; ++i) {
		float am = a_m(bees, phis, i);
		ams.push_back(am);
		float bm = b_m(bees, phis, i);
		bms.push_back(bm);
		float cm = sqrt(am*am+bm*bm);
		cms.push_back(cm);
//		float pm = atan(am/bm);
		float pm = atan2(bm, am);
		pms.push_back(pm*180/pi);
	}

	pltnm = datdir;
	pltnm += "asandbs.plt";
	ofstream abstr(pltnm);
		if(!abstr.is_open()) {
		cerr << "Failed to open plotfile " << pltnm << endl;
		exit(1);
	}
	for(int i=1; i<ams.size(); ++i) {
//		cout << "a(" <<i<<")= "<<ams[i]<<"  b(" <<i<<")= "<<bms[i] << "  c("<<i<<")="<<cms[i]<<endl;
		abstr << i <<" "<< ams[i] << " " << bms[i] 
				<< " " << cms[i] <<" "<< pms[i]<< endl;
	}

	string opfilenm = datdir;
	opfilenm += "optim_dipole_022.csv";

	ifstream opfs(opfilenm);
	if(!opfs.is_open()){
		std::cerr << "Failed to open " << opfilenm << endl;
		exit(1);
	}
	
	vector<float> phios;
	vector<float> beeos;
	float wd1{0.};
	float wd2{0.};
	while (getline(opfs, line)) {
		stringstream s(line);
		s >> wd1 >> wd2;
		phios.push_back(wd1);
		beeos.push_back(wd2);
	}
	
	cout << "Pushed " << phios.size() << " phios and " << beeos.size() << " beeos." << endl;
	
	avg = Avg(beeos);
	rmsbs = Rms(beeos, avg);
	cout << "Avg opt = " << avg << "  Rms opt = " << rmsbs << endl;
	beeos = unslope(beeos);
	avg = Avg(beeos);
	rmsbs = Rms(beeos, avg);
	cout << "Avg unsloped opt = " << avg << "  Rms unsloped opt = " << rmsbs << endl;
	phios.pop_back();
	vector<float> amos;
	vector<float> bmos;
	vector<float> cmos;
	vector<float> pmos;
	
	mmax=121;
	mmax = beeos.size()/2;
	for (int i=0; i<mmax; ++i) {
		float am = a_m(beeos, phios, i);
		amos.push_back(am);
		float bm = b_m(beeos, phios, i);
		bmos.push_back(bm);
		float cm = sqrt(am*am+bm*bm);
		cmos.push_back(cm);
//		float pm = atan(am/bm);
		float pm = atan2(bm, am);
		pmos.push_back(pm*180/pi);
	}

	pltnm = datdir;
	pltnm += "asandbos.plt";
	ofstream abostr(pltnm);
		if(!abostr.is_open()) {
		cerr << "Failed to open plotfile " << pltnm << endl;
		exit(1);
	}
	for(int i=1; i<ams.size(); ++i) {
//		cout << "a(" <<i<<")= "<<ams[i]<<"  b(" <<i<<")= "<<bms[i] << "  c("<<i<<")="<<cms[i]<<endl;
		abostr << i <<" "<< amos[i] << " " << bmos[i] 
						<<" "<< cmos[i] << " " << pmos[i] << endl;
	}

	int tom{36};
	vector<float> vm = sum_to_m(bees, phis, tom);

	pltnm = datdir;
	std::ostringstream plotfnm(pltnm, std::ios_base::ate);
	std::ostringstream plotofnm(pltnm, std::ios_base::ate);
	plotfnm << "vm" << tom << ".plt";
	plotofnm << "vmo" << tom << ".plt";
//	pltnm += "vm.plt";
	ofstream vmstr(plotfnm.str());
	if(!vmstr.is_open()) {
		cerr << "Failed to open plotfile " << plotfnm.str() << endl;
		exit(1);
	} else
		cout << "Opened plotfile " << plotfnm.str() << endl;

	for(int i=0; i<vm.size(); ++i) {
		vmstr << phis[i] << " " << vm[i] << endl;
	}
	
	vector<float> vmo = sum_to_m(beeos, phios, tom);
	pltnm = datdir;
	pltnm += "vmo.plt";
	ofstream vmostr(plotofnm.str());
	if(!vmostr.is_open()) {
		cerr << "Failed to open pl	otfile " << plotofnm.str() << endl;
		exit(1);
	} else
		cout << "Opened plotfile " << plotofnm.str() << endl;
	
	for(int i=0; i<vmo.size(); ++i) {
		vmostr << phios[i] << " " << vmo[i] << endl;
	}

//-------------------------------
	filenm = datdir;
	filenm += "raw_dipole_022.csv";

	ifstream fsr(filenm);
	if(!fsr.is_open()){
		std::cerr << "Failed to open " << filenm << endl;
		exit(1);
	} 

	map<float, float> rawd;
//	string line;
//	getline(fsr, line);
//	float wd;
//	stringstream sr(line);
	while (getline(fsr, line)) {
		stringstream s(line);
		s >> wd1 >> wd2;
		rawd.insert(std::pair<float, float>(wd1, wd2));
	}
	
	cout << "Pushed " << rawd.size() << " onto rawd." << endl;
	

	pltnm = datdir;
	pltnm += "raw_dipole_022.plt";
	ofstream pltrfil(pltnm);
	if(!pltrfil.is_open()) {
		cerr << "Failed to open plotfile " << pltnm << endl;
		exit(1);
	}
	for (auto it=rawd.begin(); it!=rawd.end(); ++it) {
		pltrfil << it->first << " " << it->second << endl;
	}
	pltrfil.close();


	return 0;
}

