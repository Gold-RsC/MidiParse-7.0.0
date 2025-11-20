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
                MidiShortMessageList(NoteList _noteList){
                    _noteList.sort();
                    this->reserve(_noteList.size()+_noteList.size()/10);
                    uint8_t instruments[16]={0};
                    for(const Note&note:_noteList){
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
                }
            public:
                void sort(void){
                    std::sort(this->begin(),this->end());
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
                MidiPlayer(const NoteList&_noteList):m_messages(_noteList){
                    m_messages.sort();
                }
                ~MidiPlayer(void){
                    
                }
            public:
                void play(void){
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
