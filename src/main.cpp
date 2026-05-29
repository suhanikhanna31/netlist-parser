#include "NetlistParser.h"
#include "Analyzer.h"
#include <iostream>

int main(int argc, char* argv[]) {
    std::string filepath = "circuits/inverter.sp";
    if (argc > 1) filepath = argv[1];

    std::cout << "NetlistParser — EDA Circuit Analyzer\n";
    std::cout << "Parsing: " << filepath << "\n";

    try {
        NetlistParser parser;
        auto subckt = parser.parse(filepath);

        Analyzer analyzer;
        analyzer.analyze(*subckt);
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
