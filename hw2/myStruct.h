#ifndef MY_STRUCT_H
#define MY_STRUCT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Row {
    string rowName;
    string siteName;
    int x, y;
    string orientation;
    int numX, numY;
    int stepX, stepY;

    Row(string rowName, string siteName,int x,int y, string orientation, int numX, int numY, int stepX, int stepY);
};

struct Component {
    string instName;
    string macroName;
    int originalX, originalY;
    int laterX, laterY;
    string orientation;

    Component(string instName, string macroName, int originalX, int originalY, string orientation);
};

void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &siteWidth, const int &siteHight) {};

#endif