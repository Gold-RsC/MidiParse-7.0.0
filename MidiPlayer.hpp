#ifndef MIDIPLAYER_HPP
#define MIDIPLAYER_HPP
#include<thread>
#include<mutex>
#include<windows.h>
#include<algorithm>
#pragma comment(lib,"winmm.lib")
#include"MidiParser.hpp"
namespace GoldType{
    namespace MidiParse{
        class MidiShortMessage{
            public:
                uint64_t time;
                uint32_t message;
            public:
                MidiShortMessage(uint64_t time=0,uint32_t message=0):time(time),message(message){
                }
                MidiShortMessage(uint64_t _time,const MidiMessage&_message){
                    time=_time;
                    switch(_message.type()){
                        case MidiEventType::note_off:
                        case MidiEventType::note_on:
                        case MidiEventType::key_afterTouch:
                        case MidiEventType::controller:
                        case MidiEventType::pitchWheel:
                        {
                            message=_message[0]|_message[1]<<8|_message[2]<<16;
                            break;
                        }
                        case MidiEventType::program:
                        case MidiEventType::channel_afterTouch:{
                            message=_message[0]|_message[1]<<8;
                            break;
                        }
                        default:{
                            
                        }
                    }
                }
        };
        bool operator==(const MidiShortMessage&a,const MidiShortMessage&b){
            return a.time==b.time&&a.message==b.message;
        }
        bool operator!=(const MidiShortMessage&a,const MidiShortMessage&b){
            return !(a==b);
        }
        bool operator<(const MidiShortMessage&a,const MidiShortMessage&b){
            if(a.time!=b.time){
                return a.time<b.time;
            }
            return a.message<b.message;
        }
        bool operator>(const MidiShortMessage&a,const MidiShortMessage&b){
            if(a.time!=b.time){
                return a.time>b.time;
            }
            return a.message>b.message;
        }
        bool operator<=(const MidiShortMessage&a,const MidiShortMessage&b){
            return !(a>b);
        }
        bool operator>=(const MidiShortMessage&a,const MidiShortMessage&b){
            return !(a<b);
        }
        class MidiShortMessageList:public std::vector<MidiShortMessage>{
            private:
            public:
                using std::vector<MidiShortMessage>::vector;
                MidiShortMessageList(const MidiFile&_file){
                    if(_file.is_read_success()){
                        for(size_t trackIdx=0;trackIdx<_file.head.ntracks;++trackIdx){
                            const MidiTrack&track=_file[trackIdx];
                            uint64_t time=0;
                            for(size_t eventIdx=0;eventIdx<track.size();++eventIdx){
                                const MidiEvent&event=track[eventIdx];
                                time+=event.time;
                                if(event.is_normal()){
                                    emplace_back(time,event.message);
                                }
                            }
                        }
                        std::sort(this->begin(),this->end());
                    }
                }
                MidiShortMessageList(const MidiParser&_parser){
                    NoteMap noteMap=_parser.noteMap();
                    if(_parser.timeMode()==MidiTimeMode::microsecond){
                        *this=MidiShortMessageList(noteMap);
                    }
                    else{
                        _parser.change_timeMode(noteMap,MidiTimeMode::microsecond);
                        *this=MidiShortMessageList(noteMap);
                    }
                }
                MidiShortMessageList(const std::string&_filename){
                    MidiFile file(_filename);
                    file.read();
                    *this=MidiShortMessageList(file);
                }
                MidiShortMessageList(NoteList _noteList){
                    _noteList.sort();
                    this->reserve(_noteList.size()+_noteList.size()/10);
                    uint8_t instruments[16]={0};
                    for(const Note&note:_noteList){
                        if(note.timeMode==MidiTimeMode::tick){
                            continue;
                        }
                        if(note.instrument!=instruments[note.channel]){
                            instruments[note.channel]=note.instrument;
                            emplace_back(note.time,(note.instrument<<8)|0xC0|note.channel);
                        }
                        if(note.velocity){
                            emplace_back(note.time,(note.velocity<<16)|(note.pitch<<8)|0x90|note.channel);
                        }
                        else{
                            emplace_back(note.time,(note.pitch<<8)|0x80|note.channel);
                        }
                    }
                    std::sort(this->begin(),this->end());
                }
                MidiShortMessageList(const NoteMap&_map):MidiShortMessageList(event_map_to_list(_map)){
                }
        };
        class MidiPlayer{
            private:
                // std::thread m_thread;
                // std::mutex m_mutex;
                // std::atomic<double> m_speed;
                MidiShortMessageList m_messages;
            public:
                MidiPlayer(void){
                    
                }
                MidiPlayer(const MidiShortMessageList&_messages):m_messages(_messages){
                }
                MidiPlayer(const std::string&_filename):m_messages(_filename){
                }
                MidiPlayer(const NoteList&_noteList):m_messages(_noteList){
                }
                MidiPlayer(const NoteMap&_noteMap):m_messages(_noteMap){
                }
                ~MidiPlayer(void){
                    
                }
            public:
                void play(void)const {
                    HMIDIOUT handle;
                    midiOutOpen(&handle,0,0,0,CALLBACK_NULL);
                    uint64_t lastTime=0;
                    for(const MidiShortMessage&msg:m_messages){
                        uint64_t waitTime=(uint64_t)((msg.time-lastTime)/1000);
                        lastTime=msg.time;
                        if(waitTime){
                            Sleep((DWORD)waitTime);
                        }
                        midiOutShortMsg(handle,msg.message);
                    }
                    Sleep((DWORD)(5000));
                    midiOutClose(handle);
                }
        };
    }
}            
#endif
