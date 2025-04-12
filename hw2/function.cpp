#include "function.h"

void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &cellSize, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY) {
    int bestDistance = INT_MAX;
    int bestXIndex = -1;
    int bestYIndex = -1;
    for (size_t i = 0; i < rowSites.size(); i++) {
        for (size_t j = 0; j < rowSites[i].size() - cellSize; j++) {
            bool canPlace = true;
            for (int k = 0; k < cellSize - 1; k++) {
                if (rowSites[i][j + k]) {
                    canPlace = false;
                    break;
                }
            }

            if (!canPlace) continue;
    
            int x = j * siteWidth;
            int y = i * siteHight;
            int distance = abs(cell.originalX - x) + abs(cell.originalY - y);
            if (distance < bestDistance) {
                bestXIndex = j;
                bestYIndex = i;
                bestDistance = distance;
            }
        }
    }
    if (bestXIndex != -1) {
        cell.laterX = bestXIndex * siteWidth;
        cell.laterY = bestYIndex * siteHight;
        cell.distance = abs(cell.laterX - cell.originalX) + abs(cell.laterY - cell.originalY);
        for (int i = 0; i < cellSize; i++) {
            rowSites[bestYIndex][bestXIndex + i] = true;
        }
    }
}

void FindCellBestPlace (vector<Component> &cells, Row row, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY) {
    cout <<  "siteWidth " << siteWidth << endl;
    for (auto &cell : cells) {
        int bestXIndex = (cell.originalX >= 0 && cell.originalX < dieX) ? cell.originalX / siteWidth : (cell.originalX < 0) ? 0 : (dieX - 1) / siteWidth;
        int bestYIndex = (cell.originalY >= 0 && cell.originalY < dieY) ? cell.originalY / siteHight : (cell.originalY < 0) ? 0 : (dieY - 1) / siteHight;
        cout << "cell name = " << cell.instName << " X = " << bestXIndex << " Y = " << bestYIndex << endl;
        
        cell.laterX = bestXIndex * siteWidth;
        cell.laterY = bestYIndex * siteHight;
        cell.distance = abs(cell.originalX - cell.laterX) + abs(cell.originalY - cell.laterY);
    }
}