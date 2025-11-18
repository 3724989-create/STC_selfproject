#ifndef _LQ_UTM_H_
#define _LQ_UTM_H_

#include <math.h>

#define PI 3.14159265358979323846

struct Point {
    double lat; // 纬度
    double lon; // 经度
};

struct UTM {
    double ux; // 平面坐标x
    double uy; // 平面坐标y
    int zone; // UTM投影区带号
};

extern struct Point _p;
extern struct UTM utm;
struct UTM latlon2utm(struct Point _p);
void Test_UTM(void);
#endif
