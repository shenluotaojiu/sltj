#include "../src/sltj.h"
#include <chrono>

// 为啥没明显差别呢？
// mutex : 5线程-10000次 5w --- 4396324575
// nullmutex: 5线程-10000次 5w条实际49997条 --- 4467462864 4454990725

int count = 0;
sltj::RWMutex s_rwmutex;
sltj::Mutex s_mutex;

void func1(){
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "thread_name = " << sltj::Thread::GetName()
                                    << " this_name = " << sltj::Thread::GetThis()->getName()
                                    << " thread_id = " << sltj::Thread::GetThis()->getId() ;
    auto start = std::chrono::high_resolution_clock::now(); 
    // for(int i=0;i<10000;i++){
    //     sltj::Mutex::Lock mm(s_mutex);
         
    // }
    int i = 1000;
    while(i--)
    {
        ++count;
        SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "thread_name = " << sltj::Thread::GetName()
                                    << " this_name = " << sltj::Thread::GetThis()->getName()
                                    << " thread_id = " << sltj::Thread::GetThis()->getId() ;
    }
    auto end = std::chrono::high_resolution_clock::now(); 
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count(); 
 }

void func2(){

}

int main(int argc,char** argv){
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "test start";
    std::vector<sltj::Thread::ptr> vec;
    for(int i=0;i<50;i++){
        vec.emplace_back(new sltj::Thread(func1,"name_" + std::to_string(i)));
    }

    for(u_int i=0;i<vec.size();i++){
        vec[i]->join();
    }

    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << count;

    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << "test end";
    return 0;
}