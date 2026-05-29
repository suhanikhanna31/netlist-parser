* CMOS Inverter Netlist
* Syntax: R/C <name> <nodeA> <nodeB> <value>
*         M <name> <drain> <gate> <source> <bulk> <NMOS|PMOS>

* Pull-up PMOS
M1 out in vdd vdd PMOS

* Pull-down NMOS
M2 out in gnd gnd NMOS

* Input bias resistor
R1 in net1 1000

* Load capacitor on output
C1 out gnd 1e-12

* Decoupling cap on supply
C2 vdd gnd 10e-12
