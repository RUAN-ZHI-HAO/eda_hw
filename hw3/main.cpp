#include "my_struct.h"

int main(int argc, char* argv[]) {
    std::ifstream inputFile(argv[1]);
    std::ofstream outputFile(argv[2]);

    if (!inputFile) {
        std::cerr << "you need ChatGPT help -__-" << std::endl;
        return 1;
    }

    Point dieArea(0, 0);
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
            while(getline(inputFile, line)) {
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
}