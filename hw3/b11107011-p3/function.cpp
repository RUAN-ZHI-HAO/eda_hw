#include "function.h"

void AStar(Point dieArea, Net &net, const std::map<std::string, int> &loss, std::unordered_map<Point, int, PointHash> &gridUsageCount, const std::map<std::string, float> &ratio) {
    const int dx[4] = {1, 0, -1, 0};// right down left up
    const int dy[4] = {0, -1, 0, 1};

    gridUsageCount[net.src]--;
    gridUsageCount[net.dst]--;

    auto inside = [&](int x, int y) {
        return x >= 0 && x < dieArea.x && y >= 0 && y < dieArea.y;
    };

    auto hCost = [&](const Point &a, const Point &b) -> float {
        // 曼哈頓距離
        const int manhattan = std::abs(a.x - b.x) + std::abs(a.y - b.y);

        // 必要最少轉折 (同列/同行 = 0，否則 1)
        const int minBend = (a.x == b.x || a.y == b.y) ? 0 : 1;

        float h = manhattan * loss.at("propagation") * ratio.at("propagation")
                + minBend  * loss.at("bending")      * ratio.at("bending");

        // 全域縮放 (可用來 fine-tune search 範圍)
        return h * ratio.at("heuristic");
    };

    auto crossCost = [&](int x, int y) {
        return gridUsageCount[{x, y}] * loss.at("crossing") * ratio.at("crossing");
    };

    std::priority_queue<Node> pq;
    std::unordered_map<Point, Point, PointHash> cameFrom;
    std::unordered_map<Point, int, PointHash> bestG;
    std::unordered_set<Point, PointHash> closed;

    Node start{net.src, 0, hCost(net.src, net.dst), -1};
    pq.push(start);
    bestG[start.p] = 0;

    while (!pq.empty()) {
        Node cur = pq.top(); pq.pop();
        if (cur.p == net.dst) {
            Point v = net.dst;
            net.path.clear();
            net.path.push_back(v);
            while (v != net.src) {
                v = cameFrom[v];
                net.path.push_back(v);
            }
            for (const Point &pt : net.path) gridUsageCount[pt]++;
            return;
        }
        if (closed.count(cur.p)) continue;
        closed.insert(cur.p);

        for (int dir = 0; dir < 4; ++dir) {
            int nx = cur.p.x + dx[dir];
            int ny = cur.p.y + dy[dir];
            if (!inside(nx, ny)) continue;

            int addG = loss.at("propagation") * ratio.at("propagation") + crossCost(nx, ny);
            if (cur.dir != -1 && cur.dir != dir) addG += loss.at("bending") * ratio.at("bending");
            int ng = cur.g + addG;
            Point np{nx, ny};
            if (!bestG.count(np) || ng < bestG[np]) {
                bestG[np] = ng;
                float nf = ng + hCost(np, net.dst);
                pq.push({np, ng, nf, dir});
                cameFrom[np] = cur.p;
            }
        }
    }
    std::cerr << "Error: No path found for net " << net.index << std::endl;
}

void outputFile(std::string &outputFileName, std::vector<Net> &nets) {
    std::ofstream file(outputFileName);
    for (size_t i = 0; i < nets.size(); i++) {
        int pathSize = nets[i].path.size();
        file << nets[i].index << ' ' << pathSize - 1 << std::endl;
        for (int j = pathSize - 1; j >= 0; j--) {
            Point point = nets[i].path[j];
            if (j == 0) file << point.x << ' ' << point.y;
            else if (j == pathSize - 1) file << point.x << ' ' << point.y << ' ';
            else file << point.x << ' ' << point.y << '\n' << point.x << ' ' << point.y << ' ';
        }
        if (i != nets.size() - 1) file << std::endl;
    }
}

float calculateNetCost(Net &net, std::unordered_map<Point, int, PointHash> &gridUsageCount, std::map<std::string, int> &loss) {
    int propagationLoss = net.path.size() - 1;
    int crossingLoss = 0;
    int bendingLoss = 0;

    for (const auto point : net.path) {
        if (gridUsageCount[point] > 1) crossingLoss++;
    }

    Point firstPoint, secondPoint, thirdPoint;
    for (size_t i = 2; i < net.path.size(); i++) {
        thirdPoint = net.path[i];
        secondPoint = net.path[i - 1];
        firstPoint = net.path[i - 2];

        // 如果三點都在垂直或水平直線上，沒有彎折
        if ((firstPoint.x == secondPoint.x && secondPoint.x == thirdPoint.x) ||
            (firstPoint.y == secondPoint.y && secondPoint.y == thirdPoint.y)) {
            continue;
        }

        bendingLoss++;
    }

    return propagationLoss * loss["propagation"] + crossingLoss * loss["crossing"] + bendingLoss * loss["bending"];
}

