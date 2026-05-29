#pragma once
#include "component.h"
#include "Subcircuit.h"
#include <iostream>
#include <map>
#include <string>

// Visitor interface — decouples analysis from the component hierarchy
class CircuitVisitor {
public:
    virtual ~CircuitVisitor() = default;
    virtual void visit(const Resistor& r) = 0;
    virtual void visit(const Capacitor& c) = 0;
    virtual void visit(const Mosfet& m) = 0;
};

// Concrete visitor: counts components by type and reports circuit stats
class Analyzer : public CircuitVisitor {
public:
    void visit(const Resistor&)  override { counts_["Resistor"]++; }
    void visit(const Capacitor&) override { counts_["Capacitor"]++; }
    void visit(const Mosfet& m)  override { counts_[m.type()]++; }

    void analyze(const Subcircuit& sub) {
        std::cout << "\n=== Netlist Report: " << sub.name() << " ===\n";

        // Run visitor over every component
        for (const auto& comp : sub.components())
            comp->accept(*this);

        // Component census
        std::cout << "\nComponent counts:\n";
        int total = 0;
        for (const auto& [type, count] : counts_) {
            std::cout << "  " << type << ": " << count << "\n";
            total += count;
        }
        std::cout << "  TOTAL: " << total << "\n";

        // Net statistics
        std::cout << "\nNet count: " << sub.nets().size() << "\n";

        if (const Net* top = sub.highestFanoutNet()) {
            std::cout << "Highest fanout net: \"" << top->name()
                      << "\"  fanout=" << top->fanout() << "\n";
        }
        std::cout << "===========================\n";
    }

private:
    std::map<std::string, int> counts_;
};
