#include "my_struct.h"
#include "function.h"

float calc_loss(const Net &net, std::unordered_map<Point, int, PointHash> gridUsageCount){
    float propLen = (float)net.path.size()-1;
    float crossCnt=0, bendCnt=0;

    /* crossing */
    for(const Point &pt: net.path){
        crossCnt += std::max(0, gridUsageCount[{pt.x, pt.y}]-1);
    }
    /* bending */
    for(size_t i=2;i<net.path.size();++i){
        auto &c=net.path[i-2], &b=net.path[i-1], &a=net.path[i];
        if((c.x==b.x&&b.x==a.x)||(c.y==b.y&&b.y==a.y)) continue;
        ++bendCnt;
    }
    return propLen*1 + crossCnt*10 + bendCnt*3;
}


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
    // for (const auto &net : nets) {
    //     std::cout << "index = " << net.index << " src = (" << net.src.x << ", " << net.src.y << ") dst = (" << net.dst.x << ", " << net.dst.y << ")" << std::endl;
    // }
    std::cout << std::endl;

    std::unordered_map<Point, int, PointHash> gridUsageCount;
    std::map<std::string, float> ratio = {{"propagation", 1}, {"crossing", 1.1}, {"bending", 1}, {"heuristic", 1.14}};
    for (const auto &net : nets) {
        gridUsageCount[net.src]++;
        gridUsageCount[net.dst]++;
    }

    // std::sort(nets.begin(), nets.end(), [](Net &a, Net &b) {
    //     return std::abs(a.dst.x - a.src.x) + std::abs(a.dst.y - a.src.y) > std::abs(b.dst.x - b.src.x) + std::abs(b.dst.y - b.src.y);
    // });

    for (size_t i = 0; i < nets.size(); i++) {
        // std::cout << "i = " << i << std::endl;
        AStar(dieArea, nets[i], loss, gridUsageCount, ratio);
        nets[i].loss = calculateNetCost(nets[i], gridUsageCount, loss);
        // for (const auto point : nets[i].path) {
        //     std::cout << "point = " << point.x << ' ' << point.y << std::endl;
        // }
    }

    for (int i = 0; i < 5; i++) {
        if (i == 3) ratio["crossing"] *= 1.5;
        int k = std::max<int>(1, nets.size() * 0.1); //find top loss with 10%
        std::partial_sort(nets.begin(), nets.begin() + k, nets.end(),[](auto &a, auto &b) {
             return a.loss > b.loss; 
        });

        std::vector<Net> testNets;


        float beforCost = 0;
        float afterCost = 0;
        for (int j = 0; j < k; j++) {
            beforCost += calculateNetCost(nets[j], gridUsageCount, loss);
            testNets.push_back(nets[j]);
            // std::cout << "befro = " << nets[j].path.size() << std::endl;

            for (size_t m = 0; m < nets[j].path.size(); m++) {
                gridUsageCount[nets[j].path[m]]--;
            }
            // std::cout << "testNest[j].loss = " << testNets[j].loss << std::endl;
            testNets[j].path.clear();
            testNets[j].loss = 0;

            // std::cout << "after = " << nets[j].path.size() << std::endl;

            
        }

        for (int j = 0; j < k; j++) {
            gridUsageCount[nets[j].src]++;
            gridUsageCount[nets[j].dst]++;
        }

        for (int j = 0; j < k; j++) {
            AStar(dieArea, testNets[j], loss, gridUsageCount, ratio);
            float pp = calculateNetCost(testNets[j], gridUsageCount, loss);
            afterCost += pp;
            // std::cout << "pp = " << pp << std::endl;
            // std::cout << "testNest[j].loss = " << testNets[j].loss << std::endl;
            // testNets[j].loss = pp;
        }

        if (afterCost < beforCost) {
            std::copy(testNets.begin(), testNets.end(), nets.begin());
            // std::cout << "nets_size = " << nets.size() << std::endl;
            // 更新其餘 nets 的 loss（交叉數已改變）
            for (auto &n : nets)
                n.loss = calculateNetCost(n, gridUsageCount, loss);
            }
        else {
            // std::cout << "有道這邊" << std::endl;
            for (int j = 0; j < k; j++) {
                for (const Point &point : testNets[j].path) gridUsageCount[point]--;
                for (const Point &point : nets[j].path) gridUsageCount[point]++; // 補回原本舊線的使用量
            }
        }
    }

    // for (int i = 0; i < 5; i++) {
    //     std::cout << "第 " << i << " 次優化" << std::endl;
    //     int originalCost = 0;
    //     for (const auto &net : nets) {
    //         originalCost += net.loss;
    //     }

    //     int afterCost = 0;

    //     std::vector<Net> copyNets = nets;
    //     std::sort(copyNets.begin(), copyNets.end(), [](const Net &a, const Net &b) {
    //         return a.loss > b.loss;
    //     });
    //     for (auto &net : copyNets) {
    //         net.loss = 0;
    //         net.path.clear();
    //     }

    //     std::unordered_map<Point, int, PointHash> gridUsageCount;
    //     for (size_t j = 0; j < copyNets.size(); j++) {
    //         gridUsageCount[copyNets[j].src]++;
    //         gridUsageCount[copyNets[j].dst]++;
    //         // std::cout << "j = " << j << std::endl;
    //         AStar(dieArea, copyNets[j], loss, gridUsageCount, ratio);
    //         copyNets[j].loss = calculateNetCost(copyNets[j], gridUsageCount, loss);
    //         // std::cout << "copyNet " << j << " loss = " << copyNets[j].loss << std::endl;
    //         // for (const auto point : nets[j].path) {
    //         //     std::cout << "point = " << point.x << ' ' << point.y << std::endl;
    //         // }

    //         afterCost += copyNets[j].loss;
    //     }
    //     if (afterCost < originalCost) nets = copyNets;
    //     else break;

    //     // std::cout << std::endl;
    // }

    outputFile(outputFileName, nets);
}