* RC Low-Pass Filter
* fc = 1 / (2 * pi * R * C) ~ 1.6 kHz

R1 vin vmid 100000
R2 vmid vout 100000
C1 vmid gnd 1e-9
C2 vout gnd 1e-9
