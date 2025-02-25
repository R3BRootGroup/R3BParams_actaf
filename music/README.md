=====
R3B-Music: Multi-Sampling Ionization Chamber
=====

Description:

- 8 anodes + 2 screen anodes
- Gas: CH4 [79%], Ar [20%] and CO2 [1%]

=====
Description of the parameter cointainers
=====

Thera are three cointeiners to manage the parameters of this detector:

- MusicGeoPar: Manage the position, angles and dimenssions of the detector in the Lab. frame.
- musicCalPar: Manage the pedestals for energy alignment and drift time calibration in mm.
- musicHitPar: Manage the ToF correction for atomic number calibration and the position of each anode to obtain the polar angles of our fragments in the plane X-Z.
