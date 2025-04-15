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
    }
    else {
        cout << "這個問題無法解決" << endl;
    }
}

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
    int end = min((int)sameRow.size() - 1, centerIdx + range);
    for (int i = start; i <= end; i++) {
        cl.cells.push_back(sameRow[i]);
        cl.sites.push_back(sameRow[i]->laterX);
        cl.originalMaxDisp = max(cl.originalMaxDisp, sameRow[i]->distance);
    }
    return cl;
}

PermResult FindBestPermutation(const Cluster& cl) {
    PermResult best;
    best.maxDisp = INT_MAX;

    auto cells = cl.cells;

    do {
        int curMax = 0;
        for (size_t i = 0; i < cells.size(); ++i) {
            int newX = cl.sites[i];
            int dist = abs(cells[i]->originalX - newX);
            curMax = max(curMax, dist);
        }
        if (curMax < best.maxDisp) {
            best.order = cells;
            best.maxDisp = curMax;
        }
    } while (next_permutation(cells.begin(), cells.end()));
    return best;
}

void ApplyClusterPlacement(Cluster& cl, const PermResult& best) {
    for (size_t i = 0; i < best.order.size(); ++i) {
        Component* cell = best.order[i];
        int newX = cl.sites[i];  // 注意：site 順序對應 cell 排序
        cell->laterX = newX;
        cell->distance = abs(cell->originalX - newX) + abs(cell->originalY - cell->laterY);
    }
}

void OutputDEF(string &outputFileName, string &designName, int &dieX, int dieY, vector<Row> rows, vector<Component> &cells) {
    ofstream file(outputFileName + ".def");
    int rowHight = rows[1].y;
    string even = rows[0].orientation;
    string odd = rows[1].orientation;

    file << "VERSION 5.8 ;" << endl;
    file << "DIVIDERCHAR \"/\" ;" << endl;
    file << "BUSBITCHARS \"[]\" ;" << endl;
    file << "DESIGN " << designName << " ;" << endl;
    file << "UNITS DISTANCE MICRONS 1000 ;\n" << endl;
    file << "DIEAREA (0 0) ( " << dieX << " " << dieY << " );" << endl;
    for (const auto &row : rows) {
        file << "ROW " << row.rowName << ' ' << row.siteName << ' ' << 
        row.x << ' ' << row.y << ' ' << row.orientation << " DO " << 
        row.numX << " BY " << row.numY << " STEP " << row.stepX << ' ' << 
        row.stepY << ';' << endl;
    }
    file << "\nCOMPONENTS " << cells.size() << " ;" << endl;
    for (const auto &cell : cells) {
        file << "- " << cell.instName << ' ' << cell.macroName << " + PLACED ( " << cell.laterX << ' ' << cell.laterY << " ) " ;
        if ((cell.laterY / rowHight) % 2 == 0) {
            file << even << " ;" << endl;
        }
        else {
            file << odd << " ;" << endl;
        }
    }
    file << "END COMPONENTS\n" << endl;
    file << "END DESIGN";
}
