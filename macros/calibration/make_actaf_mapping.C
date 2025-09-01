void make_actaf_mapping()
{
  auto rtdb = FairRuntimeDb::instance();
  
  auto parOut = new FairParAsciiFileIo();
  parOut->open("actafmapping_v1.par", "out");
  rtdb->setOutput(parOut);

  auto par = (R3BActafMappingPar*)(rtdb->getContainer("actafMappingPar"));
  
  rtdb->addRun(1);
 
  par->setChanged();

  rtdb->writeContainers();

  rtdb->saveOutput();

  rtdb->print();
}


