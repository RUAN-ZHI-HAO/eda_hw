#include "gnuPlot.h"
#include <regex>


void SetGPTital(ofstream &file) {
    file << "reset" << endl;
    file << "set title \"result\"" << endl;
    file << "set xlabel \"X\"" << endl;
    file << "set ylabel \"Y\"" << endl;
}

void WriteComponents(ofstream &file, vector<Component> components, int &objectIndex) {
    for (const auto &component : components) {
        file << "set object " << objectIndex++ << " rect from " << component.x1 << "," << component.y1 << " to " << component.x2 << "," << component.y2 << " lw 1 fs solid fc rgb \"#00CACA\"" << endl;
        file << "set label \"{" << component.name << "}\" at " << (component.x1 + component.x2) / 2 << "," << (component.y1 + component.y2) / 2 << " center font \",8\"" << endl;
    }
}

void WriteSpecialNets(ofstream &file, vector<SpecialNet> specialNets, int &objectIndex, int &dieAreaY2) {
    for (const auto &specialNet : specialNets) {
        if (specialNet.name.find("Metal3")) {
            file << "set object " << objectIndex++ << " rect from " << specialNet.x1 << "," << specialNet.y1 << " to " << specialNet.x2 << "," << specialNet.y2 << " lw 1 fs solid fc rgb \"#1AFD9C\"" << endl;
            file << "set label \"{" << specialNet.name << "}\" at " << (specialNet.x1 + specialNet.x2) / 2 << "," << (specialNet.y1 + specialNet.y2) / 2 << " center font \",6\"" << endl;
        }
        else {
            file << "set object " << objectIndex++ << " rect from " << specialNet.x1 << "," << specialNet.y1 << " to " << specialNet.x2 << "," << specialNet.y2 << " lw 1 fs solid fc rgb \"#c38b13\"" << endl;
            file << "set label \"{" << specialNet.name << "}\" at " << (specialNet.x1 + specialNet.x2) / 2 << "," << dieAreaY2 << " rotate by 270 font \",6\"" << endl;
        }
    }
}

void SetGPSize(ofstream &file, ChipDesign &chip,const string &outputFileName) {
    file << "set label 1 \"Die size: " << chip.die.x2 << 'x' << chip.die.y2 << "\" at screen 0.1, 0.01 font \",12\"" << endl;
    file << "set xtics auto" << endl;
    file << "set ytics auto" << endl;
    file << "plot [0:" << chip.die.x2 << "][0:" << chip.die.y2 << "] 0 with lines" << endl;
    file << "set terminal png size 1024,768" << endl;
    file << "set output \"" << outputFileName << "\"" << endl;
    file << "replot";
}

string findCS(const string &path) {
    regex pattern("CS[0-9]+x[0-9]+");
    smatch match;
    if (regex_search(path, match, pattern)) {
        return match.str() + ".png";
    }
    return "output.png";
}

string idiotSubscript(const string &str) {
    string goodString;
    for (const auto ch : str) {
        if (ch == '_') {
            goodString += "\\\\";
        }
        goodString += ch;
    }
    return goodString;
}