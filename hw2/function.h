#ifndef FUNCTION_H
#define FUNCTION_H

#include "myStruct.h"
#include <climits>

void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &cellSize, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY);
void FindCellBestPlace (vector<Component> &cells, Row row, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY);
#endif