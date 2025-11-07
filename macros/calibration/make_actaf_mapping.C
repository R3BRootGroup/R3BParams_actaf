void make_actaf_mapping()
{
    std::ifstream infile("mapping_amber_202509.txt");
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
            continue; // ignorar líneas vacías o comentarios
        std::istringstream iss(line);
        if (!(iss >> col1 >> col2 >> col3))
        {
            std::cerr << "Error reading line: " << line << std::endl;
            continue;
        }
        data.push_back(std::make_tuple(col1, col2, col3));
    }

    infile.close();

    std::vector<int> modval;
    std::vector<int> chval;

    for (auto& entry : data)
    {
        int pad, module, ch;
        std::tie(pad, module, ch) = entry;
        modval.push_back(module);
        chval.push_back(ch);
        // std::cout<<pad<<" "<<module<<" "<<ch<<std::endl;
    }

    auto rtdb = FairRuntimeDb::instance();

    auto parOut = new FairParAsciiFileIo();
    parOut->open("actafmapping_v2.par", "out");
    rtdb->setOutput(parOut);

    auto par = (R3BActafMappingPar*)(rtdb->getContainer("actafMappingPar"));

    rtdb->addRun(1);

    for (int index = 0; index < 128; index++)
    {
        par->SetFADCModule(index, modval[index]);
        par->SetFADCChannel(index, 17-chval[index]);
    }

    par->setChanged();

    rtdb->writeContainers();

    rtdb->saveOutput();

    rtdb->print();
}
