//此文件内容为典型的错误示例，请注意避免写成这样的代码
#include"MidiPlayer.hpp"
using namespace GoldType::MidiParse;
MidiPlayer player;
int main(){
    player=MidiPlayer("../../../midi/nggyu.mid");
    player.start_normal();

    //请在下方添加以下两种方法中的一种

    //1.等待线程结束
    // player.join();

    //2.永不结束的循环，让线程在循环中退出
    // while(true){
    //     Sleep(1000);
    // }

    //若不添加，则会导致main函数立即结束
    //main函数的提前结束会导致两个问题：
    //1.MidiPlayer创建的线程仍在运行，抛出异常“terminate called without an active exception”
    //2.WINAPI依赖于main函数，若main函数提前结束，midiOutOpen函数会卡住

    return 0;
}