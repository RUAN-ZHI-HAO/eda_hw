#include "my_struct.h"
#include "function.h"

int main(int argc, char* argv[]) {
    std::ifstream inputFile(argv[1]);
    std::string outputFileName = argv[2];

    if (!inputFile) {
        std::cerr << "you need ChatGPT help -__-" << std::endl;
        return 1;
    }

    Point dieArea;
    std::map<std::string, int> loss;
    std::vector<Net> nets;

    std::string line;
    while(getline(inputFile, line)) {
        std::istringstream iss(line);
        std::string keyword;
        iss >> keyword;

        if (keyword == "grid") {
            int x, y;
            iss >> x >> y;
            dieArea.x = x;
            dieArea.y = y;
        }
        else if (keyword == "propagation") {
            std::string temp;
            int propagation = 0;
            iss >> temp >> propagation;
            loss["propagation"] = propagation;
        }
        else if (keyword == "crossing") {
            std::string temp;
            int crossing = 0;
            iss >> temp >> crossing;
            loss["crossing"] = crossing;
        }
        else if (keyword == "bending") {
            std::string temp;
            int bending = 0;
            iss >> temp >> bending;
            loss["bending"] = bending;
        }
        else if (keyword == "num") {
            int count = 0;
            std::string temp;
            iss >> temp >> count;
            for (int i = 0; i < count; i++) {
                getline(inputFile, line);
                std::istringstream iss(line);
                int index, srcX, srcY, dstX, dstY;
                iss >> index >> srcX >> srcY >> dstX >> dstY;
                Net net(index, srcX, srcY, dstX, dstY);
                nets.push_back(net);
            }
        }
    }

    std::cout << "dieArea = (" << dieArea.x << ", " << dieArea.y << ")" << std::endl;
    std::cout << "propagation = " << loss["propagation"] << " crossing = " << loss["crossing"] << " bending = " << loss["bending"] << std::endl;
    for (const auto &net : nets) {
        std::cout << "index = " << net.index << " src = (" << net.src.x << ", " << net.src.y << ") dst = (" << net.dst.x << ", " << net.dst.y << ")" << std::endl;
    }
    std::cout << std::endl;

    std::map<Point, int> gridUsageCount;
    std::map<std::string, int> ratio = {{"propagation", 1}, {"crossing", 1}, {"bending", 1}};
    for (const auto &net : nets) {
        gridUsageCount[net.src]++;
        gridUsageCount[net.dst]++;
    }
    for (size_t i = 0; i < nets.size(); i++) {
        // std::cout << "i = " << i << std::endl;
        AStar(dieArea, nets[i], loss, gridUsageCount, ratio);
        nets[i].loss = calculateNetCost(nets[i], gridUsageCount, loss);
        // std::cout << "net " << i << " loss = " << nets[i].loss << std::endl;
        // for (const auto point : nets[i].path) {
        //     std::cout << "point = " << point.x << ' ' << point.y << std::endl;
        // }
    }

    for (int i = 0; i < 10; i++) {
        std::cout << "第 " << i << " 次優化" << std::endl;
        int originalCost = 0;
        int afterCost = 0;

        std::vector<Net> copyNets = nets;
        std::sort(copyNets.begin(), copyNets.end(), [](const Net &a, const Net &b) {
            return a.loss > b.loss;
        });
        for (auto &net : copyNets) {
            net.loss = 0;
            net.path.clear();
        }

        std::map<Point, int> gridUsageCount;
        for (size_t j = 0; j < copyNets.size(); j++) {
            gridUsageCount[copyNets[j].src]++;
            gridUsageCount[copyNets[j].dst]++;
            // std::cout << "j = " << j << std::endl;
            AStar(dieArea, copyNets[j], loss, gridUsageCount, ratio);
            copyNets[j].loss = calculateNetCost(copyNets[j], gridUsageCount, loss);
            // std::cout << "copyNet " << j << " loss = " << copyNets[j].loss << std::endl;
            // for (const auto point : nets[j].path) {
            //     std::cout << "point = " << point.x << ' ' << point.y << std::endl;
            // }

            originalCost += nets[j].loss;
            afterCost += copyNets[j].loss;
        }
        if (afterCost < originalCost) nets = copyNets;
        else break;

        // std::cout << std::endl;
    }

    outputFile(outputFileName, nets);
}