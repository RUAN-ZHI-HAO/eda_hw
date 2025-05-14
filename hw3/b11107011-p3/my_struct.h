#ifndef MY_STRUCT_H
#define MY_STRUCT_H

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include <climits>
#include <unordered_map>
#include <unordered_set>

struct Point {
    int x, y;
    // 加上這個才能當作 std::map 的 key 使用
    bool operator<(const Point& other) const {
        return (x < other.x) || (x == other.x && y < other.y);
    }

    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    bool operator!=(const Point& other) const {
        return !(*this == other);
    }

    Point();
    Point(int x ,int y);
};

struct Net {
    int index;
    Point src, dst;
    float loss;
    std::vector<Point> path;

    Net(int index, int srcX, int srcY, int dstX, int dstY);
};

struct Node {
        Point p;
        int g = 0;
        float  f = 0;
        int dir = -1;
        bool operator<(const Node &o) const { return f > o.f; }
    };

struct PointHash {
    std::size_t operator()(const Point& p) const noexcept {
        return (static_cast<std::size_t>(p.x) << 20) ^ p.y;
    }
};

#endif