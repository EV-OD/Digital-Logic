#include <fstream>
#include "ChipArea.h"
#include <cstring>

void ChipArea::addChip(Chip *chip){
    chip->index = chips->size();
    chips->push_back(*chip);
}

void ChipArea::save_circuit(std::string &name){
    std::fstream file;
    std::string ext = ".chip";
    name += ext;
    file.open(name, std::ios::in|std::ios::binary|std::ios::trunc|std::ios::out);
    int no_of_global_input_pin = globalInputPins->size();
    int no_of_global_output_pin = globalOutputPins->size();
    file << "# Global Input -> " <<  no_of_global_input_pin << std::endl;
    file << "# Global Output -> " <<  no_of_global_output_pin << std::endl;
    file << "# Chips" << std::endl;
    file << "# start" << std::endl;
    for(int i = 0;i<chips->size();i++){
        file << i << "." << " " << chips->at(i).name << std::endl;
        file << "InputPin -> " << chips->at(i).inputPins.size()  << std::endl;
        file << "OutputPin -> " << chips->at(i).outputPins.size()  << std::endl;
        std::string chip_type = (chips->at(i).type == CUSTOM ? "CUSTOM" : "INBUILT" );
        file << "Type -> " << chip_type << std::endl;
    }
    file << "# end" << std::endl;
    file << "# Binds" << std::endl;
    file << "# start" << std::endl;
    int no_of_binds_from_global_input_to_global_output = 0;
    int no_of_binds_from_global_input_to_chip = 0;
    int no_of_binds_from_chip_to_chip = 0;
    int no_of_binds_from_chip_to_global_output = 0;

    for(int i = 0;i<globalInputPins->size();i++){
        no_of_binds_from_global_input_to_global_output += globalInputPins->at(i)->gbinds->size();
        no_of_binds_from_global_input_to_chip += globalInputPins->at(i)->binds->size();
    }

    for(int i = 0;i<chips->size();i++){
        for(int j = 0;j<chips->at(i).outputPins.size();j++){
            no_of_binds_from_chip_to_chip += chips->at(i).outputPins.at(j)->binds->size();
            no_of_binds_from_chip_to_global_output += chips->at(i).outputPins.at(j)->bindsToGlobalOutput->size();
        }
    }

    //global input pin to ..
    // print # GI to GO -> no of all binds from global input to global output
    file << "# GI to GO -> " << no_of_binds_from_global_input_to_global_output << std::endl;
    for(int i = 0;i<globalInputPins->size();i++){
        // print GI[index] -> GO[index]
        // global input pin to global output pin
        for(int j = 0;j<globalInputPins->at(i)->gbinds->size();j++){
            int index = -1;
            for(int k = 0;k<globalOutputPins->size();k++){
                if(globalOutputPins->at(k) == &(globalInputPins->at(i)->gbinds->at(j)->output)){
                    index = k;
                    break;
                }
            }
            file << "GI[" << i << "] -> GO[" << index << "]" << std::endl;
        }
    }

    //print GI[index] -> CI[index][index]
    // global input pin to chip input pin
    file << "# GI to CI -> " << no_of_binds_from_global_input_to_chip << std::endl;
    for(int i = 0;i<globalInputPins->size();i++){
        for(int j = 0;j<globalInputPins->at(i)->binds->size();j++){
            int index = -1;
            for(int k = 0;k<chips->size();k++){
                for(int l = 0;l<chips->at(k).inputPins.size();l++){
                    if(chips->at(k).inputPins.at(l) == &(globalInputPins->at(i)->binds->at(j)->input)){
                        index = k;
                        break;
                    }
                }
            }

            int pinIndex = -1;
            for(int k = 0;k<chips->at(index).inputPins.size();k++){
                if(chips->at(index).inputPins.at(k) == &(globalInputPins->at(i)->binds->at(j)->input)){
                    pinIndex = k;
                    break;
                }
            }
            file << "GI[" << i << "] -> CI[" << index << "][" << pinIndex << "]" << std::endl;
        }
    }

    //print CO[index][index] -> CI[index][index]
    // chip output pin to chip input pin
    file << "# CO to CI -> " << no_of_binds_from_chip_to_chip << std::endl;
    for(int i = 0;i<chips->size();i++){
        for(int j = 0;j<chips->at(i).outputPins.size();j++){
            for(int k = 0;k<chips->at(i).outputPins.at(j)->binds->size();k++){
                int index = -1;
                for(int l = 0;l<chips->size();l++){
                    for(int m = 0;m<chips->at(l).inputPins.size();m++){
                        if(chips->at(l).inputPins.at(m) == &(chips->at(i).outputPins.at(j)->binds->at(k)->input)){
                            index = l;
                            break;
                        }
                    }
                }

                int pinIndex = -1;
                for(int l = 0;l<chips->at(index).inputPins.size();l++){
                    if(chips->at(index).inputPins.at(l) == &(chips->at(i).outputPins.at(j)->binds->at(k)->input)){
                        pinIndex = l;
                        break;
                    }
                }
                file << "CO[" << i << "][" << j << "] -> CI[" << index << "][" << pinIndex << "]" << std::endl;
            }
        }
    }

    //print CO[index][index] -> GO[index]
    // chip output pin to global output pin
    file << "# CO to GO -> " << no_of_binds_from_chip_to_global_output << std::endl;
    for(int i = 0;i<chips->size();i++){
        for(int j = 0;j<chips->at(i).outputPins.size();j++){
            for(int k = 0;k<chips->at(i).outputPins.at(j)->bindsToGlobalOutput->size();k++){
                int index = -1;
                for(int l = 0;l<globalOutputPins->size();l++){
                    if(globalOutputPins->at(l) == &(chips->at(i).outputPins.at(j)->bindsToGlobalOutput->at(k)->output)){
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

