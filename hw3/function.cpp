#include "function.h"

Point secondNode;
Point thirdNode;

int calculateBend(const Node &node) {
    Point a = node.point;
    Point b = node.firstSource;
    Point c = node.secondSource;

    if (a.x == -1 || c.x == -1) return 0;

    // 如果三點都在垂直或水平直線上，沒有彎折
    if ((a.x == b.x && b.x == c.x) ||
        (a.y == b.y && b.y == c.y)) {
        return 0;
    }
    return 1;
}


int calculateCost(Node node, const std::map<std::string, int> &loss, std::map<Point, int> &gridUsageCount, const Net &net, const std::map<std::string, int> &ratio) {
    int srcManhDist = std::abs(node.point.x - net.src.x) + std::abs(node.point.y - net.src.y);
    int dstManhDist = std::abs(node.point.x - net.dst.x) + std::abs(node.point.y - net.dst.y);
    
    // Can use loss["propagation"] only if loss is not const
    int distLose = (srcManhDist + dstManhDist) * loss.at("propagation") * ratio.at("propagation");
    int crossingLose = gridUsageCount[{node.point.x, node.point.y}] * loss.at("crossing") * ratio.at("crossing");
    int bendingLose = calculateBend(node) * loss.at("bending") * ratio.at("bending");

    return distLose + crossingLose + bendingLose;
}

void AStar(Point dieArea, Net &net, const std::map<std::string, int> &loss, std::map<Point, int> &gridUsageCount, const std::map<std::string, int> &ratio) {
    gridUsageCount[net.src]--;
    gridUsageCount[net.dst]--;
    
    std::priority_queue<Node> nodes;
    std::map<Point, bool> gridUsage;
    std::map<Point, Point> cameFrom; // <dst src>

    Node currentNode(net.src);
    gridUsage[currentNode.point] = true;
    // currentNode.loss = calculateCost(currentNode, loss, gridUsageCount, net, ratio);

    while(currentNode.point != net.dst) {
        // std::cout << "原本的node " << currentNode.point.x << ' ' << currentNode.point.y << std::endl;
        Node leftNode({currentNode.point.x - 1, currentNode.point.y});
        Node downNode({currentNode.point.x, currentNode.point.y - 1});
        Node rightNode({currentNode.point.x + 1, currentNode.point.y});
        Node upNode({currentNode.point.x, currentNode.point.y + 1});
        if (leftNode.point.x >= 0 && !gridUsage[leftNode.point]) {
            cameFrom[leftNode.point] = currentNode.point;
            leftNode.firstSource = currentNode.point;
            leftNode.secondSource = currentNode.firstSource;
            gridUsage[leftNode.point] = true;
            leftNode.loss = calculateCost(leftNode, loss, gridUsageCount, net, ratio);
            
            // std::cout << "leftNode.firstSource = " << leftNode.firstSource.x << ' ' << leftNode.firstSource.y << std::endl;
            // std::cout << "左邊的node " << leftNode.point.x << ' ' << leftNode.point.y << " loss = "  << leftNode.loss << std::endl;
            nodes.push(leftNode);
        }
        if (downNode.point.y >= 0 && !gridUsage[downNode.point]) {
            cameFrom[downNode.point] = currentNode.point;
            downNode.firstSource = currentNode.point;
            downNode.secondSource = currentNode.firstSource;
            gridUsage[downNode.point] = true;
            downNode.loss = calculateCost(downNode, loss, gridUsageCount, net, ratio);

            // std::cout << "下面的node " << downNode.point.x << ' ' << downNode.point.y << " loss = "  << downNode.loss << std::endl;
            nodes.push(downNode);
        }
        if (rightNode.point.x < dieArea.x && !gridUsage[rightNode.point]) {
            cameFrom[rightNode.point] = currentNode.point;
            rightNode.firstSource = currentNode.point;
            rightNode.secondSource = currentNode.firstSource;
            gridUsage[rightNode.point] = true;
            rightNode.loss = calculateCost(rightNode, loss, gridUsageCount, net, ratio);

            // std::cout << "rightNode.firstSource = " << rightNode.firstSource.x << ' ' << rightNode.firstSource.y << std::endl;
            // std::cout << "右邊的node " << rightNode.point.x << ' ' << rightNode.point.y << " loss = "  << rightNode.loss << std::endl;
            nodes.push(rightNode);
        }
        if (upNode.point.y < dieArea.y && !gridUsage[upNode.point]) {
            cameFrom[upNode.point] = currentNode.point;
            upNode.firstSource = currentNode.point;
            upNode.secondSource = currentNode.firstSource;
            gridUsage[upNode.point] = true;
            upNode.loss = calculateCost(upNode, loss, gridUsageCount, net, ratio);

            // std::cout << "上面的node " << upNode.point.x << ' ' << upNode.point.y << " loss = "  << upNode.loss << std::endl;
            nodes.push(upNode);
        }

        currentNode = nodes.top();
        // std::cout << "後來選擇的node = " << currentNode.point.x << ' ' << currentNode.point.y << " loss = " << currentNode.loss << std::endl;
        nodes.pop();
        // std::cout << std::endl;
    }
    gridUsageCount[net.src]++;
    gridUsageCount[net.dst]++;
    Point dstPoint = currentNode.point;
    net.path.push_back(currentNode.point);
    while (dstPoint != net.src) {
        Point srcPoint = cameFrom[dstPoint];
        net.path.push_back(srcPoint);
        gridUsageCount[srcPoint]++;
        dstPoint = srcPoint;
    }



    // Point pp = currentNode.point;
    // while (pp != net.src) {
    //     Point prev = cameFrom[pp];
    //     std::cout << pp.x << ' ' << pp.y << " <- " << prev.x << ' ' << prev.y << '\n';
    //     pp = prev;
    // }
    // std::cout << net.src.x << ' ' << net.src.y << " (start)\n";

}

void outputFile(std::string &outputFileName, std::vector<Net> &nets) {
    std::ofstream file(outputFileName);
    for (size_t i = 0; i < nets.size(); i++) {
        int pathSize = nets[i].path.size();
        file << i << ' ' << pathSize - 1 << std::endl;
        for (int j = pathSize - 1; j >= 0; j--) {
            Point point = nets[i].path[j];
            if (j == 0) file << point.x << ' ' << point.y;
            else if (j == pathSize - 1) file << point.x << ' ' << point.y << ' ';
            else file << point.x << ' ' << point.y << '\n' << point.x << ' ' << point.y << ' ';
        }
        if (i != nets.size() - 1) file << std::endl;
    }
}
