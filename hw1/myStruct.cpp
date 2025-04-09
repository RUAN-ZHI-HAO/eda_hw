#include "myStruct.h"

Component::Component(string name, int x1, int y1, int x2, int y2) : name(name), x1(x1), y1(y1), x2(x2), y2(y2) {}

SpecialNet::SpecialNet(string name, int x1, int y1, int x2, int y2) : name(name), x1(x1), y1(y1), x2(x2), y2(y2) {}


ChipDesign::ChipDesign(int width, int height) : componentWidth(width), componentHeight(height) {}
void ChipDesign::SetDieArea(int x2, int y2) {
    die.x2 = x2;
    die.y2 = y2;
}
void ChipDesign::AddComponents(const string &name, int x, int y) {
    components.emplace_back(name, x, y, x + componentWidth, y + componentHeight);
}
void ChipDesign::AddSpecialNets(const string &name, int x1, int y1, int x2, int y2) {
    specialNets.emplace_back(name, x1, y1, x2, y2);
}