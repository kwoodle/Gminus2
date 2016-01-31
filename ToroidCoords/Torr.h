/* 
 * File:   Torr.h
 * Author: kwoodle
 *
 * Created on October 30, 2015, 2:23 PM
 */

#ifndef TORR_H
#define	TORR_H


namespace Tor {
    const double a = 711.2;   // (major) radius of storage ring (cm)
    const double rp = 3.5;    // probes at radius rp (cm))
    const double ro = 4.5;    // boundary of storage region
    const int N = 13;         // number of probes at radius rp ?????
    int M = 6000;       // number of steps in azimuth????
                        // approximate Joe Grange 4 mar 2015 Argonne
                        // 300 encoder counts ~ 1 degree
}



#endif	/* TORR_H */

