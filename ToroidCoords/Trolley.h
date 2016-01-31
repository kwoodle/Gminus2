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

const int Npb = 17;  // number of probes

struct Row {
    float time;
    int ipb;        // probe
    double f;       // mixed down freqency of FID
    long uns;       // unix secs
    long enc1;      // encoder left
    long encr;      // encoder right
};

class Trolley {
public:
    Trolley(std::string s) : fname(s){};
protected:
    std::string fname;
    int nlines;     // lines in file
    

};


#endif /* TROLLEY_H */

