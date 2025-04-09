#ifndef MY_STRUCT_H
#define MY_STRUCT_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


using namespace std; //超好用


struct DieArea {
    int x1 = 0, y1 = 0, x2, y2; //先左下再右上
};

struct Component {
    string name;
    int x1, y1, x2, y2; //先左下再右上

    Component(string name, int x1, int y1, int x2, int y2);
};

struct SpecialNet {
    string name;
    int x1, y1, x2, y2; // 先左下再右上

    SpecialNet(string name, int x1, int y1, int x2, int y2);
};

struct ChipDesign {
    DieArea die;
    vector<Component> components;
    vector<SpecialNet> specialNets;
    int componentWidth, componentHeight;
    
    void SetDieArea(int x2, int y2);
    void AddComponents(const string &name, int x, int y);
    void AddSpecialNets(const string &name, int x1, int y1, int x2, int y2);
    
    ChipDesign(int width, int height);
};

#endif