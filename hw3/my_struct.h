#ifndef MY_STRUCT_H
#define MY_STRUCT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>

struct Point {
    int x, y;

    Point(int x ,int y);
};

struct Net {
    int index;
    Point src, dst;
    std::vector<Point> path;

    Net(int index, int srcX, int srcY, int dstX, int dstY);
};


#endif