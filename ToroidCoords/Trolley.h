/* 
 * File:   Trolley.h
 * Author: kim
 *
 * Created on January 30, 2016, 9:38 PM
 */

#ifndef TROLLEY_H
#define TROLLEY_H

#include <string>
#include <vector>
#include <tuple>
#include <map>
#include <fstream>
#include <boost/math/constants/constants.hpp>

extern const double pi;
class TRow;
// map probe num to trait
using DMap = std::map<int,double>;

// Probe data and traits
//
class Probe {
protected:
	int ipb;							// probe number
   double ppm;						// (nmr freq -hz)/hz
	static DMap radii;
	static DMap csns;
	static DMap sns;
	static DMap make_radii();
	static DMap make_csns();
	static DMap make_sns();
public:
	static const int Npb = 17;				// number of trolley probes
	static const double r1;		// radius of inner probes 2 - 5
	static const double rp;		// radius of outer probes 6 - 17
	double phi();					// azimuthal angle

	friend class Trolley;
	friend class TRow;
   friend std::ostream& operator<<(std::ostream&, const TRow&);
};
// Row of trolley run file
//
class TRow {
public:
    TRow(std::string s);
    long idx;
    double time;
    Probe pb;        // probe
    int chk;
    long uns;       // unix secs
    long encl;      // encoder left
    long encr;      // encoder right
 
    friend std::ostream& operator<<(std::ostream&, const TRow&);
};



using RVec = std::vector<TRow>;
using RMap = std::map<int, RVec>;

class Trolley {
public:
    explicit Trolley(std::ifstream& s) : fs(s) {};
    bool scan();
    int nbads() {
		return bads.size();
    }
	 void print_bads(std::ostream&) const;
	 void print_rows(std::ostream&) const;
	 RMap::iterator get_rowvec(int i) {
		 return rows.find(i);
	 }
	 RMap::iterator end_rows() {
		 return rows.end();
	 }

protected:
	std::ifstream& fs;
	RVec bads;
	RMap rows;
};

#endif /* TROLLEY_H */

