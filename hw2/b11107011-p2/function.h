#ifndef FUNCTION_H
#define FUNCTION_H

#include "myStruct.h"
#include <climits>

void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &cellSize, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY);
void FindCellBestPlace (vector<Component> &cells, Row row, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY);
vector<Component*> GatTopKDisplacedCells(vector<Component> &components, int count);
Cluster FormCluster(Component* center, vector<Component> &components, const vector<vector<bool>>& rowSites, int range = 3);
PermResult FindBestPermutation(const Cluster& cl);
void ApplyClusterPlacement(Cluster& cl, const PermResult& best);
void OutputDEF(string &outputFileName, string &designName, int &dieX, int dieY, vector<Row> rows, vector<Component> &cells);

#endif