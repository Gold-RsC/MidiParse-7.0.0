#ifndef MIDITRACK_HPP
#define MIDITRACK_HPP
#include<vector>
#include"MidiEvent.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Track
        ********************************/
        using MidiTrack=std::vector<MidiEvent>;
        
    }
}
#endif
