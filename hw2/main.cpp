#include "myStruct.h"

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

    for (auto row : rows) {
        cout  << "ROW " << row.rowName << " " << row.siteName << " " << row.x << " " << row.y << " " << row.orientation << " DO " << row.numX << " BY " << row.numY << " STEP " << row.stepX << " " << row.stepY << " ;" << endl;
    }

    cout << endl;

    for (auto component : components) {
        cout << "- " << component.instName << " " << component.macroName << " + PLACED ( " << component.x << " " << component.y << " ) " << component.orientation << " ;" << endl;
    }
}