#include <fstream>
#include "ChipArea.h"
#include <cstring>
#include <fstream>
#include <vector>
#include <sstream>
#include <Parse.h>
#include <ChipArea.h>
#include <filesystem>

void ChipArea::addChip(Chip *chip)
{
    chip->index = chips->size();
    chips->push_back(chip);

}

void ChipArea::save_circuit(std::string &name)
{
    std::fstream file;
    std::string ext = ".chip";
    name += ext;
    std::string fullPath = currentDirName + "/" + name;
    file.open(fullPath, std::ios::in | std::ios::binary | std::ios::trunc | std::ios::out);
    int no_of_global_input_pin = globalInputPins->size();
    int no_of_global_output_pin = globalOutputPins->size();
    file << "# Global Input -> " << no_of_global_input_pin << std::endl;
    file << "# Global Output -> " << no_of_global_output_pin << std::endl;
    file << "# Chips" << std::endl;
    file << "# start" << std::endl;
    for (int i = 0; i < chips->size(); i++)
    {
        file << i << "." << " " << chips->at(i)->name << std::endl;
        file << "InputPin -> " << chips->at(i)->inputPins.size() << std::endl;
        file << "OutputPin -> " << chips->at(i)->outputPins.size() << std::endl;
        std::string chip_type = (chips->at(i)->type == CUSTOM ? "CUSTOM" : "INBUILT");
        file << "Type -> " << chip_type << std::endl;
    }
    file << "# end" << std::endl;
    file << "# Binds" << std::endl;
    file << "# start" << std::endl;
    int no_of_binds_from_global_input_to_global_output = 0;
    int no_of_binds_from_global_input_to_chip = 0;
    int no_of_binds_from_chip_to_chip = 0;
    int no_of_binds_from_chip_to_global_output = 0;

    for (int i = 0; i < globalInputPins->size(); i++)
    {
        no_of_binds_from_global_input_to_global_output += globalInputPins->at(i)->gbinds->size();
        no_of_binds_from_global_input_to_chip += globalInputPins->at(i)->binds->size();
    }

    for (int i = 0; i < chips->size(); i++)
    {
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {
            no_of_binds_from_chip_to_chip += chips->at(i)->outputPins.at(j)->binds->size();
            no_of_binds_from_chip_to_global_output += chips->at(i)->outputPins.at(j)->bindsToGlobalOutput->size();
        }
    }

    // global input pin to ..
    //  print # GI to GO -> no of all binds from global input to global output
    file << "# GI to GO -> " << no_of_binds_from_global_input_to_global_output << std::endl;
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        // print GI[index] -> GO[index]
        // global input pin to global output pin
        for (int j = 0; j < globalInputPins->at(i)->gbinds->size(); j++)
        {
            int index = -1;
            for (int k = 0; k < globalOutputPins->size(); k++)
            {
                if (globalOutputPins->at(k) == &(globalInputPins->at(i)->gbinds->at(j)->output))
                {
                    index = k;
                    break;
                }
            }
            file << "GI[" << i << "] -> GO[" << index << "]" << std::endl;
        }
    }

    // print GI[index] -> CI[index][index]
    //  global input pin to chip input pin
    file << "# GI to CI -> " << no_of_binds_from_global_input_to_chip << std::endl;
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        for (int j = 0; j < globalInputPins->at(i)->binds->size(); j++)
        {
            int index = -1;
            for (int k = 0; k < chips->size(); k++)
            {
                for (int l = 0; l < chips->at(k)->inputPins.size(); l++)
                {
                    if (chips->at(k)->inputPins.at(l) == &(globalInputPins->at(i)->binds->at(j)->input))
                    {
                        index = k;
                        break;
                    }
                }
            }

            int pinIndex = -1;
            for (int k = 0; k < chips->at(index)->inputPins.size(); k++)
            {
                if (chips->at(index)->inputPins.at(k) == &(globalInputPins->at(i)->binds->at(j)->input))
                {
                    pinIndex = k;
                    break;
                }
            }
            file << "GI[" << i << "] -> CI[" << index << "][" << pinIndex << "]" << std::endl;
        }
    }

    // print CO[index][index] -> CI[index][index]
    //  chip output pin to chip input pin
    file << "# CO to CI -> " << no_of_binds_from_chip_to_chip << std::endl;
    for (int i = 0; i < chips->size(); i++)
    {
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {
            for (int k = 0; k < chips->at(i)->outputPins.at(j)->binds->size(); k++)
            {
                int index = -1;
                for (int l = 0; l < chips->size(); l++)
                {
                    for (int m = 0; m < chips->at(l)->inputPins.size(); m++)
                    {
                        if (chips->at(l)->inputPins.at(m) == &(chips->at(i)->outputPins.at(j)->binds->at(k)->input))
                        {
                            index = l;
                            break;
                        }
                    }
                }

                int pinIndex = -1;
                for (int l = 0; l < chips->at(index)->inputPins.size(); l++)
                {
                    if (chips->at(index)->inputPins.at(l) == &(chips->at(i)->outputPins.at(j)->binds->at(k)->input))
                    {
                        pinIndex = l;
                        break;
                    }
                }
                file << "CO[" << i << "][" << j << "] -> CI[" << index << "][" << pinIndex << "]" << std::endl;
            }
        }
    }

    // print CO[index][index] -> GO[index]
    //  chip output pin to global output pin
    file << "# CO to GO -> " << no_of_binds_from_chip_to_global_output << std::endl;
    for (int i = 0; i < chips->size(); i++)
    {
        for (int j = 0; j < chips->at(i)->outputPins.size(); j++)
        {
            for (int k = 0; k < chips->at(i)->outputPins.at(j)->bindsToGlobalOutput->size(); k++)
            {
                int index = -1;
                for (int l = 0; l < globalOutputPins->size(); l++)
                {
                    if (globalOutputPins->at(l) == &(chips->at(i)->outputPins.at(j)->bindsToGlobalOutput->at(k)->output))
                    {
                        index = l;
                        break;
                    }
                }
                file << "CO[" << i << "][" << j << "] -> GO[" << index << "]" << std::endl;
            }
        }
    }

    file << "# end" << std::endl;
}

Chip *ChipArea::load_chip(std::string &name)
{
    Parse parser;
    std::string fullPath = currentDirName + "/" + name;
    parser.parseFile(fullPath);
    int global_input_pins_count = parser.globalInputCount;
    int global_output_pins_count = parser.globalOutputCount;
    std::vector<Chip *> *Ichips = new std::vector<Chip *>();
    // // // // create chips
    for (const auto &chipInfo : parser.chips)
    {
        // Initialize a new ChipStructure with a bounding box
        ChipStructure *structure = new ChipStructure(new ChipBoundingBox({0, 0, 0, 0}));
        std::vector<InputPin *> inputPins;
        std::vector<OutputPin *> outputPins;

        // Determine the chip type
        ChipType typeE;
        std::string chipName = chipInfo.name;
        // trim spaces
        chipName.erase(std::remove(chipName.begin(), chipName.end(), ' '), chipName.end());
        if (chipInfo.type == "INBUILT")
        {
            if (chipName == "AND")
            {
                typeE = AND;
            }
            else if (chipName == "NOT")
            {
                typeE = NOT;
            }
        }
        else
        {
            typeE = CUSTOM;
            std::string chipFileName = chipName + ".chip";
            std::string test = "NAND.chip";

            Chip *newChip = load_chip(chipFileName);
            Ichips->push_back(newChip);
            continue;
        }

        // Create input pins
        for (int j = 0; j < chipInfo.inputPin; ++j)
        {
            InputPin *inputPin = new InputPin("I", j);
            inputPins.push_back(inputPin);
        }

        // Create output pins
        for (int j = 0; j < chipInfo.outputPin; ++j)
        {
            OutputPin *outputPin = new OutputPin("O", j);
            outputPins.push_back(outputPin);
        }

        // Create the chip with the structure, input pins, output pins, and name
        Chip *chip = new Chip(structure, inputPins, outputPins, chipInfo.name);
        chip->type = typeE;

        // Add the chip to the list of chips
        Ichips->push_back(chip);
    }

    // // create global input pins
    std::vector<GlobalInputPin *> *globalInputPins = new std::vector<GlobalInputPin *>();
    std::vector<InputPin *> *inputPins = new std::vector<InputPin *>();
    for (int i = 0; i < parser.globalInputCount; i++)
    {
        GlobalInputPin *globalInputPin = new GlobalInputPin(0, 0);
        globalInputPins->push_back(globalInputPin);
        std::string nameI = "A";
        // nameI += std::to_string(i);
        InputPin *inputPin = new InputPin(nameI, i);
        inputPins->push_back(inputPin);
    }
    // // // create global output pins
    std::vector<GlobalOutputPin *> *globalOutputPins = new std::vector<GlobalOutputPin *>();
    std::vector<OutputPin *> *outputPins = new std::vector<OutputPin *>();
    for (int i = 0; i < parser.globalOutputCount; i++)
    {
        GlobalOutputPin *globalOutputPin = new GlobalOutputPin(0, 0);
        globalOutputPins->push_back(globalOutputPin);
        std::string nameO = "Y";
        // nameO += std::to_string(i);

        OutputPin *outputPin = new OutputPin(nameO, i);
        outputPins->push_back(outputPin);
    }


    // // now binds

    // GI to GO
    for (int i = 0; i < parser.giToGoBinds.size(); i++)
    {
        int GIIndex = parser.giToGoBinds[i].first;
        int GOIndex = parser.giToGoBinds[i].second;
        BindToGlobalOutPut *bind = new BindToGlobalOutPut(*globalOutputPins->at(GOIndex));
        bind->gInput = globalInputPins->at(GIIndex);
        globalInputPins->at(GIIndex)->gbinds->push_back(bind);
        globalOutputPins->at(GOIndex)->bindToGlobalOutput = bind;
    }

    // // // GI to CI
    for (const auto &bindPair : parser.giToCiBinds)
    {
        int GIIndex = bindPair.first;
        const auto &secondIndices = bindPair.second;

        if (secondIndices.size() < 2)
        {
            std::cerr << "Insufficient binding indices in parseGICIBinds: " << bindPair.first << std::endl;
            continue;
        }

        int chipIndex = secondIndices[0];
        int chipPinIndex = secondIndices[1];

        if (GIIndex < 0 || GIIndex >= globalInputPins->size())
        {
            std::cerr << "Invalid global input pin index in parseGICIBinds: " << GIIndex << std::endl;
            continue;
        }

        if (chipIndex < 0 || chipIndex >= Ichips->size())
        {
            std::cerr << "Invalid chip index in parseGICIBinds: " << chipIndex << std::endl;
            continue;
        }

        if (chipPinIndex < 0 || chipPinIndex >= Ichips->at(chipIndex)->inputPins.size())
        {
            std::cerr << "Invalid input pin index in parseGICIBinds: " << chipPinIndex << std::endl;
            continue;
        }

        try
        {
            Bind *bind = new Bind(*Ichips->at(chipIndex)->inputPins[chipPinIndex]);
            bind->gInput = globalInputPins->at(GIIndex);
            globalInputPins->at(GIIndex)->binds->push_back(bind);
            Ichips->at(chipIndex)->inputPins[chipPinIndex]->bind = bind;
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in parseGICIBinds: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception in parseGICIBinds." << std::endl;
        }
    }

    // // // CO to CI
    for (const auto &bind : parser.coToCiBinds)
    {
        int chipIndex = bind.first[0];
        if (chipIndex < 0 || chipIndex >= Ichips->size())
        {
            std::cerr << "Invalid chip index: " << chipIndex << std::endl;
            continue;
        }

        int chipPinIndex = bind.first[1];
        int chipIndex2 = bind.second[0];
        int chipPinIndex2 = bind.second[1];

        if (chipPinIndex < 0 || chipPinIndex >= Ichips->at(chipIndex)->outputPins.size())
        {
            std::cerr << "Invalid output pin index: " << chipPinIndex << " for chip index: " << chipIndex << std::endl;
            continue;
        }

        if (chipIndex2 < 0 || chipIndex2 >= Ichips->size())
        {
            std::cerr << "Invalid second chip index: " << chipIndex2 << std::endl;
            continue;
        }

        if (chipPinIndex2 < 0 || chipPinIndex2 >= Ichips->at(chipIndex2)->inputPins.size())
        {
            std::cerr << "Invalid input pin index: " << chipPinIndex2 << " for second chip index: " << chipIndex2 << std::endl;
            continue;
        }

        Ichips->at(chipIndex)->outputPins[chipPinIndex]->bindTo(*Ichips->at(chipIndex2)->inputPins[chipPinIndex2]);
    }

    // CO to GO
    for (const auto &bindPair : parser.coToGoBinds)
    {
        int chipIndex = bindPair.first[0];
        const auto &secondIndices = bindPair.second;

        int chipPinIndex = bindPair.first[1];
        int GOIndex = secondIndices;

        try
        {
            Ichips->at(chipIndex)->outputPins[chipPinIndex]->bindToGlobalOutput(*globalOutputPins->at(GOIndex));
        }
        catch (const std::exception &e)
        {
            std::cerr << "Exception in parseCOTOGOBinds: " << e.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "Unknown exception in parseCOTOGOBinds." << std::endl;
        }
    }

    ChipStructure *structure = new ChipStructure(new ChipBoundingBox({100, 200, 0, 0}));
    Chip *chip = new Chip(structure, *inputPins, *outputPins, name.substr(0, name.find(".chip")));
    chip->setChipType(CUSTOM);
    CustomChip *customChip = new CustomChip(globalInputPins, globalOutputPins);
    chip->setCustomChip(customChip);
    return chip;
}

void ChipArea::load_chip_to_circuit(std::string &name)
{
    isLoadingChip = true;
    // console
    Chip *chip = load_chip(name);
    if(chip == nullptr)
    {
        std::cerr << "Chip is null" << std::endl;
        return;
    }
    int new_x =  mousePos.x - chip->structure->boundingBox->width/2-30;
    int new_y = height - chip->structure->boundingBox->height-100;

    chip->structure->setLoc(new_x, new_y);
    addChip(chip);
    canvas->queue_draw();
}

void ChipArea::load_each_chip(std::string &filename)
{
    Gtk::Button *btn = Gtk::manage(new Gtk::Button());
    btn->set_label(filename.substr(0, filename.size() - 5));
    chipSelector->append(*btn);

    btn->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &ChipArea::load_chip_to_circuit), filename));
}

void ChipArea::load_all_chips()
{
    std::string currentDir = std::filesystem::current_path().generic_string();
    currentDir += "/" + currentDirName;
    chipFiles = getFilesWithExtension(currentDir, ".chip");
    std::cout << "Current directory: " << currentDir << std::endl;
    for (int i = 0; i < chipFiles.size(); i++)
    {
        std::cout << chipFiles[i] << std::endl;
        load_each_chip(chipFiles[i]);
    }
}

void CustomChip::run()
{
    std::vector<Chip *> chips;
    for (int i = 0; i < globalInputPins->size(); i++)
    {
        for (int j = 0; j < globalInputPins->at(i)->binds->size(); j++)
        {
            globalInputPins->at(i)->binds->at(j)->input.state = globalInputPins->at(i)->state;

            if (globalInputPins->at(i)->binds->at(j)->input.chip != nullptr)
            {
                chips.push_back(globalInputPins->at(i)->binds->at(j)->input.chip);
            }
        }

        for (int j = 0; j < globalInputPins->at(i)->gbinds->size(); j++)
        {
            globalInputPins->at(i)->gbinds->at(j)->output.state = globalInputPins->at(i)->state;
        }

    }

    for (int i = 0; i < chips.size(); i++)
    {
        chips[i]->run();
    }
}

void PinPlusAction::draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    PinAction::draw(cr);
    // plus sign
    // white 
    cr->set_source_rgb(1, 1, 1);
    cr->move_to(x - 10, y);
    cr->line_to(x + 10, y);
    cr->move_to(x, y - 10);
    cr->line_to(x, y + 10);
    cr->stroke();
}

void PinMinusAction::draw(const Cairo::RefPtr<Cairo::Context> &cr)
{
    PinAction::draw(cr);
    // minus sign
    // white
    cr->set_source_rgb(1, 1, 1);
    cr->move_to(x - 10, y);
    cr->line_to(x + 10, y);
    cr->stroke();
}

