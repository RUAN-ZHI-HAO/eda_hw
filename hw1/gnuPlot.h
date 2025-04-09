#ifndef GNUPLOT_H
#define GNUPLOT_H

#include "myStruct.h"

void SetGPTital(ofstream &file);
void WriteComponents(ofstream &file, vector<Component> components, int &objectIndex);
void WriteSpecialNets(ofstream &file, vector<SpecialNet> specialNets, int &objectIndex, int &dieAreaY2);
void SetGPSize(ofstream &file, ChipDesign &chip,const string &outputFileName);
string findCS(const string &path);
string idiotSubscript(const string &str);
#endif