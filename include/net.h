#pragma once
#include <string>
#include <vector>

// Represents a named electrical net (wire) connecting component pins
class Net {
public:
    explicit Net(const std::string& name) : name_(name) {}

    void addConnection(const std::string& componentName, const std::string& pin) {
        connections_.emplace_back(componentName, pin);
    }

    const std::string& name() const { return name_; }
    int fanout() const { return static_cast<int>(connections_.size()); }

    const std::vector<std::pair<std::string, std::string>>& connections() const {
        return connections_;
    }

private:
    std::string name_;
    std::vector<std::pair<std::string, std::string>> connections_; // {component, pin}
};
