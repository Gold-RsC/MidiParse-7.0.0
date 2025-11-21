#ifndef MIDIEVENT_HPP
#define MIDIEVENT_HPP
#include"MidiMessage.hpp"
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Event
        ********************************/
        
        
        enum class MidiTimeMode:bool{
            tick=0,
            microsecond=1
        };
        
        class BasicMidiEvent{
            public:
                static uint64_t __time_error_v;
                uint64_t time;
                MidiTimeMode timeMode:1;
                uint8_t track:7;
            public:
                BasicMidiEvent(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0):
                    time(_time),timeMode(_timeMode),track(_track){}
                BasicMidiEvent(const BasicMidiEvent&)=default;
                virtual ~BasicMidiEvent(void)=default;
            public:
                virtual uint64_t&tick(void){
                    if(timeMode==MidiTimeMode::tick){
                        return time;
                    }
                    return __time_error_v;
                }
                virtual const uint64_t&tick(void)const {
                    if(timeMode==MidiTimeMode::tick){
                        return time;
                    }
                    return __time_error_v;
                }
                virtual uint64_t&microsecond(void){
                    if(timeMode==MidiTimeMode::microsecond){
                        return time;
                    }
                    return __time_error_v;
                }
                virtual const uint64_t&microsecond(void)const {
                    if(timeMode==MidiTimeMode::microsecond){
                        return time;
                    }
                    return __time_error_v;
                }
        };
        uint64_t BasicMidiEvent::__time_error_v=uint64_t(-1);
        
        class BasicMidiEvent_Non:public BasicMidiEvent{
            public:
                uint8_t channel:4;
            public:
                BasicMidiEvent_Non(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint8_t _channel=0):
                    BasicMidiEvent(_time,_timeMode,_track),channel(_channel){}
                BasicMidiEvent_Non(const BasicMidiEvent_Non&)=default;
                virtual ~BasicMidiEvent_Non(void)=default;
        };
        class BasicMidiEvent_Meta:public BasicMidiEvent{
            public:
                BasicMidiEvent_Meta(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0):
                    BasicMidiEvent(_time,_timeMode,_track){}
                BasicMidiEvent_Meta(const BasicMidiEvent_Meta&)=default;
                virtual ~BasicMidiEvent_Meta(void)=default;
        };
        
        class MidiEvent:public BasicMidiEvent {
            public:
                MidiMessage message;
            public:
                MidiEvent(void):
                    message() {}
                MidiEvent(const MidiEvent&another)=default;
                MidiEvent(const MidiMessage&_message):
                    message(_message) {}
                ~MidiEvent(void){}
            public:
                MidiEventType type(void)const {
                    return message.type();
                }
                uint8_t channel(void)const {
                    return message.channel();
                }
            public:
                bool is_normal(void)const{
                    return !(is_meta()||is_sysex());
                }
                bool is_meta(void)const {
                    return type()==MidiEventType::meta;
                }
                bool is_sysex(void)const{
                    return type()==MidiEventType::sysex_begin||type()==MidiEventType::sysex_end;
                }
            public:
                uint8_t&operator[](size_t idx) {
                    return message[idx];
                }
                const uint8_t&operator[](size_t idx) const{
                    return message[idx];
                }
                MidiEvent&operator=(const MidiEvent&another)=default;
        };
        
        template<>
        MidiError&MidiError::operator()<MidiEvent>(const MidiEvent&_midiEvent){
            MidiErrorType type=MidiErrorType::noError;
            
            switch(_midiEvent.type()){
                case MidiEventType::note_off:
                case MidiEventType::note_on:
                case MidiEventType::key_afterTouch:
                case MidiEventType::controller:
                case MidiEventType::pitchWheel:{
                    if(_midiEvent[1]&0x80){
                        return operator()(MidiErrorType((uint8_t)type|0x01));
                    }
                    if(_midiEvent[2]&0x80){
                        return operator()(MidiErrorType((uint8_t)type|0x02));
                    }
                    break;
                }
                case MidiEventType::program:
                case MidiEventType::channel_afterTouch:{
                    if(_midiEvent[1]&0x80){
                        return operator()(MidiErrorType((uint8_t)type|0x01));
                    }
                    break;
                }
                case MidiEventType::sysex_begin:
                case MidiEventType::sysex_end:{
                    size_t i=1;
                    uint32_t length=0;
                    for(;i<5;++i) {
                        length<<=7;
                        length|=(_midiEvent[i]&0x7F);
                        if(_midiEvent[i]<0x80) {
                            break;
                        }
                    }
                    if(i+length!=_midiEvent.message.size()){
                        return operator()(MidiErrorType::sysex);
                    }
                    break;
                }
                case MidiEventType::meta:{
                    size_t i=2;
                    uint32_t length=0;
                    for(;i<5;++i) {
                        length<<=7;
                        length|=(_midiEvent[i]&0x7F);
                        if(_midiEvent[i]<0x80) {
                            break;
                        }
                    }
                    if(i+length!=_midiEvent.message.size()){
                        return operator()(MidiErrorType::sysex);
                    }
                    break;
                }
                default:{
                    return operator()(MidiErrorType::event_type);
                }
            }
            return operator()(MidiErrorType::noError);
        }
    }
}
#endif
