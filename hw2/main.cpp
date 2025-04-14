#include "myStruct.h"
#include "function.h"
#include "gnuPlot.h"

int main(int argc, char* argv[]) {
    int cellSize = atoi(argv[1]);
    float alpha = stof(argv[2]);
    ifstream inputFile(argv[3]);
    ofstream outputFile(argv[4]);

    if (!inputFile) {
        cerr << "you need ChatGPT help -__-" << endl;
        return 1;
    }


    int dieX = 0;
    int dieY = 0;
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
    }

    FindCellBestPlace(components, rows[0],rows[0].stepX, dieY / rows.size(), dieX, dieY);
   
    sort(components.begin(), components.end(), [](const Component &a, const Component &b) {
        return a.distance > b.distance;
    });

    cout << components[0].distance << endl;

    // sort(components.begin(), components.end(), [](const Component &a, const Component &b) { //排序左到右、下到上
    //     if (a.originalX != b.originalX) return a.originalX < b.originalX;
    //     return a.originalY < b.originalY;
    // });

    vector<vector<bool>> rowSites;
    for (const auto &row : rows) {
        rowSites.emplace_back(row.numX, false);
    }

    // for (auto &row : rows) {
    //     cout  << "ROW " << row.rowName << " " << row.siteName << " " << row.x << " " << row.y << " " << row.orientation << " DO " << row.numX << " BY " << row.numY << " STEP " << row.stepX << " " << row.stepY << " ;" << endl;
    // }

    // cout << endl;

    // for (auto &component : components) {
    //     cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.originalX << " " << component.originalY << " ) " << component.orientation  << " distance = " << component.distance << " ;" << endl;
    // }

    // cout << endl;

    // for (auto &component : components) {
    //     cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.originalX << " " << component.originalY << " ) " << component.orientation  << " distance = " << component.distance << " laterX = " << component.laterX << " laterY = " << component.laterY << " ;" << endl;
    // }

    for (auto &cell : components) { //Greedy做完
        CellPlace(rowSites, cell, cellSize, rows[0].stepX, dieY / rows.size(),dieX, dieY);
    }

    sort(components.begin(), components.end(), [](const Component &a, const Component &b) {
        return a.distance > b.distance;
    });

    vector<int> befor;
    for (const auto &cell : components) {
        befor.push_back(cell.distance);
    }

    int topKCount = components.size() * 0.1;
    vector<Component*> topK = GatTopKDisplacedCells(components, topKCount);

    for (Component* cell : topK) {
        Cluster cl = FormCluster(cell, components, rowSites, 7);
        PermResult best = FindBestPermutation(cl);
        if (best.maxDisp < cl.originalMaxDisp)
            // cout << "真的有優化" << endl;
            ApplyClusterPlacement(cl, best);
    }

    vector<int> after;
    for (const auto &cell : components) {
        after.push_back(cell.distance);
    }

    for (int i = 0; i < befor.size(); i++) {
        if (befor[i] != after[i]) cout << "這邊有不一樣 原本 = " << befor[i] << " 之後 " << after[i] << endl;
    }

    // cout << endl;
    // for (auto &component : components) {
    //     cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.originalX << " " << component.originalY << " ) " << component.orientation  << " distance = " << component.distance << " laterX = " << component.laterX << " laterY = " << component.laterY << " ;" << endl;
    // }

    int maxDistance = -1;
    int64_t averageDistance = 0;
    for (const auto &cell : components) {
        if (cell.distance > maxDistance) maxDistance = cell.distance;
        averageDistance += cell.distance;
    }
    averageDistance /= components.size();
    cout << "average distance = " << averageDistance << " max distance = " << maxDistance << " total = " << averageDistance + alpha * maxDistance << endl;

    DrawPlacement(rows, components, "placement.png", cellSize, rows[0].stepX, dieY / rows.size(), dieX, dieY);
}