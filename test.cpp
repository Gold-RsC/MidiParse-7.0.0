#include<iostream>
#include<thread>
using namespace std;
int main(){
    std::thread t1([](){
        std::cout<<"Thread 1 is running"<<std::endl;
    });
    t1.join();
    cout<<"finish"<<endl;
    return 0;
}
