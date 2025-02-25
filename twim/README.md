=====
Twim Music: Double Multi-Sampling Ionization Chamber
=====

Description:

- Double ionisation chamber with a central cathode
- Two anode planes (left/right) of 16 anodes + 2 screen anodes
- Each anode is segmented up/down
- Outbox dimension: 430x480x550 mm3
- 2 active volumes of 110x220x400 mm3
- Frish grids are located at 110 mm from the central cathod and 3 mm from the anode planes
- Gas: CH4 [79%], Ar [20%] and CO2 [1%]

=====
Description of the parameter cointainers
=====

Thera are three cointeiners to manage the parameters of this detector:

- TwimGeoPar: Manage the position, angles and dimenssions of the detector in the Lab. frame.
- twimCalPar: Manage the pedestals for energy alignment and drift time calibration in mm.
- twimHitPar: Manage the ToF correction for atomic number calibration and the position of each anode to obtain the polar angles of our fragments in the plane X-Z.
