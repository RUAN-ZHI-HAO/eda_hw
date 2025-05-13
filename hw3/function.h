#ifndef FUNCTION_H
#define FUNCTION_H

#include "my_struct.h"

void AStar(Point dieArea, Net &net, const std::map<std::string, int> &loss, std::unordered_map<Point, int, PointHash> &gridUsageCount, const std::map<std::string, float> &ratio);
void outputFile(std::string &outputFileName, std::vector<Net> &nets);
float calculateNetCost(Net &net, std::unordered_map<Point, int, PointHash> &gridUsageCount, std::map<std::string, int> &loss);

#endif