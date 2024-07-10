#include <fstream>
#include "ChipArea.h"

void ChipArea::save_circuit(std::string &name){
    std::fstream file;
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
    for(int i = 0;i<globalInputPins->size();i++){
        // global input to global output
        file << "# GI to GO ->" << globalInputPins->at(i)->gbinds->size() << std::endl;
        for(int j = 0;j<globalInputPins->at(i)->gbinds->size();j++){
            // globalInputPins->at(i)->gbinds
        }
    }
    file << "# start" << std::endl;
    file << "# end" << std::endl;
}

