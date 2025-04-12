#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "myStruct.h"

void SetGPTital(ofstream &file);
void SetGPSize(ofstream &file, int dieX, int dieY, const vector<Component> &components, int cellSize, int siteWidth, int siteHeight, const string &outputFileName);

void DrawPlacement(const vector<Row> &rows, const vector<Component> &components,
                   const string &outputPath, int cellSize, int siteWidth, int siteHeight,
                   int dieX, int dieY);

#endif