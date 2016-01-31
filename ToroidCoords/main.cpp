/* 
 * File:   main.cpp
 * Author: kwoodle
 *
 * Created on October 17, 2015, 2:41 PM
 */

// Magnetic field magnitude B satisfies Laplace's equation ins of a torus
// Expand in toroidial harmonics and solve boundary value problem

// pkg-config --cflags gsl broken
// gsl-config --cflags => -I/usr/include

//#include <cstdlib>
#include "Torr.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <numeric>
#include <string>
#include "Hyperg.h"

using namespace Tor;
using std::string;
using std::cout;
using std::vector;
using std::endl;

int main(int argc, char** argv) {

    string filenm("/home/kim/NetBeansProjects/ToroidCoords/tail.dat");
    string q;
    string line;
    std::ifstream fs(filenm);
    if (fs.is_open()) {
        while (!fs.eof()) {
            getline(fs, line);
            q += line;
            cout << line << endl;
        }
        fs.close();
    }

    // Fourier coeffs for expansion in toroidial corrds
    vector< vector<double> > ac;
    vector< vector<double> > bc;
    vector< vector<double> > cc;
    vector< vector<double> > dc;

    double sp = a / rp;
    double so = a / ro;
    cout << "so = " << so << "  sp= " << sp << endl;

    Hypg hg;
    Dpair vo = hg(1, 2, 0.001);
    if (vo.first != GSL_NAN)
        cout << "val = " << vo.first << "  err = " << vo.second << endl;
    vector<int> v(10); // vector with 100 ints.
    std::iota(std::begin(v), std::end(v), 0); // Fill with 0, 1, ...
    for (auto n : v) {
        Dpair out = hg(n, 2, 0.001);
        if (out.first != GSL_NAN)
            cout << "n = " << n << "  val = " << out.first << "  err =" << out.second << endl;
    }
    return 0;
}

