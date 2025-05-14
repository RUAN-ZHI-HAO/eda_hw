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

    // std::cout << "dieArea = (" << dieArea.x << ", " << dieArea.y << ")" << std::endl;
    // std::cout << "propagation = " << loss["propagation"] << " crossing = " << loss["crossing"] << " bending = " << loss["bending"] << std::endl;

    std::unordered_map<Point, int, PointHash> gridUsageCount;
    std::map<std::string, float> ratio = {{"propagation", 1}, {"crossing", 1.1}, {"bending", 1}, {"heuristic", 1.14}};
    for (const auto &net : nets) {
        gridUsageCount[net.src]++;
        gridUsageCount[net.dst]++;
    }

    for (size_t i = 0; i < nets.size(); i++) {
        AStar(dieArea, nets[i], loss, gridUsageCount, ratio);
        nets[i].loss = calculateNetCost(nets[i], gridUsageCount, loss);
    }

    for (int i = 0; i < 6; i++) {
        if (i == 3) ratio["crossing"] *= 1.5;
        int k = std::max<int>(1, nets.size() * 0.2); //find top loss with 10%
        std::partial_sort(nets.begin(), nets.begin() + k, nets.end(),[](auto &a, auto &b) {
             return a.loss > b.loss; 
        });

        std::vector<Net> testNets;


        float beforCost = 0;
        float afterCost = 0;
        for (int j = 0; j < k; j++) {
            beforCost += calculateNetCost(nets[j], gridUsageCount, loss);
            testNets.push_back(nets[j]);

            for (size_t m = 0; m < nets[j].path.size(); m++) {
                gridUsageCount[nets[j].path[m]]--;
            }
            testNets[j].path.clear();
            testNets[j].loss = 0;   
        }

        for (int j = 0; j < k; j++) {
            gridUsageCount[nets[j].src]++;
            gridUsageCount[nets[j].dst]++;
        }

        for (int j = 0; j < k; j++) {
            AStar(dieArea, testNets[j], loss, gridUsageCount, ratio);
            float pp = calculateNetCost(testNets[j], gridUsageCount, loss);
            afterCost += pp;
        }

        if (afterCost < beforCost) {
            std::copy(testNets.begin(), testNets.end(), nets.begin());
            for (auto &n : nets)
                n.loss = calculateNetCost(n, gridUsageCount, loss);
            }
        else {
            for (int j = 0; j < k; j++) {
                for (const Point &point : testNets[j].path) gridUsageCount[point]--;
                for (const Point &point : nets[j].path) gridUsageCount[point]++; // 補回原本舊線的使用量
            }
        }
    }
    outputFile(outputFileName, nets);
}