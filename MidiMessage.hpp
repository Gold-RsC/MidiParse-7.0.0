#ifndef MIDIMESSAGE_HPP
#define MIDIMESSAGE_HPP
#include<vector>
#include"MidiError.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Message
        ********************************/
        enum class MidiEventType:uint8_t{
            null=0x00,
            /******************
            Part1:type = 0xNx (N = 8~E ,x = 0~F)
            type & 0xF0 = 0xN0
            ******************/
            note_off=0x80,              // 2
            note_on=0x90,               // 2
            key_afterTouch=0xA0,        // 2
            controller=0xB0,            // 2
            program=0xC0,               // 1
            channel_afterTouch=0xD0,    // 1
            pitchWheel=0xE0,            // 2
            
            /*******************
            Part2:type = 0xF0 or 0xF7
            *******************/
            sysex_begin=0xF0,sysex_end=0xF7,
            
            /*******************
            Part3:type = 0xFF
            *******************/
            meta=0xFF
        };
        class MidiMessage:public std::vector<uint8_t>{
            public:
                using std::vector<uint8_t>::vector;
                MidiEventType type(void)const {
                    if(operator[](0)==0xFF||operator[](0)==0xF0||operator[](0)==0xF7){
                        return MidiEventType(operator[](0));
                    }
                    else{
                        return MidiEventType(operator[](0)&0xF0);
                    }
                }
                uint8_t channel(void)const {
                    #ifdef MIDI_DEBUG
                    if(operator[](0)<0xF0&&operator[](0)>0x7F){
                    #endif
                        return operator[](0)&0x0F;
                    #ifdef MIDI_DEBUG
                    }
                    return 0xFF;
                    #endif
                }
        };
    }
}
#endif
