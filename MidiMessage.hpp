#ifndef MIDIMESSAGE_HPP
#define MIDIMESSAGE_HPP
#include<vector>
#include"MidiError.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Message
        ********************************/
        using MidiMessage=std::vector<uint8_t> ; 
    }
}
#endif
