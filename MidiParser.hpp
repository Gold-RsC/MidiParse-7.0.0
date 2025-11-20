#ifndef MIDIPARSER_HPP
#define MIDIPARSER_HPP
#include"MidiFile.hpp"
#include"MoreMidiEvent.hpp"
namespace GoldType{
    namespace MidiParse{
        class MidiParser{
            private:
                MidiTimeMode m_timeMode;
                MidiHead m_head;
            public:
                TempoMap tempoMap;
                BarBeatMap bbMap;
                NoteMap noteMap;
                TextMap textMap;
            private:
                uint8_t get_metaTrack(uint8_t _track)const{
                    if(m_head.format==0||m_head.format==0x01){
                        return 0;
                    }
                    else if(m_head.format==0x02){
                        return _track;
                    }
                    return 0xFF;
                }
                
				MidiErrorType parse_tick(const MidiFile&m_midi){
                    uint8_t instruments[16]={0};
                    for(size_t trackIdx=0;trackIdx<m_head.ntracks;++trackIdx){
                        uint64_t time=0;
                        uint8_t metaTrack=get_metaTrack(trackIdx);
                        if(metaTrack==0xFF){
                            return MidiErrorType::parse_error;
                        }

                        for(size_t eventIdx=0;eventIdx<m_midi[trackIdx].size();++eventIdx){
                            const MidiEvent&event=m_midi[trackIdx][eventIdx];
                            time+=event.time;
                            uint8_t channel=event.channel();
                            switch(event.type()){
                                case MidiEventType::note_off:
                                case MidiEventType::note_on:{
                                    double bar,beat;
                                    if(bbMap[metaTrack].size()){
                                        double deltaBeat=(time-bbMap[metaTrack].back().time)
                                                        *bbMap[metaTrack].back().denominator
                                                        /4.0/m_head.tpqn();
                                        beat=bbMap[metaTrack].back().beatNode+deltaBeat;
                                        bar=bbMap[metaTrack].back().barNode
                                           +deltaBeat/bbMap[metaTrack].back().numerator;
                                    }
                                    else{
                                        beat=time*1.0/m_head.tpqn();
                                        bar=beat/4;
                                    }
                                    noteMap[trackIdx].emplace_back(
                                        time,
                                        MidiTimeMode::tick,
                                        trackIdx,
                                        channel,
                                        event[1],
                                        (event.type()==MidiEventType::note_on)?event[2]:0,
                                        instruments[channel],
                                        bar,
                                        beat
                                    );
                                    break;
                                }
                                case MidiEventType::key_afterTouch:{
                                    break;
                                }
                                case MidiEventType::controller:{
                                    break;
                                }
                                case MidiEventType::program:{
                                    instruments[channel]=event[1];
                                    break;
                                }
                                case MidiEventType::channel_afterTouch:{
                                    break;
                                }
                                case MidiEventType::pitchWheel:{
                                    break;
                                }
                                case MidiEventType::sysex_begin:{
                                    break;
                                }
                                case MidiEventType::sysex_end:{
                                    break;
                                }
                                case MidiEventType::meta:{
                                    switch(event[1]){
                                        case 0x01:
                                        case 0x02:
                                        case 0x03:
                                        case 0x04:
                                        case 0x05:
                                        case 0x06:
                                        case 0x07:
                                        case 0x08:
                                        case 0x09:{
                                            uint32_t _size=0;
                                            size_t beginIdx=2;
                                            for(;beginIdx<6;++beginIdx) {
                                                _size<<=7;
                                                _size|=(event[beginIdx]&0x7F);
                                                if(event[beginIdx]<0x80) {
                                                    ++beginIdx;
                                                    break;
                                                }
                                            }
                                            
                                            textMap[trackIdx].emplace_back(
                                                time,
                                                MidiTimeMode::tick,
                                                trackIdx,
                                                event[1],
                                                std::string(event.message.begin()+beginIdx,event.message.begin()+beginIdx+_size)
                                            );
                                            break;
                                        }

                                        case 0x51:{
                                            uint32_t mispqn=(event[3]<<16)|(event[4]<<8)|(event[5]);
                                            if(tempoMap[metaTrack].size()){
                                                tempoMap[metaTrack].emplace_back(
                                                    time,
                                                    MidiTimeMode::tick,
                                                    trackIdx,
                                                    mispqn,
                                                    tempoMap[metaTrack][tempoMap[metaTrack].size()-1].timeNode+event.time*tempoMap[metaTrack][tempoMap[metaTrack].size()-1].mispqn/m_head.tpqn()
                                                );
                                            }
                                            else{
                                                tempoMap[metaTrack].emplace_back(
                                                    time,
                                                    MidiTimeMode::tick,
                                                    trackIdx,
                                                    mispqn,
                                                    event.time*0x07A120u/m_head.tpqn()
                                                );
                                            }
                                            break;
                                        }
                                        case 0x58:{
                                            double newBeat,newBar;
                                            if(bbMap[metaTrack].size()){
                                                double deltaBeat;
                                                deltaBeat=(time-bbMap[metaTrack].back().time)
                                                        *bbMap[metaTrack].back().denominator
                                                        /4.0/m_head.tpqn();
                                                newBeat=bbMap[metaTrack].back().beatNode+deltaBeat;
                                                newBar=bbMap[metaTrack].back().barNode+deltaBeat/bbMap[metaTrack].back().numerator;
                                            }
                                            else{
                                                newBeat=time
                                                        *1.0/m_head.tpqn();
                                                newBar=newBeat/4;
                                            }
                                            bbMap[metaTrack].emplace_back(
                                                time,
                                                MidiTimeMode::tick,
                                                trackIdx,
                                                newBar,
                                                newBeat,
                                                event[3],
                                                1<<event[4]
                                            );
                                            break;
                                        }
                                        default:{
                                            break;
                                        }
                                    }
                                    break;
                                }
                                default:{
                                    break;
                                }
                            }
                        }
                    }
                    return MidiErrorType::noError;
                }
                MidiErrorType parse_micro(const MidiFile&m_midi){
                    MidiErrorType err=parse_tick(m_midi);
                    if(err!=MidiErrorType::noError){
                        return err;
                    }
                    err=change_timeMode(MidiTimeMode::microsecond);
                    return err;
                }

                template<typename _MidiEvent>
                bool is_time_sorted(const MidiEventMap<_MidiEvent>&_map){
                    for(size_t trackIdx=0;trackIdx<_map.size();++trackIdx){
                        for(size_t eventIdx=1;eventIdx<_map[trackIdx].size();++eventIdx){
                            if(_map[trackIdx][eventIdx].time<_map[trackIdx][eventIdx-1].time){
                                return false;
                            }
                        }
                    }
                    return true;
                }
                
                size_t _find_tempoIdx_between(uint64_t _time,TempoList&_tempoList,size_t _left,size_t _right){
                    if(_left>=_right){
                        return size_t(-1);
                    }
                    size_t mid=(_left+_right)/2;

                    if(_tempoList[mid].time<=_time){
                        if(mid!=_tempoList.size()-1){
                            if(_tempoList[mid+1].time>_time){
                                return mid;
                            }
                            else{
                                return _find_tempoIdx_between(_time,_tempoList,mid+1,_right);
                            }
                        }
                        else{
                            return mid;
                        }
                    }
                    return _find_tempoIdx_between(_time,_tempoList,_left,mid);
                }
                
                template<typename _MidiEvent,typename _Fun>
                MidiErrorType change_timeMode_sorted(MidiEventMap<_MidiEvent>&_map,_Fun&&_fun){
                    for(size_t trackIdx=0;trackIdx<_map.size();++trackIdx){
                        size_t tempoIdx=0;
                        uint8_t metaTrack=get_metaTrack(trackIdx);
                        for(size_t eventIdx=0;eventIdx<_map[trackIdx].size();++eventIdx){
                            _MidiEvent&event=_map[trackIdx][eventIdx];
                            while(tempoMap[metaTrack][tempoIdx].time<event.time){
                                if(tempoIdx<tempoMap[metaTrack].size()-1){
                                    if(tempoMap[metaTrack][tempoIdx+1].time<=event.time){
                                        ++tempoIdx;
                                        continue;
                                    }
                                }
                                break;
                            }
                            if(tempoMap[metaTrack][tempoIdx].time>event.time){
                                return MidiErrorType::change_timeMode;
                            }
                            _fun(event,tempoMap[metaTrack][tempoIdx]);
                        }
                    }
                    return MidiErrorType::noError;
                }
                
                template<typename _MidiEvent,typename _Fun>
                MidiErrorType change_timeMode_unsorted(MidiEventMap<_MidiEvent>&_map,_Fun&&_fun){
                    for(size_t trackIdx=0;trackIdx<_map.size();++trackIdx){
                        uint8_t metaTrack=get_metaTrack(trackIdx);
                        for(size_t eventIdx=0;eventIdx<_map[trackIdx].size();++eventIdx){
                            _MidiEvent&event=_map[trackIdx][eventIdx];
                            size_t tempoIdx=_find_tempoIdx_between(event.time,tempoMap[metaTrack],0,tempoMap[metaTrack].size());
                            if(tempoIdx==size_t(-1)){
                                _fun(event,tempoMap[metaTrack][tempoIdx]);
                            }
                            else{
                                return MidiErrorType::change_timeMode;
                            }
                        }
                    }
                    return MidiErrorType::noError;
                }
                
                template<typename _MidiEvent>
                MidiErrorType change_timeMode_tick2micro_sorted(MidiEventMap<_MidiEvent>&_map){
                    return change_timeMode_sorted(_map,[&](_MidiEvent&event,const Tempo&tempo){
                        event.timeMode=MidiTimeMode::microsecond;
                        event.time=tempo.timeNode
                                +(event.time-tempo.time)
                                *tempo.mispqn/m_head.tpqn();
                    });
                }
                
                template<typename _MidiEvent>
                MidiErrorType change_timeMode_micro2tick_sorted(MidiEventMap<_MidiEvent>&_map){
                    return change_timeMode_sorted(_map,[&](_MidiEvent&event,const Tempo&tempo){
                        event.timeMode=MidiTimeMode::tick;
                        event.time=tempo.timeNode
                                +(event.time-tempo.time)
                                *m_head.tpqn()/tempo.mispqn;
                    });
                }
                
                template<typename _MidiEvent>
                MidiErrorType change_timeMode_tick2micro_unsorted(MidiEventMap<_MidiEvent>&_map){
                    return change_timeMode_unsorted(_map,[&](_MidiEvent&event,const Tempo&tempo){
                        event.timeMode=MidiTimeMode::microsecond;
                        event.time=tempo.timeNode
                                +(event.time-tempo.time)
                                *tempo.mispqn/m_head.tpqn();
                    });
                }
                
                template<typename _MidiEvent>
                MidiErrorType change_timeMode_micro2tick_unsorted(MidiEventMap<_MidiEvent>&_map){
                    return change_timeMode_unsorted(_map,[&](_MidiEvent&event,const Tempo&tempo){
                        event.timeMode=MidiTimeMode::tick;
                        event.time=tempo.timeNode
                                +(event.time-tempo.time)
                                *m_head.tpqn()/tempo.mispqn;
                    });
                }
            public:
                MidiParser(void)=delete;
                MidiParser(const MidiParser&_midiParser):m_head(_midiParser.m_head),m_timeMode(_midiParser.m_timeMode)
                {
                    if(m_head.format==0x00||m_head.format==0x01){
                        tempoMap.resize(1);
                        tempoMap[0]=_midiParser.tempoMap[0];
                        bbMap.resize(1);
                        bbMap[0]=_midiParser.bbMap[0];
                    }
                    else if(m_head.format==0x02){
                        tempoMap.resize(m_head.ntracks);
                        for(size_t i=0;i<m_head.ntracks;++i){
                            tempoMap[i]=_midiParser.tempoMap[i];
                        }
                        bbMap.resize(m_head.ntracks);
                        for(size_t i=0;i<m_head.ntracks;++i){
                            bbMap[i]=_midiParser.bbMap[i];
                        }
                    }
                    if(_midiParser.noteMap.size()){
                        noteMap.resize(m_head.ntracks);
                        for(size_t i=0;i<m_head.ntracks;++i){
                            noteMap[i]=_midiParser.noteMap[i];
                        }
                    }
                    if(_midiParser.textMap.size()){
                        textMap.resize(m_head.ntracks);
                        for(size_t i=0;i<m_head.ntracks;++i){
                            textMap[i]=_midiParser.textMap[i];
                        }
                    }
                    
                }
                MidiParser(MidiParser&&_midiParser):m_head(std::move(_midiParser.m_head)),m_timeMode(_midiParser.m_timeMode),tempoMap(std::move(_midiParser.tempoMap)),bbMap(std::move(_midiParser.bbMap)),noteMap(std::move(_midiParser.noteMap)),textMap(std::move(_midiParser.textMap))
                {}

                MidiParser(std::string _filename,MidiTimeMode _timeMode=MidiTimeMode::tick):
                    m_timeMode(_timeMode){
                    MidiFile m_midi(_filename);
                    m_midi.read();
                    if(m_midi.is_read_success()){
                        m_head=m_midi.head;
                        if(m_head.format==0x00||m_head.format==0x01){
                            tempoMap.resize(1);
                            bbMap.resize(1);
                        }
                        else if(m_head.format==0x02){
                            tempoMap.resize(m_head.ntracks);
                            bbMap.resize(m_head.ntracks);
                        }
                        noteMap.resize(m_head.ntracks);
                        textMap.resize(m_head.ntracks);

                        if(_timeMode==MidiTimeMode::tick){
                            parse_tick(m_midi);
                        }
                        else if(_timeMode==MidiTimeMode::microsecond){
                            parse_micro(m_midi);
                        }
                    }
                }
                ~MidiParser(void){
                }
            public:
                MidiErrorType change_timeMode(MidiTimeMode _mode=MidiTimeMode::microsecond){
                    if(m_timeMode==_mode){
                        return MidiErrorType::noError;
                    }
                    MidiErrorType err=MidiErrorType::noError;
                    if(m_timeMode==MidiTimeMode::tick&&_mode==MidiTimeMode::microsecond){
                        err=change_timeMode_tick2micro_sorted(noteMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                        err=change_timeMode_tick2micro_sorted(bbMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                        err=change_timeMode_tick2micro_sorted(textMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                    }
                    else if(m_timeMode==MidiTimeMode::microsecond&&_mode==MidiTimeMode::tick){
                        err=change_timeMode_micro2tick_sorted(noteMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                        err=change_timeMode_micro2tick_sorted(bbMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                        err=change_timeMode_micro2tick_sorted(textMap);
                        if(err!=MidiErrorType::noError){
                            return err;
                        }
                    }
                    for(uint8_t trackIdx=0;trackIdx<tempoMap.size();++trackIdx){
                        for(size_t eventIdx=0;eventIdx<tempoMap[trackIdx].size();++eventIdx){
                            std::swap(tempoMap[trackIdx][eventIdx].time,tempoMap[trackIdx][eventIdx].timeNode);
                        }
                    }
                    m_timeMode=_mode;
                    return err;
                }
            public:
                uint16_t format(void)const{
                    return m_head.format;
                }
                uint16_t ntracks(void)const {
                    return m_head.ntracks;
                }
                uint16_t division(void)const{
                    return m_head.division;
                }
                uint16_t tpqn(void)const{
                    return m_head.tpqn();
                }
                MidiTimeMode timeMode(void)const{
                    return m_timeMode;
                }
            public:
                MidiParser& operator=(const MidiParser&)=default;
                MidiParser& operator=(MidiParser&&)=default;

        };
    }
}
#endif