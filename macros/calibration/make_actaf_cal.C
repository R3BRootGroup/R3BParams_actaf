void make_actaf_cal()
{
    std::ifstream infile("cal_amber_test.txt");
    if (!infile.is_open())
    {
        std::cerr << "Error opening input file!" << std::endl;
        return;
    }

    int col1, col2;
    double col3;
    std::vector<std::tuple<int, int, double>> data;

    std::string line;
    while (std::getline(infile, line))
    {
        if (line.empty() || line[0] == '#')
            continue; // ignore empty lines or comments
        std::istringstream iss(line);
        if (!(iss >> col1 >> col2 >> col3))
        {
            std::cerr << "Error reading line: " << line << std::endl;
            continue;
        }
        data.push_back(std::make_tuple(col1, col2, col3));
    }

    infile.close();

    std::vector<double> gainval;
    std::vector<double> thrval;

    for (auto& entry : data)
    {
        int pad;
        double gain, thr;
        std::tie(pad, gain, thr) = entry;
        gainval.push_back(gain);
        thrval.push_back(thr);
    }

    auto rtdb = FairRuntimeDb::instance();

    auto parOut = new FairParAsciiFileIo();
    parOut->open("actafcal_v1.par", "out");
    rtdb->setOutput(parOut);

    rtdb->addRun(1);
    auto par = (R3BActafCalPar*)(rtdb->getContainer("actafCalPar"));

    for (int index = 0; index < 128; index++)
    {
        par->SetGainVal(index, gainval[index]);
        par->SetThresholdVal(index, thrval[index]);
    }

    par->setChanged();

    rtdb->writeContainers();

    rtdb->saveOutput();

    rtdb->print();
}
