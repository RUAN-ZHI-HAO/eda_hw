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
    std::vector<Point> path;

    Net(int index, int srcX, int srcY, int dstX, int dstY);
};

struct Node {
    Point point;
    Point firstSource;
    Point secondSource;
    int loss;

    bool operator<(const Node& other) const {
        return loss  > other.loss;
    }

    Node(Point point, Point firstSource = Point(), Point secondSource = Point(), int loss = 0);
};


#endif