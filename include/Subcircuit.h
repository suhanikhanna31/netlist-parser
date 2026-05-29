#pragma once
#include "component.h"
#include "net.h"
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

// Composite: owns components and nets, models a .subckt block
class Subcircuit {
public:
    explicit Subcircuit(const std::string& name) : name_(name) {}

    // Add a component; Subcircuit takes ownership
    void addComponent(std::unique_ptr<Component> comp) {
        // Register component pins onto their nets
        for (const auto& pin : comp->pins()) {
            nets_[pin].addConnection(comp->name(), pin);
        }
        components_.push_back(std::move(comp));
    }

    const std::string& name() const { return name_; }

    const std::vector<std::unique_ptr<Component>>& components() const {
        return components_;
    }

    const std::unordered_map<std::string, Net>& nets() const { return nets_; }

    // Returns the net with highest fanout
    const Net* highestFanoutNet() const {
        const Net* best = nullptr;
        for (const auto& [netName, net] : nets_) {
            if (!best || net.fanout() > best->fanout())
                best = &net;
        }
        return best;
    }

private:
    std::string name_;
    std::vector<std::unique_ptr<Component>> components_;
    std::unordered_map<std::string, Net> nets_;
};
