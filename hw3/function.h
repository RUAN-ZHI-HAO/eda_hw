#ifndef FUNCTION_H
#define FUNCTION_H

#include "my_struct.h"

int calculateCost(const Point &point, const std::map<std::string, int> &loss,const std::map<Point, int> &gridUsageCount, const Net &net);

#endif