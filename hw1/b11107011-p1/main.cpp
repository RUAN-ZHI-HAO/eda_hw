#include "myStruct.h"
#include "gnuPlot.h"

int main(int argc, char* argv[]) { // Ex: ./genPlot 7100 6600 input.txt output.gp
    ifstream inputFile(argv[3]);

    string outputFileName = findCS(argv[3]);

    if (!inputFile) {
        cerr << "you need ChatGPT help -__-" << endl;
        return 1;
    }

    string line;
    ChipDesign chip(atoi(argv[1]), atoi(argv[2]));

    while (getline(inputFile, line)) {
        istringstream iss1(line);
        string keyword;
        iss1 >> keyword;

        if (keyword == "DIEAREA") { // DIEAREA ( 0 0 ) ( 82970 80070 ) ;
            int x, y;
            string temp;
            iss1 >> temp >> temp >> temp >> temp >> temp >> x >> y;
            chip.SetDieArea(x, y);
        }
        else if (keyword == "-") {
            string name;
            iss1 >> name;
            name = idiotSubscript(name);

            getline(inputFile, line);
            istringstream iss2(line);
            string temp;
            iss2 >> temp >> temp; // 判斷是Net or Transistor

            if (temp == "PLACED") { // Transistor
                int x, y;
                iss2 >> temp >> x >> y;
                chip.AddComponents(name, x, y);
            }
            else { // Net
                int x1, y1, x2, y2, width;
                iss2 >> temp >> width >> temp >> x1 >> y1 >> temp >> temp;
                if (iss2 >> x2) {
                    y2 = y1;
                    y1 -= width / 2;
                    y2 += width / 2;
                }
                else {
                    iss2.clear(); // 清空錯誤訊息
                    iss2.ignore(1); // 跳過一個字元
                    iss2 >> y2;
                    x2 = x1;
                    x1 -= width / 2;
                    x2 += width / 2;
                }
                chip.AddSpecialNets(name, x1, y1, x2, y2);
            }
        }
    }

    inputFile.close();

    // cout << "Die Area = ( " << chip.die.x1 << " " << chip.die.y1 << " ) ( " << chip.die.x2 << " " << chip.die.y2 << " )" << endl;
    // cout << endl;
    // for (const auto &component : chip.components) {
    //     cout << component.name << " ( " << component.x1 << " " << component.y1 << " ) ( " << component.x2 << " " << component.y2 << " )" << endl;
    // }
    // cout << endl;
    // for (const auto &specialNet : chip.specialNets) {
    //     cout << specialNet.name << " ( " << specialNet.x1 << " " << specialNet.y1 << " ) ( " << specialNet.x2 << " " << specialNet.y2 << " )" << endl;
    // }

    ofstream outputFile(argv[4]);
    int objectIndex = 1;

    if (!outputFile) {
        cerr << "you need ChatGPT help -__-" << endl;
        return 1;
    }
    SetGPTital(outputFile);
    WriteComponents(outputFile, chip.components, objectIndex);
    WriteSpecialNets(outputFile, chip.specialNets, objectIndex, chip.die.y2);
    SetGPSize(outputFile, chip, outputFileName);

    outputFile.close();
}