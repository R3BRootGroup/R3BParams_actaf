=====
AMS (Alpha Magnetic Spectrometer) silicon detectors
=====

Description:

Each Si sensor has a size of 72 mm x 40 mm, and is 0.3 mm thick. Due to the strong forward focusing of the reaction products at relativistic energies, a very high position resolution, and therefore a high granularity of the Si detectors is required. Therefore, each sensor has an implantation pitch on the junction side (called S-side or p-side) of the sensor of 27.5 μm. The corresponding read-out pitch is 110 μm, i.e. every fourth strip is connected to a read-out channel, while the others are left floating. On the ohmic (K- or n-) side of the sensor, the implantation pitch is 104 μm with every strip being read out. This adds up to 640 strips to be read out on the S-side and 384 on the K-side, yielding a total of 1024 channels per sensor. Two printed circuit boards (“hybrids”) are glued back to back, and they perform readout of a single sensor with 640 readout strips on S-side and 384 readout strips on K-side. Each silicon sensor is connected electrically to these boards through flexible capton cables. Mechanical rigidity is provided by a stesalite structure where both hybrids and sensor are attached to. 

=====
Description of the parameter cointainers
=====

Thera are two cointeiners to manage the parameters of each AMS detector:

- amsMappingPar: Manage the information about the detector mapping, position of each detector with respect to the target in the Lab. frame.
- amsStripCalPar: Manage the pedestals of each pedestal for the energy alignment.
