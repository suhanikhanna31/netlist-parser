#include "component.h"
#include "Analyzer.h"

void Resistor::accept(CircuitVisitor& visitor) const  { visitor.visit(*this); }
void Capacitor::accept(CircuitVisitor& visitor) const { visitor.visit(*this); }
void Mosfet::accept(CircuitVisitor& visitor) const    { visitor.visit(*this); }
