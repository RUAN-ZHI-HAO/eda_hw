#include "function.h"

int calculateCost(const Point &point, const std::map<std::string, int> &loss, std::map<Point, int> &gridUsageCount, const Net &net) {
    int srcManhDist = std::abs(point.x - net.src.x) + std::abs(point.y - net.src.y);
    int dstManhDist = std::abs(point.x - net.dst.x) + std::abs(point.y - net.dst.y);
    // Can use loss["propagation"] only if loss is not const
    srcManhDist *= loss.at("propagation");
    dstManhDist *= loss.at("propagation");

    int distLose = srcManhDist + dstManhDist;
    int crossingLose = gridUsageCount[{point.x, point.y}] * loss.at("crossing");
    

    return 0;
}