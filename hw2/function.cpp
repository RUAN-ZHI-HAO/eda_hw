#include "function.h"

void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &cellSize, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY) {
    int bestDistance = INT_MAX;
    int bestXIndex = -1;
    int bestYIndex = -1;
    for (size_t i = 0; i < rowSites.size(); i++) {
        for (size_t j = 0; j < rowSites[i].size() - cellSize + 1; j++) {
            bool canPlace = true;
            for (int k = 0; k <= cellSize - 1; k++) {
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
        // cout << "成功" << endl;
    }
    else {
        cout << "找不到" << endl;
    }
}

// void CellPlace(vector<vector<bool>> &rowSites, Component &cell, const int &cellSize,
//     const int &siteWidth, const int &siteHeight, const int &dieX, const int &dieY) {
// // 以預估位置作為擴展中心
// int bestXIndex = min((int)(cell.laterX / siteWidth), (int)rowSites[0].size() - 1);
// int bestYIndex = min((int)(cell.laterY / siteHeight), (int)rowSites.size() - 1);

// const int maxWindowSize = 101;
// int bestX = -1, bestY = -1, bestDistance = INT_MAX;

// for (int radius = 1; radius <= maxWindowSize; radius += 2) {
// for (int dy = -radius; dy <= radius; ++dy) {
//  for (int dx = -radius; dx <= radius; ++dx) {
//      int i = bestYIndex + dy;
//      int j = bestXIndex + dx;

//      if (i < 0 || i >= (int)rowSites.size()) continue;
//      if (j < 0 || j + cellSize > (int)rowSites[i].size()) continue;

//      // 檢查 cellSize 範圍是否全為空
//      bool canPlace = true;
//      for (int k = 0; k < cellSize; ++k) {
//          if (rowSites[i][j + k]) {
//              canPlace = false;
//              break;
//          }
//      }

//      if (!canPlace) continue;

//      int x = j * siteWidth;
//      int y = i * siteHeight;
//      int dist = abs(cell.originalX - x) + abs(cell.originalY - y);

//      if (dist < bestDistance) {
//          bestX = j;
//          bestY = i;
//          bestDistance = dist;
//      }
//  }
// }

// // 找到合法位置就不再擴展半徑
// if (bestX != -1) break;
// }

// // 更新 cell 狀態
// if (bestX != -1) {
// cell.laterX = bestX * siteWidth;
// cell.laterY = bestY * siteHeight;
// cell.distance = bestDistance;
// for (int k = 0; k < cellSize; ++k) {
//  rowSites[bestY][bestX + k] = true;
// }
// } else {
// cout << "⚠️ 找不到合法位置: " << cell.instName << endl;
// }
// }


void FindCellBestPlace (vector<Component> &cells, Row row, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY) {
    for (auto &cell : cells) {
        int bestXIndex = (cell.originalX >= 0 && cell.originalX < dieX) ? cell.originalX / siteWidth : (cell.originalX < 0) ? 0 : (dieX - 1) / siteWidth;
        int bestYIndex = (cell.originalY >= 0 && cell.originalY < dieY) ? cell.originalY / siteHight : (cell.originalY < 0) ? 0 : (dieY - 1) / siteHight;
        
        cell.laterX = bestXIndex * siteWidth;
        cell.laterY = bestYIndex * siteHight;
        cell.distance = abs(cell.originalX - cell.laterX) + abs(cell.originalY - cell.laterY);
    }
}

vector<Component*> GatTopKDisplacedCells(vector<Component> &components, int count) {
    sort(components.begin(), components.end(), [](const Component &a, const Component &b) {
        return a.distance > b.distance;
    });

    vector<Component*> result;
    for (int i = 0; i < count; i++) {
        result.push_back(&components[i]);
    }
    return result;
}

Cluster FormCluster(Component* center, vector<Component> &components, const vector<vector<bool>>& rowSites, int range) {
    Cluster cl;
    cl.originalMaxDisp = center->distance;

    vector<Component*> sameRow;
    for (auto &cell : components) {
        if (cell.laterY == center->laterY) sameRow.push_back(&cell);
    }

    sort(sameRow.begin(), sameRow.end(), [](Component *a, Component *b) {
        return a->laterX < b->laterX;
    });

    int centerIdx = -1;
    for (size_t i = 0; i < sameRow.size(); i++) {
        if (sameRow[i] == center) {
            centerIdx = i;
            break;
        }
    }

    if (centerIdx == -1) return cl;

    int start = max(0, centerIdx - range);
    int end = min((int)sameRow.size(), centerIdx + range);
    // cout << "start = " << start << " end = " << end << endl;
    for (int i = start; i <= end; i++) {
        cl.cells.push_back(sameRow[i]);
        cl.sites.push_back(sameRow[i]->laterX);
        cl.originalMaxDisp = max(cl.originalMaxDisp, sameRow[i]->distance);
    }
    // cout << "cl size = " << cl.cells.size() << endl;
    return cl;
}

PermResult FindBestPermutation(const Cluster& cl) {
    PermResult best;
    best.maxDisp = INT_MAX;
    // cout << "cl.cells size = " << cl.cells.size() << endl;
    auto cells = cl.cells;
    // cout << "cells size = " << cells.size() << endl;

    do {
        int curMax = 0;
        for (size_t i = 0; i < cells.size(); ++i) {
            int newX = cl.sites[i];
            int dist = abs(cells[i]->originalX - newX);
            curMax = max(curMax, dist);
            // cout << "curMax = " << curMax << endl;
        }
        if (curMax < best.maxDisp) {
            // if (best.maxDisp != INT_MAX) cout << "真的有不一樣" << endl;
            best.order = cells;
            best.maxDisp = curMax;
        }
    } while (next_permutation(cells.begin(), cells.end()));
    // cout << endl;
    return best;
}

void ApplyClusterPlacement(Cluster& cl, const PermResult& best) {
    for (size_t i = 0; i < best.order.size(); ++i) {
        Component* cell = best.order[i];
        int newX = cl.sites[i];  // 注意：site 順序對應 cell 排序
        cell->laterX = newX;
        // ✅ 正確更新 displacement
        // if (cell->distance != abs(cell->originalX - newX) + abs(cell->originalY - cell->laterY)) cout << "befor = " << cell->distance << " after = " << abs(cell->originalX - newX) + abs(cell->originalY - cell->laterY);
        cell->distance = abs(cell->originalX - newX) + abs(cell->originalY - cell->laterY);
    }
}
