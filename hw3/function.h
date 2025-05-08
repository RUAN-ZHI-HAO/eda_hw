#ifndef FUNCTION_H
#define FUNCTION_H

#include "my_struct.h"

int calculateBend(const Node &firstNode);
int calculateCost(Node node, const std::map<std::string, int> &loss, std::map<Point, int> &gridUsageCount, const Net &net, const std::map<std::string, int> &ratio);
void AStar(Point dieArea, Net &net, const std::map<std::string, int> &loss, std::map<Point, int> &gridUsageCount, const std::map<std::string, int> &ratio);
void outputFile(std::string &outputFileName, std::vector<Net> &nets);

#endif