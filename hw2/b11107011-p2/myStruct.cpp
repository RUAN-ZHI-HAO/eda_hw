#include "myStruct.h"

Row::Row(string rowName, string siteName,int x,int y, string orientation, int numX, int numY, int stepX, int stepY) : rowName(rowName), siteName(siteName), x(x), y(y), orientation(orientation), numX(numX), numY(numY), stepX(stepX), stepY(stepY) {};
Component::Component(string instName, string macroName, int originalX, int originalY, string orientation) : instName(instName), macroName(macroName), originalX(originalX), originalY(originalY), laterX(0), laterY(0), orientation(orientation), distance(0) {};
