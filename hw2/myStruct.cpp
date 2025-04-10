#include "myStruct.h"

Row::Row(string rowName, string siteName,int x,int y, string orientation, int numX, int numY, int stepX, int stepY) : rowName(rowName), siteName(siteName), x(x), y(y), orientation(orientation), numX(numX), numY(numY), stepX(stepX), stepY(stepY) {};
Component::Component(string instName, string macroName, int originalX, int originalY, string orientation) : instName(instName), macroName(macroName), originalX(originalX), originalY(originalY), laterX(0), laterY(0), orientation(orientation) {};
void CellPlace (vector<vector<bool>> &rowSites, Component &cell, const int &siteWidth, const int &siteHight, const int &dieX, const int &dieY) {
    int originalX = (cell.originalX >= 0 && cell.originalX < dieX) ? cell.originalX / siteWidth : (cell.originalX < 0) ? 0 : dieX;
    int originalY = (cell.originalY >= 0 && cell.originalY < dieY) ? cell.originalY / siteWidth : (cell.originalY < 0) ? 0 : dieY;
    while(1) {
        int 
    }
};