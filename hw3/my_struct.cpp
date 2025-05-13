#include "my_struct.h"

Point::Point() : x(-1), y(-1) {};
Point::Point(int x, int y) : x(x), y(y) {};
Net::Net(int index, int srcX, int srcY, int dstX, int dstY) :index(index), src(srcX, srcY), dst(dstX, dstY), loss(0) {};