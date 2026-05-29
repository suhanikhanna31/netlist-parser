# NetlistParser

A hierarchical SPICE netlist analyzer written in C++17, inspired by netlist processing in EDA tools like Cadence Spectre and Virtuoso.

## Features

- Parses SPICE-format `.sp` netlists into an in-memory object model
- Models circuit topology with an abstract `Component` hierarchy (`Resistor`, `Capacitor`, `Mosfet`)
- Uses the **Composite pattern** for subcircuit nesting
- Uses the **Visitor pattern** for extensible analysis passes
- Reports component census, net count, and highest-fanout node

## Project Structure

```
NetlistParser/
├── include/
│   ├── Component.h       # Abstract base + Resistor, Capacitor, Mosfet
│   ├── Net.h             # Electrical net with fanout tracking
│   ├── Subcircuit.h      # Composite container for components and nets
│   ├── NetlistParser.h   # Tokenizer and recursive parser
│   └── Analyzer.h        # Visitor-based circuit analysis
├── src/
│   ├── main.cpp          # Entry point
│   └── Component.cpp     # Visitor accept() implementations
├── circuits/
│   ├── inverter.sp       # CMOS inverter sample netlist
│   └── rc_filter.sp      # RC low-pass filter sample netlist
└── CMakeLists.txt
```

## Build

```bash
mkdir build && cd build
cmake ..
cmake --build .
```

## Usage

```bash
# Parse default circuit (circuits/inverter.sp)
./netlist_parser

# Parse a custom netlist
./netlist_parser circuits/rc_filter.sp
```

## Sample Output

```
NetlistParser — EDA Circuit Analyzer
Parsing: circuits/inverter.sp

=== Netlist Report: inverter ===

Component counts:
  Capacitor: 2
  NMOS: 1
  PMOS: 1
  Resistor: 1
  TOTAL: 5

Net count: 6
Highest fanout net: "gnd"  fanout=4
===========================
```

## Design Patterns Used

| Pattern | Where | Why |
|---|---|---|
| **Abstract Factory / Inheritance** | `Component` hierarchy | Uniform interface across element types |
| **Composite** | `Subcircuit` | Owns and organizes components + nets |
| **Visitor** | `CircuitVisitor` / `Analyzer` | Decouple analysis from data model |

## Netlist Format

```spice
* Comment lines start with *
R<name> <nodeA> <nodeB> <resistance_ohms>
C<name> <nodeA> <nodeB> <capacitance_farads>
M<name> <drain> <gate> <source> <bulk> <NMOS|PMOS>
```
