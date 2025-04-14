#include "gnuPlot.h"

void SetGPTital(ofstream &file) {
    file << "reset" << endl;
    file << "set title \"result\"" << endl;
    file << "set xlabel \"X\"" << endl;
    file << "set ylabel \"Y\"" << endl;
}

void SetGPSize(ofstream &file, int dieX, int dieY, const vector<Component> &components, int cellSize, int siteWidth, int siteHeight, const string &outputFileName) {
    int maxX = dieX;
    int maxY = dieY;

    for (const auto &c : components) {
        maxX = max(maxX, c.laterX + cellSize * siteWidth);
        maxY = max(maxY, c.laterY + siteHeight);
    }

    file << "set xrange [0:" << maxX + 50 << "]\n";
    file << "set yrange [0:" << maxY + 50 << "]\n";
    file << "plot 0 notitle\n";
    file << "set terminal pngcairo size 6000,4000 enhanced font 'Arial,10'\n";
    file << "set output \"" << outputFileName << "\"\n";
    file << "replot\n";
}


void WriteRows(ofstream &file, const vector<Row> &rows, int &objectIndex) {
    for (const auto &row : rows) {
        file << "set arrow from " << 0 << " , "
             << row.y << " to " << row.stepX * row.numX << "," << row.y << " nohead lw 2 lc rgb \"#D0D0D0\"\n";
    }
}

void WriteCells(ofstream &file, const vector<Component> &components, int &objectIndex, int cellSize, int siteWidth, int siteHeight, bool drawLegalized) {
    for (const auto &cell : components) {
        int x1 = drawLegalized ? cell.laterX : cell.originalX;
        int y1 = drawLegalized ? cell.laterY : cell.originalY;
        int x2 = x1 + cellSize * siteWidth;
        int y2 = y1 + siteHeight;

        string color = drawLegalized ? "#00CACA" : "#00CCCC";

        file << "set object " << objectIndex++ << " rect from "
             << x1 << "," << y1 << " to " << x2 << "," << y2
             << " lw 1 fs solid fc rgb \"" << color << "\"" << endl;

        // file << "set label \"{" << cell.instName << "}\" at "
        //      << (x1 + x2) / 2 << "," << (y1 + y2) / 2
        //      << " center font \",8\"" << endl;
    }
}

void DrawPlacement(const vector<Row> &rows, const vector<Component> &components,
                   const string &outputPath, int cellSize, int siteWidth, int siteHeight,
                   int dieX, int dieY) {
    ofstream file("draw_placement.gp");
    int objIndex = 1;

    SetGPTital(file);
    WriteRows(file, rows, objIndex);
    // WriteCells(file, components, objIndex, cellSize, siteWidth, siteHeight, false); // original
    WriteCells(file, components, objIndex, cellSize, siteWidth, siteHeight, true);  // legalized
    SetGPSize(file, dieX, dieY, components, cellSize, siteWidth, siteHeight, outputPath);

    file.close();
    system("gnuplot draw_placement.gp");
}
