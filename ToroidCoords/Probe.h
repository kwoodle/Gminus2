/* 
 * File:   Probe.h
 * Author: kwoodle
 *
 * Created on February 9, 2016, 3:56 PM
 */

#ifndef PROBE_H
#define PROBE_H

#include <map>
#include <vector>
#include <utility>
#include <fstream>
#include <string>
#include <boost/date_time/gregorian/gregorian.hpp>

using namespace::boost::gregorian;




extern const double pi;
extern const double Mhz;

// map probe num to trait
//
// for trolley probes
using DMap = std::map<int, double>;

// for fixed probes pair<imodule, iprobe>
using PMap = std::map<std::pair<int, int>, double>;

// Probe data and traits
//
class Probe {
protected:
   int iprobe;							// probe number
	double time;					// secs since midnight
public:
   double phi() const;			// azimuthal angle
	double gettime() const {
		return time;
	}
};

class Fixed;

class FProbe : public Probe {
protected:
	int imod;		// module number
	double f;		// nmr freq
	int nz;			// zero crossings
	static PMap phis;				// map module, probe to azimuth
	static PMap make_phis();
public:
	double freq() const {
		return f;
	}
	int getnz() const {
		return nz;
	}
	static PMap::const_iterator find_prb(PMap::key_type k) {
		return phis.find(k);
	}
	static PMap::const_iterator phis_end() {
		return phis.end();
	}
	friend class Fixed;
   friend std::ostream& operator<<(std::ostream&, const FProbe&);
};

using FPVec = std::vector<FProbe>;
using FRMap = std::map<std::pair<int,int>, FPVec>;
using PVec = std::vector<std::pair<double,double>>;

class Fixed {
public:
	explicit Fixed(std::ifstream& s) : nlines(0), npread(0), fs(s) {};
	bool scan();
	std::size_t nbads() {
		return bads.size();
	}
	void print_bads(std::ostream&) const;
	void print_rows(std::ostream&) const;
	int lines() const {
		return nlines;
	}
	int preads() const {
		return npread;
	}
	bool make_fplot(int imod, int iprb, const std::string&) const;
	
protected:
	std::ifstream& fs;
	date date0;
	int nlines;
	int npread;
	FRMap bads;
	FRMap rows;
};
#endif /* PROBE_H */

