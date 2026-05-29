#pragma once
#include "Subcircuit.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Parses a minimal SPICE-format netlist into a Subcircuit object model.
// Supported element lines:
//   R<name> <nodeA> <nodeB> <value>        -- Resistor
//   C<name> <nodeA> <nodeB> <value>        -- Capacitor
//   M<name> <drain> <gate> <source> <bulk> <NMOS|PMOS>  -- MOSFET
class NetlistParser {
public:
    // Parse file at path; throws std::runtime_error on failure
    std::unique_ptr<Subcircuit> parse(const std::string& filepath) {
        std::ifstream file(filepath);
        if (!file.is_open())
            throw std::runtime_error("Cannot open netlist: " + filepath);

        // Use filename (without extension) as subcircuit name
        std::string subcktName = filepath;
        auto slash = subcktName.find_last_of("/\\");
        if (slash != std::string::npos) subcktName = subcktName.substr(slash + 1);
        auto dot = subcktName.rfind('.');
        if (dot != std::string::npos) subcktName = subcktName.substr(0, dot);

        auto subckt = std::make_unique<Subcircuit>(subcktName);

        std::string line;
        int lineNo = 0;
        while (std::getline(file, line)) {
            ++lineNo;
            // Strip comments and blank lines
            auto commentPos = line.find('*');
            if (commentPos != std::string::npos) line = line.substr(0, commentPos);
            if (line.find_first_not_of(" \t\r\n") == std::string::npos) continue;

            try {
                parseLine(line, *subckt);
            } catch (const std::exception& e) {
                std::cerr << "Warning: line " << lineNo << " skipped — " << e.what() << "\n";
            }
        }
        return subckt;
    }

private:
    std::vector<std::string> tokenize(const std::string& line) {
        std::istringstream iss(line);
        std::vector<std::string> tokens;
        std::string tok;
        while (iss >> tok) tokens.push_back(tok);
        return tokens;
    }

    void parseLine(const std::string& line, Subcircuit& subckt) {
        auto tokens = tokenize(line);
        if (tokens.empty()) return;

        const char prefix = static_cast<char>(std::toupper(tokens[0][0]));

        if (prefix == 'R') {
            if (tokens.size() < 4)
                throw std::runtime_error("Resistor needs 4 fields: R<name> nodeA nodeB value");
            double val = std::stod(tokens[3]);
            subckt.addComponent(
                std::make_unique<Resistor>(tokens[0], tokens[1], tokens[2], val));

        } else if (prefix == 'C') {
            if (tokens.size() < 4)
                throw std::runtime_error("Capacitor needs 4 fields: C<name> nodeA nodeB value");
            double val = std::stod(tokens[3]);
            subckt.addComponent(
                std::make_unique<Capacitor>(tokens[0], tokens[1], tokens[2], val));

        } else if (prefix == 'M') {
            if (tokens.size() < 6)
                throw std::runtime_error("MOSFET needs 6 fields: M<name> drain gate source bulk type");
            std::string mtype = tokens[5];
            for (auto& c : mtype) c = static_cast<char>(std::toupper(c));
            Mosfet::Channel ch = (mtype == "PMOS") ? Mosfet::Channel::PMOS
                                                    : Mosfet::Channel::NMOS;
            subckt.addComponent(
                std::make_unique<Mosfet>(tokens[0], tokens[1], tokens[2],
                                         tokens[3], tokens[4], ch));
        } else {
            // Unknown prefix — skip silently (e.g. .subckt, .ends, .tran)
        }
    }
};
