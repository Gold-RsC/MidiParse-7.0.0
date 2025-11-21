#ifndef MOREMIDIEVENT_HPP
#define MOREMIDIEVENT_HPP
#include"MidiEvent.hpp"
#include<string>
#include<algorithm>
#include<map>
#include<queue>
namespace GoldType{
    namespace MidiParse{
        /********************************
                    More Midi Event
        ********************************/
        class Note:public BasicMidiEvent_Non{
            public:
                uint8_t pitch;
                uint8_t velocity;
                uint8_t instrument;
                double bar;
                double beat;
            public:
                Note(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint8_t _channel=0,uint8_t _pitch=0,uint8_t _velocity=0,uint8_t _instrument=0,double _bar=0,double _beat=0):
                    BasicMidiEvent_Non(_time,_timeMode,_track,_channel),pitch(_pitch),velocity(_velocity),instrument(_instrument),bar(_bar),beat(_beat){}
                Note(const Note&)=default;
                ~Note(void)=default;
            public:
                bool is_empty(void)const {
                    return !(time)&&!(channel)&&!(track)&&!(pitch)&&!(velocity)&&(!instrument);
                }
        };
        bool operator==(const Note&a,const Note&b){
            return a.time==b.time&&
            a.timeMode==b.timeMode&&
            a.channel==b.channel&&
            a.track==b.track&&
            a.pitch==b.pitch&&
            a.velocity==b.velocity&&
            a.instrument==b.instrument;
        }
        bool operator!=(const Note&a,const Note&b){
            return !(a==b);
        }
        bool operator<(const Note&a,const Note&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.channel!=b.channel){
                return a.channel<b.channel;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            if(a.velocity!=b.velocity){
                return a.velocity<b.velocity;
            }
            if(a.pitch!=b.pitch){
                return a.pitch<b.pitch;
            }
            return a.instrument<b.instrument;
        }
        bool operator>(const Note&a,const Note&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.channel!=b.channel){
                return a.channel>b.channel;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            if(a.pitch!=b.pitch){
                return a.pitch>b.pitch;
            }
            if(a.velocity!=b.velocity){
                return a.velocity>b.velocity;
            }
            return a.instrument>b.instrument;
        }
        bool operator<=(const Note&a,const Note&b){
            return !(a>b);
        }
        bool operator>=(const Note&a,const Note&b){
            return !(a<b);
        }
        
        class Program:public BasicMidiEvent_Non{
            public:
                uint8_t instrument;
            public:
                Program(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint8_t _channel=0,uint8_t _instrument=0):
                    BasicMidiEvent_Non(_time,_timeMode,_track,_channel),instrument(_instrument){}
                Program(const Program&)=default;
                ~Program(void)=default;
            public:
        };
        bool operator==(const Program&a,const Program&b){
            return a.time==b.time&&
            a.timeMode==b.timeMode&&
            a.channel==b.channel&&a.track==b.track&&a.instrument==b.instrument;
        }
        bool operator!=(const Program&a,const Program&b){
            return !(a==b);
        }
        bool operator<(const Program&a,const Program&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.channel!=b.channel){
                return a.channel<b.channel;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            return a.instrument<b.instrument;
        }
        bool operator>(const Program&a,const Program&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.channel!=b.channel){
                return a.channel>b.channel;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            return a.instrument>b.instrument;
        }
        bool operator<=(const Program&a,const Program&b){
            return !(a>b);
        }
        bool operator>=(const Program&a,const Program&b){
            return !(a<b);
        }
        
        class Text:public BasicMidiEvent_Meta{
            public:
                uint8_t type;
                std::string text;
            public:
                Text(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint8_t _type=0,std::string&&_text=""):
                    BasicMidiEvent_Meta(_time,_timeMode,_track),type(_type),text(std::forward<std::string>(_text)){}
                Text(const Text&)=default;
                ~Text(void)=default;
        };
        bool operator==(const Text&a,const Text&b){
            return a.time==b.time&&
            a.timeMode==b.timeMode&&
            a.track==b.track&&
            a.type==b.type&&
            a.text==b.text;
        }
        bool operator!=(const Text&a,const Text&b){
            return !(a==b);
        }
        bool operator<(const Text&a,const Text&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            if(a.type!=b.type){
                return a.type<b.type;
            }
            return a.text<b.text;
        }
        bool operator>(const Text&a,const Text&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            if(a.type!=b.type){
                return a.type>b.type;
            }
            return a.text>b.text;
        }
        bool operator<=(const Text&a,const Text&b){
            return !(a>b);
        }
        bool operator>=(const Text&a,const Text&b){
            return !(a<b);
        }
        
        class Tempo:public BasicMidiEvent_Meta{
            public:
                uint32_t mispqn;
                uint64_t timeNode;
            public:
                Tempo(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint32_t _mispqn=0x07A120,uint64_t _timeNode=0):
                    BasicMidiEvent_Meta(_time,_timeMode,_track),mispqn(_mispqn),timeNode(_timeNode){}
                Tempo(const Tempo&)=default;
                ~Tempo(void)=default;
            public:
                double bpm(void)const{
                    return 6e7/mispqn;
                }
                uint64_t&tick(void){
                    if(timeMode==MidiTimeMode::tick){
                        return time;
                    }
                    return timeNode;
                }
                const uint64_t&tick(void)const {
                    if(timeMode==MidiTimeMode::tick){
                        return time;
                    }
                    return timeNode;
                }
                uint64_t&microsecond(void){
                    if(timeMode==MidiTimeMode::microsecond){
                        return time;
                    }
                    return timeNode;
                }
                const uint64_t&microsecond(void)const {
                    if(timeMode==MidiTimeMode::microsecond){
                        return time;
                    }
                    return timeNode;
                }
        };
        bool operator==(const Tempo&a,const Tempo&b){
            return a.timeMode==b.timeMode&&a.time==b.time&&a.track==b.track&&a.mispqn==b.mispqn;
        }
        bool operator!=(const Tempo&a,const Tempo&b){
            return !(a==b);
        }
        bool operator<(const Tempo&a,const Tempo&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            return a.mispqn<b.mispqn; 
        }
        bool operator>(const Tempo&a,const Tempo&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            return a.mispqn>b.mispqn; 
        }
        bool operator<=(const Tempo&a,const Tempo&b){
            return !(a>b);
        }
        bool operator>=(const Tempo&a,const Tempo&b){
            return !(a<b);
        }

        class TimeSignature:public BasicMidiEvent_Meta{
            public:
                uint8_t numerator;
                uint8_t denominator;
                uint8_t tickPerMidiclock;
                uint8_t num32ndNotePer24Midiclock;
            public:
                TimeSignature(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,uint8_t _numerator=4,uint8_t _denominator=4,uint8_t _tickPerMidiclock=24,uint8_t _num32ndNotePer24Midiclock=8):
                    BasicMidiEvent_Meta(_time,_timeMode,_track),numerator(_numerator),denominator(_denominator),tickPerMidiclock(_tickPerMidiclock),num32ndNotePer24Midiclock(_num32ndNotePer24Midiclock){}
                TimeSignature(const TimeSignature&)=default;
                ~TimeSignature(void)=default;
        };
        bool operator==(const TimeSignature&a,const TimeSignature&b){
            return a.time==b.time&&
            a.timeMode==b.timeMode&&
            a.track==b.track&&a.numerator==b.numerator&&a.denominator==b.denominator&&a.tickPerMidiclock==b.tickPerMidiclock&&a.num32ndNotePer24Midiclock==b.num32ndNotePer24Midiclock;
        }
        bool operator!=(const TimeSignature&a,const TimeSignature&b){
            return !(a==b);
        }
        bool operator<(const TimeSignature&a,const TimeSignature&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            if(a.numerator!=b.numerator){
                return a.numerator<b.numerator; 
            }
            if(a.denominator!=b.denominator){
                return a.denominator<b.denominator; 
            }
            if(a.tickPerMidiclock!=b.tickPerMidiclock){
                return a.tickPerMidiclock<b.tickPerMidiclock; 
            }
            return a.num32ndNotePer24Midiclock<b.num32ndNotePer24Midiclock;
        }
        bool operator>(const TimeSignature&a,const TimeSignature&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            if(a.numerator!=b.numerator){
                return a.numerator>b.numerator; 
            }
            if(a.denominator!=b.denominator){
                return a.denominator>b.denominator; 
            }
            if(a.tickPerMidiclock!=b.tickPerMidiclock){
                return a.tickPerMidiclock>b.tickPerMidiclock; 
            }
            return a.num32ndNotePer24Midiclock>b.num32ndNotePer24Midiclock;
        }
        bool operator<=(const TimeSignature&a,const TimeSignature&b){
            return !(a>b);
        }
        bool operator>=(const TimeSignature&a,const TimeSignature&b){
            return !(a<b);
        }
        
        class BarBeat:public BasicMidiEvent_Meta{
            public:
                double barNode;
                double beatNode;
                uint8_t numerator;
                uint8_t denominator;
            public:
                BarBeat(uint64_t _time=0,MidiTimeMode _timeMode=MidiTimeMode::tick,uint8_t _track=0,double _barNode=0,double _beatNode=0,uint8_t _numerator=4,uint8_t _denominator=4):
                    BasicMidiEvent_Meta(_time,_timeMode,_track),barNode(_barNode),beatNode(_beatNode),numerator(_numerator),denominator(_denominator){}
                BarBeat(const BarBeat&)=default;
                ~BarBeat(void)=default;
        };
        bool operator==(const BarBeat&a,const BarBeat&b){
            return a.time==b.time&&
            a.timeMode==b.timeMode&&
            a.track==b.track&&a.barNode==b.barNode&&a.barNode==b.barNode;
        }
        bool operator!=(const BarBeat&a,const BarBeat&b){
            return !(a==b);
        }
        bool operator<(const BarBeat&a,const BarBeat&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode<b.timeMode;
            }
            if(a.time!=b.time){
                return a.time<b.time;
            }
            if(a.track!=b.track){
                return a.track<b.track;
            }
            if(a.barNode!=b.barNode){
                return a.barNode<b.barNode; 
            }
            if(a.barNode!=b.barNode){
                return a.barNode<b.barNode; 
            }
            if(a.numerator!=b.numerator){
                return a.numerator<b.numerator;
            }
            return a.denominator<b.denominator;
        }
        bool operator>(const BarBeat&a,const BarBeat&b){
            if(a.timeMode!=b.timeMode){
                return a.timeMode>b.timeMode;
            }
            if(a.time!=b.time){
                return a.time>b.time;
            }
            if(a.track!=b.track){
                return a.track>b.track;
            }
            if(a.barNode!=b.barNode){
                return a.barNode>b.barNode; 
            }
            if(a.barNode!=b.barNode){
                return a.barNode>b.barNode; 
            }
            if(a.numerator!=b.numerator){
                return a.numerator>b.numerator;
            }
            return a.denominator>b.denominator;
        }
        bool operator<=(const BarBeat&a,const BarBeat&b){
            return !(a>b);
        }
        bool operator>=(const BarBeat&a,const BarBeat&b){
            return !(a<b);
        }
        
        template<typename _MidiEvent>
        class MidiEventList:public std::vector<_MidiEvent>{
            public:
                using std::vector<_MidiEvent>::vector;
            public:
                MidiTimeMode get_timeMode(void)const {
                    if(this->size()){
                        MidiTimeMode ret=this->front().timeMode;
                        for(size_t i=1;i<this->size();++i){
                            if(ret!=this->operator[](i).timeMode){
                                return MidiTimeMode::tick;
                            }
                        }
                        return ret;
                    }
                    return MidiTimeMode::tick;
                }
                void sort(void){
                    std::sort(this->begin(),this->end());
                }
        };

        using NoteList=MidiEventList<Note>;
        using TextList=MidiEventList<Text>;
        using TempoList=MidiEventList<Tempo>;
        using TsList=MidiEventList<TimeSignature>;
        using ProgramList=MidiEventList<Program>;
        using BarBeatList=MidiEventList<BarBeat>;
        

        template<typename _MidiEvent>
        class MidiEventMap:public std::vector<MidiEventList<_MidiEvent>>{
            public:
                using std::vector<MidiEventList<_MidiEvent>>::vector;
            public:
                MidiTimeMode get_timeMode(void)const {
                    if(this->size()){
                        MidiTimeMode ret=this->front().get_timeMode();
                        for(size_t i=1;i<this->size();++i){
                            if(ret!=this->operator[](i).get_timeMode()){
                                return MidiTimeMode::tick;
                            }
                        }
                        return ret;
                    }
                    return MidiTimeMode::tick;
                }
                void sort(void){
                    for(size_t i=0;i<this->size();++i){
                        this->operator[](i).sort();
                    }
                }
        };
        template<typename _MidiEvent>
        MidiEventList<_MidiEvent> event_map_to_list(const MidiEventMap<_MidiEvent>&_map){
            size_t _size=0;
            for(size_t i=0;i<_map.size();++i){
                _size+=_map[i].size();
            }
            MidiEventList<_MidiEvent> ret;
            ret.reserve(_size);
            for(uint8_t trackIdx=0;trackIdx<_map.size();++trackIdx){
                ret.insert(ret.end(),_map[trackIdx].begin(),_map[trackIdx].end());
            }
            return ret;
        }
        template<typename _MidiEvent>
        MidiEventMap<_MidiEvent> event_list_to_map(const MidiEventList<_MidiEvent>&_list){
            size_t _size[128]={0};
            uint8_t _max_track=0;
            for(size_t i=0;i<_list.size();++i){
                ++_size[_list[i].track];
                _max_track=std::max(_list[i].track,_max_track);
            }
            MidiEventMap<_MidiEvent> ret(_max_track+1);
            for(size_t i=0;i<=_max_track;++i){
                ret[i].reserve(_size[i]);
            }
            for(size_t i=0;i<_list.size();++i){
                ret[_list[i].track].emplace_back(_list[i]);
            }
            return ret;
        }
        
        template<typename _MidiEvent>
        uint8_t get_ntracks(const MidiEventList<_MidiEvent>&_list){
            uint8_t _num=0;
            for(typename MidiEventList<_MidiEvent>::iterator it=_list.begin();it!=_list.end();++it){
                _num=max(_num,it->track);
            }
            return _num+1;
        }
        template<typename _MidiEvent>
        uint8_t get_ntracks(const MidiEventMap<_MidiEvent>&_map){
            return _map.size();
        }
        
        template<typename _MidiEvent,typename _Fun,typename..._Args>
        void for_event(MidiEventList<_MidiEvent>&_list,_Fun&&_fun,_Args&&..._args){
            for(size_t eventIdx=0;eventIdx<_list.size();++eventIdx){
                _fun(_list[eventIdx],std::forward(_args)...);
            }
        }
        template<typename _MidiEvent,typename _Fun,typename..._Args>
        void for_event(MidiEventMap<_MidiEvent>&_map,_Fun&&_fun,_Args&&..._args){
            for(uint8_t trackIdx=0;trackIdx<_map.size();++trackIdx){
                for(size_t eventIdx=0;eventIdx<_map[trackIdx].size();++eventIdx){
                    _fun(_map[trackIdx][eventIdx],std::forward(_args)...);
                }
            }
        }

        using NoteMap=MidiEventMap<Note>;
        using TextMap=MidiEventMap<Text>;
        using TempoMap=MidiEventMap<Tempo>;
        using TsMap=MidiEventMap<TimeSignature>;
        using ProgramMap=MidiEventMap<Program>;
        using BarBeatMap=MidiEventMap<BarBeat>;
    }
}
#endif
