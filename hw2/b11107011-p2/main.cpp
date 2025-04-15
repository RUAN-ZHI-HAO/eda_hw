#include "myStruct.h"
#include "function.h"
#include "gnuPlot.h"

int main(int argc, char* argv[]) {
    int cellSize = atoi(argv[1]);
    // float alpha = stof(argv[2]);
    ifstream inputFile(argv[3]);
    string outputFile(argv[4]);

    if (!inputFile) {
        cerr << "you need ChatGPT help -__-" << endl;
        return 1;
    }


    int dieX = 0;
    int dieY = 0;
    string designName;
    vector<Row> rows;
    vector<Component> components;

    string line;
    while (getline(inputFile, line)) {
        istringstream iss(line);
        string keyword;
        iss >> keyword;

        if (keyword == "DIEAREA") {
            string temp;
            iss >> temp >> temp >> temp >> temp >> temp >> dieX >> dieY;
        }
        else if (keyword == "ROW") {
            string temp;
            string rowName, siteName, orientation;
            int x, y, numX,numY,stepX, stepY;
            iss >> rowName >> siteName >> x >> y >> orientation >> temp >> numX >> temp >> numY >> temp >> stepX >> stepY;
            Row row(rowName, siteName, x, y, orientation, numX, numY, stepX, stepY);
            rows.push_back(row);
        }
        else if (keyword == "-") {
            string temp;
            string instName, macroName, orientation;
            int x, y;
            iss >> instName >> macroName >> temp >> temp >> temp >> x >> y >> temp >> orientation;
            Component component(instName, macroName, x, y, orientation);
            components.push_back(component);
        }
        else if (keyword == "DESIGN") {
            iss >> designName;
        }
    }

    FindCellBestPlace(components, rows[0],rows[0].stepX, dieY / rows.size(), dieX, dieY);
   
    sort(components.begin(), components.end(), [](const Component &a, const Component &b) {
        return a.distance > b.distance;
    });

    vector<vector<bool>> rowSites;
    for (const auto &row : rows) {
        rowSites.emplace_back(row.numX, false);
    }

    for (auto &cell : components) { //Greedy做完
        CellPlace(rowSites, cell, cellSize, rows[0].stepX, dieY / rows.size(),dieX, dieY);
    }

    int topKCount = min(6, (int)components.size());
    for (int i = 0; i < 50 ; i++) {
        vector<Component*> topK = GatTopKDisplacedCells(components, topKCount);
        for (Component* cell : topK) {
            Cluster cl = FormCluster(cell, components, rowSites, 5);
            PermResult best = FindBestPermutation(cl);
            if (best.maxDisp < cl.originalMaxDisp)
                ApplyClusterPlacement(cl, best);
        }
    }

    //計算得分
    // int maxDistance = -1;
    // int64_t averageDistance = 0;
    // for (const auto &cell : components) {
    //     if (cell.distance > maxDistance) maxDistance = cell.distance;
    //     averageDistance += cell.distance;
    // }
    // averageDistance /= components.size();
    // cout << "average distance = " << averageDistance << " max distance = " << maxDistance << " total = " << averageDistance + alpha * maxDistance << endl;

    //畫出布局圖
    // DrawPlacement(rows, components, "placement.png", cellSize, rows[0].stepX, dieY / rows.size(), dieX, dieY);

    OutputDEF(outputFile, designName, dieX, dieY, rows, components);
}