=====
MWPC: Multi Wire Proportional Chamber
=====

Description:

- Input and exit windows: 12um of Mylar
- 64 vertical pads: each pad is 3.125 mm width. The pads are Al evaporated on a 12um-foil of Mylar
- 1 plane of horizontal wires: each wire is 5 μm diameter of W
- 1 “cathode” plane of 12um of Mylar
- 1 plane of vertical wires: each wire is 5 μm diameter of W
- 64 horizontal pads: each pad is 3.125 mm width (Al evaporated on a 12um-foil of Mylar)
- HV to be adjusted with beam: pads are grounded and wires at 1100V
- Gas: 84% Ar + 16% CO2 (NB: for LISE calculation at 300K and 1 bar, M=40.6 g/mol, density=1.6491 g/L)


=====
Description of the parameter cointainers
=====

Thera are two cointeiners to manage the parameters of each MWPC detector:

- MwpcDGeoPar: Manage the position, angles and dimenssions of the detector in the Lab. frame.
- mwpcDCalPar: Manage the pad pedestals for energy alignment.

where D is the number of MWPC: 0,1,2 or 3.
