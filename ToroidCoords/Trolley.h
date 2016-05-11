/* 
 * File:   Trolley.h
 * Author: kim
 *
 * Created on January 30, 2016, 9:38 PM
 */

#ifndef TROLLEY_H
#define TROLLEY_H

#include "Probe.h"
#include <string>
#include <vector>
#include <tuple>
#include <fstream>
#include <ctime>

class TRow;

// Trolley TProbe data and traits
//
class TProbe : protected Probe {
protected:
   static DMap radii;
   static DMap csns;
   static DMap sns;
   static DMap make_radii();
   static DMap make_csns();
   static DMap make_sns();
   static const int Npb = 17;				// number of trolley probes
   double ppm;						// (nmr freq -hz)/hz
	long encl;									// left drum encoder
	long encr;									// right drum encoder
public:
   static const double r1;		// radius of inner probes 2 - 5
   static const double rp;		// radius of outer probes 6 - 17

   friend class Trolley;
   friend class TRow;
   friend std::ostream& operator<<(std::ostream&, const TRow&);
};
// Row of trolley run file
//
class TRow {
public:
   explicit TRow(std::string s);
   long idx;
   TProbe pb;	     // probe
   int chk;
   long uns;		  // unix secs? not!
 
};


using TRVec = std::vector<TRow>;
using TRMap = std::map<int, TRVec>;

class Trolley {
public:
   explicit Trolley(std::ifstream& s) : fs(s) {};
   bool scan();
   int nbads() {
      return bads.size();
    }
   void print_bads(std::ostream&) const;
   void print_rows(std::ostream&) const;
   TRMap::iterator get_rowvec(int i) {
      return rows.find(i);
   }
   TRMap::iterator end_rows() {
      return rows.end();
   }

protected:
   std::ifstream& fs;
   TRVec bads;
   TRMap rows;
};

#endif /* TROLLEY_H */

