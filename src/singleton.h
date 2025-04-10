#ifndef __SLTJ_SINGLETON_H__
#define __SLTJ_SINGLETON_H__

namespace sltj{

// 基于static的单例
template<class T,class X = void, int N = 0>
class Singleton {
public:
    static T* GetInstance() {
        static T v;
        return &v;
    }
};

// 基于智能指针的单例
template<class T,class X = void, int N = 0>
class SingletonPtr{
public:
    static std::shared_ptr<T> GetInstance() {
        static std::shared_ptr<T> v(new T());
        return v;
    }
};


}

#endif //__SLTJ_SINGLETON_H__