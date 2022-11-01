#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

int main() {
    std::string inps = R"(
        0 0 2 0
        4 0 3 0
        0 0 0 2
        0 1 0 0
    )";
    std::stringstream inp;
    inp.str(inps);

    size_t const inf = size_t(-1) >> 2;
    std::vector<std::vector<size_t>> d;
    // Input
    std::string line;
    while (std::getline(inp, line)) {
        if (line.find_last_not_of(" rnt") == std::string::npos)
            continue;
        d.resize(d.size() + 1);
        std::stringstream ss;
        ss.str(line);
        size_t x = 0;
        while (ss >> x)
            d.back().push_back(x);
    }
    // metoda Floyd-Warshall
    for (size_t i = 0; i < d.size(); ++i)
        for (size_t j = 0; j < d[i].size(); ++j)
            if (d[i][j] == 0 && i != j)
                d[i][j] = inf;
    for (size_t k = 0; k < d.size(); ++k)
        for (size_t i = 0; i < d.size(); ++i)
            for (size_t j = 0; j < d.size(); ++j)
                d[i][j] = std::min(d[i][j], d[i][k] + d[k][j]);



    // diametrul
    size_t maxd = 0, maxi = 0, maxj = 0;
    for (size_t i = 0; i < d.size(); ++i)
        for (size_t j = 0; j < d.size(); ++j)
            if (maxd < d[i][j]) {
                maxd = d[i][j];
                maxi = i;
                maxj = j;
            }
    // Output
    std::cout << "Diametrul= " << maxd << std::endl;
}
