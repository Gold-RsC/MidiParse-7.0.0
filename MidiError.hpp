#ifndef MIDIERROR_HPP
#define MIDIERROR_HPP
// #define MIDI_DEBUG
#include<stdio.h>
#include<stdint.h>

#ifdef MIDI_DEBUG_WARNING
#ifndef MIDI_DEBUG
#define MIDI_DEBUG
#endif
#endif
namespace GoldType{
    namespace MidiParse{
        /********************************
                    Midi Error
        ********************************/
        
        enum class MidiErrorType:uint8_t{
            noError=0x00,
            filename=0x01,
            head_identification=0x02,
            head_length=0x03,
            head_format=0x04,
            head_ntracks=0x05,
            head_division=0x06,
            track_identification=0x07,
            track_length=0x08,
            event_deltaTime=0x09,
            event_type=0x0A,
            
            parse_error=0x10,
            change_timeMode=0x11,
            
            noteOff_pitch=0x81,
            noteOff_velocity=0x82,
            noteOn_pitch=0x91,
            noteOn_velocity=0x92,
            keyAfterTouch_pitch=0xA1,
            keyAfterTouch_velocity=0xA2,
            controller_number=0xB1,
            controller_value=0xB2,
            program_number=0xC1,
            channelAfterTouch_velocity=0xD1,
            pitchWheel_mod=0xE1,
            pitchWheel_div=0xE2,
            sysex=0xF0,
            meta=0xFF
        };
        class MidiError{
            protected:
                MidiErrorType m_mErrType;
            public:
                MidiError(MidiErrorType _mErrType=MidiErrorType::noError):
                    m_mErrType(_mErrType){}
            public:
                MidiErrorType&type(void){
                    return m_mErrType;
                }
                const MidiErrorType&type(void)const{
                    return m_mErrType;
                }
            public:
                MidiError&operator()(MidiErrorType _mErrType){
                    m_mErrType=_mErrType;
                    return *this;
                }
                template<typename _MidiClass>
                MidiError&operator()(const _MidiClass&){
                    return *this;
                }
        };
        MidiError midiError;
        #ifdef MIDI_DEBUG
        class MidiDebug{
            protected:
                FILE*m_file;
            public:
                MidiDebug(void):
                    m_file(stderr){}
                MidiDebug(const char*_name):
                    m_file(fopen(_name,"w")){}
                ~MidiDebug(void){
                    if(m_file!=stderr){
                        fclose(m_file);
                    }
                    m_file=nullptr;
                }
            public:
                void replace(const char*_name){
                    this->~MidiDebug();
                    m_file=fopen(_name,"w");
                }
                void write(void){
                    write_type(midiError.type());
                }
                void write_type(MidiErrorType _mErrType){
                    fprintf(m_file,"MidiError:\n\t");
                    switch(_mErrType){
                        case MidiErrorType::noError:{
                            fprintf(m_file,"No error!\n");
                            break;
                        }
                        case MidiErrorType::filename:{
                            fprintf(m_file,"There is no file named this!\n");
                            break;
                        }
                        case MidiErrorType::head_identification:{
                            fprintf(m_file,"An error occurred in the head trunk!\n\t");
                            fprintf(m_file,"Head trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::head_length:{
                            fprintf(m_file,"An error occurred in the head trunk!\n\t");
                            fprintf(m_file,"Head trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::head_format:{
                            fprintf(m_file,"An error occurred in the head trunk!\n\t");
                            fprintf(m_file,"Head trunk format error!\n");
                            break;
                        }
                        case MidiErrorType::head_ntracks:{
                            fprintf(m_file,"An error occurred in the head trunk!\n\t");
                            fprintf(m_file,"Head trunk track count error!\n");
                            break;
                        }
                        case MidiErrorType::head_division:{
                            fprintf(m_file,"An error occurred in the head trunk!\n\t");
                            fprintf(m_file,"Head trunk division error!\n");
                            break;
                        }
                        case MidiErrorType::track_identification:{
                            fprintf(m_file,"An error occurred in a track trunk!\n\t");
                            fprintf(m_file,"Track trunk identification error!\n");
                            break;
                        }
                        case MidiErrorType::track_length:{
                            fprintf(m_file,"An error occurred in a track trunk!\n\t");
                            fprintf(m_file,"Track trunk length error!\n");
                            break;
                        }
                        case MidiErrorType::event_deltaTime:{
                            fprintf(m_file,"An error occurred in an event!\n\t");
                            fprintf(m_file,"Event delta time error!\n");
                            break;
                        }
                        case MidiErrorType::event_type:{
                            fprintf(m_file,"An error occurred in an event!\n\t");
                            fprintf(m_file,"Event type error!\n");
                            break;
                        }
                        case MidiErrorType::noteOff_pitch:{
                            fprintf(m_file,"An error occurred in a note_off event!\n\t");
                            fprintf(m_file,"Pitch error!\n");
                            break;
                        }
                        case MidiErrorType::noteOff_velocity:{
                            fprintf(m_file,"An error occurred in a note_off event!\n\t");
                            fprintf(m_file,"Velocity error!\n");
                            break;
                        }
                        case MidiErrorType::noteOn_pitch:{
                            fprintf(m_file,"An error occurred in a note_on event!\n\t");
                            fprintf(m_file,"Pitch error!\n");
                            break;
                        }
                        case MidiErrorType::noteOn_velocity:{
                            fprintf(m_file,"An error occurred in a note_on event!\n\t");
                            fprintf(m_file,"Velocity error!\n");
                            break;
                        }
                        case MidiErrorType::keyAfterTouch_pitch:{
                            fprintf(m_file,"An error occurred in a key_after_touch event!\n\t");
                            fprintf(m_file,"Pitch error!\n");
                            break;
                        }
                        case MidiErrorType::keyAfterTouch_velocity:{
                            fprintf(m_file,"An error occurred in a key_after_touch event!\n\t");
                            fprintf(m_file,"Velocity error!\n");
                            break;
                        }
                        case MidiErrorType::controller_number:{
                            fprintf(m_file,"An error occurred in a controller event!\n\t");
                            fprintf(m_file,"Number error!\n");
                            break;
                        }
                        case MidiErrorType::controller_value:{
                            fprintf(m_file,"An error occurred in a controller event!\n\t");
                            fprintf(m_file,"Value error!\n");
                            break;
                        }
                        case MidiErrorType::program_number:{
                            fprintf(m_file,"An error occurred in a program event!\n\t");
                            fprintf(m_file,"Number error!\n");
                            break;
                        }
                        case MidiErrorType::channelAfterTouch_velocity:{
                            fprintf(m_file,"An error occurred in a channel_after_touch event!\n\t");
                            fprintf(m_file,"Velocity error!\n");
                            break;
                        }
                        case MidiErrorType::pitchWheel_mod:{
                            fprintf(m_file,"An error occurred in a pitchWheel event!\n\t");
                            fprintf(m_file,"Mod number error!\n");
                            break;
                        }
                        case MidiErrorType::pitchWheel_div:{
                            fprintf(m_file,"An error occurred in a pitchWheel event!\n\t");
                            fprintf(m_file,"Div number error!\n");
                            break;
                        }
                        case MidiErrorType::sysex:{
                            fprintf(m_file,"An error occurred in a sysex event!\n");
                            break;
                        }
                        case MidiErrorType::meta:{
                            fprintf(m_file,"An error occurred in a meta event!\n");
                            break;
                        }
                        default:{
                            fprintf(m_file,"Unknown error!\n");
                            break;
                        }
                    }
                }
                void write_text(const char*_text){
                    fprintf(m_file,_text);
                }
            public:
                MidiDebug&operator<<(MidiErrorType _mErrType){
                    write_type(_mErrType);
                    return *this;
                }
                MidiDebug&operator<<(MidiError _midiError){
                    write_type(_midiError.type());
                    return *this;
                }
                MidiDebug&operator<<(const char*_text){
                    write_text(_text);
                    return *this;
                }
                template<typename...Args>
                MidiDebug&operator()(const char*_format,Args&&..._args){
                    fprintf(m_file,_format,_args...);
                    return *this;
                }
        };
        MidiDebug midiDebug;
        #endif
    }
}
#endif
