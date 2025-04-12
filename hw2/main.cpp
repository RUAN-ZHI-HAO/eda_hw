#include "myStruct.h"
#include "function.h"
#include "gnuPlot.h"

int main(int argc, char* argv[]) {
    int cellSize = atoi(argv[1]);
    int alpha = atoi(argv[2]);
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



    vector<vector<bool>> rowSites;
    for (const auto &row : rows) {
        rowSites.emplace_back(row.numX, false);
    }
    // for (auto &row : rows) {
    //     cout  << "ROW " << row.rowName << " " << row.siteName << " " << row.x << " " << row.y << " " << row.orientation << " DO " << row.numX << " BY " << row.numY << " STEP " << row.stepX << " " << row.stepY << " ;" << endl;
    // }

    // cout << endl;

    for (auto &component : components) {
        cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.originalX << " " << component.originalY << " ) " << component.orientation  << " distance = " << component.distance << " ;" << endl;
    }

    cout << endl;

    for (auto &cell : components) {
        CellPlace(rowSites, cell, cellSize, rows[0].stepX, dieY / rows.size(),dieX, dieY);
    }

    for (auto &component : components) {
        cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.originalX << " " << component.originalY << " ) " << component.orientation  << " distance = " << component.distance << " laterX = " << component.laterX << " laterY = " << component.laterY << " ;" << endl;
    }

    // sort(components.begin(), components.end(), [](const Component &a, const Component &b) { //排序左到右、下到上
    //     if (a.originalX != b.originalX) return a.originalX < b.originalX;
    //     return a.originalY < b.originalY;
    // });

    DrawPlacement(rows, components, "placement.png", cellSize, rows[0].stepX, dieY / rows.size(), dieX, dieY);
}