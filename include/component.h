#pragma once
#include <string>
#include <vector>

// Forward declaration for Visitor pattern
class CircuitVisitor;

// Abstract base class for all circuit components
class Component {
public:
    Component(const std::string& name, const std::vector<std::string>& pins)
        : name_(name), pins_(pins) {}

    virtual ~Component() = default;

    virtual std::string type() const = 0;
    virtual void accept(CircuitVisitor& visitor) const = 0;

    const std::string& name() const { return name_; }
    const std::vector<std::string>& pins() const { return pins_; }

protected:
    std::string name_;
    std::vector<std::string> pins_;
};

class Resistor : public Component {
public:
    Resistor(const std::string& name, const std::string& nodeA,
             const std::string& nodeB, double ohms)
        : Component(name, {nodeA, nodeB}), ohms_(ohms) {}

    std::string type() const override { return "Resistor"; }
    void accept(CircuitVisitor& visitor) const override;
    double resistance() const { return ohms_; }

private:
    double ohms_;
};

class Capacitor : public Component {
public:
    Capacitor(const std::string& name, const std::string& nodeA,
              const std::string& nodeB, double farads)
        : Component(name, {nodeA, nodeB}), farads_(farads) {}

    std::string type() const override { return "Capacitor"; }
    void accept(CircuitVisitor& visitor) const override;
    double capacitance() const { return farads_; }

private:
    double farads_;
};

class Mosfet : public Component {
public:
    enum class Channel { NMOS, PMOS };

    Mosfet(const std::string& name, const std::string& drain,
           const std::string& gate, const std::string& source,
           const std::string& bulk, Channel ch)
        : Component(name, {drain, gate, source, bulk}), channel_(ch) {}

    std::string type() const override {
        return channel_ == Channel::NMOS ? "NMOS" : "PMOS";
    }
    void accept(CircuitVisitor& visitor) const override;
    Channel channel() const { return channel_; }

private:
    Channel channel_;
};
