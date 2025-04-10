#ifndef __SLTJ_THREAD_H__
#define __SLTJ_THREAD_H__

// std::thread与pthread_t结合
#include <thread>
#include <functional>
#include <memory>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <atomic>

namespace sltj
{
    // 作用域锁模板
    template <class T>
    struct ScopedLockImpl
    {
    public:
        ScopedLockImpl(T &mutex)
            : m_mutex{mutex}
        {
            m_mutex.lock();
            m_locked = true;
        }
        ~ScopedLockImpl()
        {
            m_mutex.unlock();
            m_locked = false;
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.lock();
                m_locked = true;
            }
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }
        bool state() const // 当前锁的状态
        {
            return m_locked;
        }

    private:
        T &m_mutex;
        bool m_locked;
    };

    // 信号量
    class Semaphore
    {
    public:
        using ptr = std::shared_ptr<Semaphore>;

    public:
        Semaphore(uint32_t count = 0);
        ~Semaphore();

        void wait();
        void notify();

    private:
        Semaphore(const Semaphore &) = delete;
        Semaphore(const Semaphore &&) = delete;
        Semaphore &operator=(const Semaphore &) = delete;

    private:
        sem_t m_semaphore;
    };

    // 读锁模板
    template <class T>
    struct ReadScopedLockImpl
    {
    public:
        ReadScopedLockImpl(T &mutex)
            : m_mutex{mutex}
        {
            m_mutex.rdlock();
            m_locked = true;
        }
        ~ReadScopedLockImpl()
        {
            m_mutex.unlock();
            m_locked = false;
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.rdlock();
                m_locked = true;
            }
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }
        bool state() const // 当前锁的状态
        {
            return m_locked;
        }

    private:
        T &m_mutex;
        bool m_locked;
    };

    template <class T>
    struct WriteScopedLockImpl
    {
    public:
        WriteScopedLockImpl(T &mutex)
            : m_mutex{mutex}
        {
            m_mutex.wrlock();
            m_locked = true;
        }
        ~WriteScopedLockImpl()
        {
            m_mutex.unlock();
            m_locked = false;
        }
        void lock()
        {
            if (!m_locked)
            {
                m_mutex.wrlock();
                m_locked = true;
            }
        }
        void unlock()
        {
            if (m_locked)
            {
                m_mutex.unlock();
                m_locked = false;
            }
        }
        bool state() const // 当前锁的状态
        {
            return m_locked;
        }

    private:
        T &m_mutex;
        bool m_locked;
    };

    // 读写锁
    class RWMutex
    {
    public:
        using WriteMutex = WriteScopedLockImpl<RWMutex>;
        using ReadMutex = ReadScopedLockImpl<RWMutex>;

    public:
        RWMutex()
        {
            pthread_rwlock_init(&m_lock, nullptr);
        }
        ~RWMutex()
        {
            pthread_rwlock_destroy(&m_lock);
        }
        void rdlock()
        {
            pthread_rwlock_rdlock(&m_lock);
        }
        void wrlock()
        {
            pthread_rwlock_wrlock(&m_lock);
        }
        void unlock()
        {
            pthread_rwlock_unlock(&m_lock);
        }

    private:
        pthread_rwlock_t m_lock;
    };

    // 互斥量
    class Mutex
    {
    public:
        using Lock = ScopedLockImpl<Mutex>;

    public:
        Mutex()
        {
            pthread_mutex_init(&m_mutex, nullptr);
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&m_mutex);
        }
        void lock()
        {
            pthread_mutex_lock(&m_mutex);
        }
        void unlock()
        {
            pthread_mutex_unlock(&m_mutex);
        }

    private:
        pthread_mutex_t m_mutex;
    };

    // 自旋锁
    class SpinLock
    {
    public:
        using Lock = ScopedLockImpl<SpinLock>;

    public:
        SpinLock()
        {
            pthread_spin_init(&m_lock, 0);
        }
        ~SpinLock()
        {
            pthread_spin_destroy(&m_lock);
        }
        void lock()
        {
            pthread_spin_lock(&m_lock);
        }
        void unlock()
        {
            pthread_spin_unlock(&m_lock);
        }

    private:
        pthread_spinlock_t m_lock;
    };

    // 空互斥量
    class NullMutex
    {
    public:
        using Lock = ScopedLockImpl<NullMutex>;

    public:
        NullMutex() {}
        ~NullMutex() {}
        void lock() {}
        void unlock() {}
    };

    // CAS乐观锁
    class CASLock
    {
    public:
        CASLock()
        {
            m_mutex.clear();
        }
        ~CASLock()
        {

        }
        void lock()
        {
            while(std::atomic_flag_test_and_set_explicit(&m_mutex,std::memory_order_acquire));
        }
        void unlock()
        {
            std::atomic_flag_clear_explicit(&m_mutex,std::memory_order_release);
        }
    private:
        volatile std::atomic_flag m_mutex;
    };

    // 线程类
    class Thread
    {
    public:
        using ptr = std::shared_ptr<Thread>;

    public:
        Thread(std::function<void()> cb, const std::string &name);
        ~Thread();

        std::string getName() const { return m_name; }
        pid_t getId() const { return m_id; }

        static Thread *GetThis();
        static const std::string GetName();
        void SetName(const std::string &name);
        static void *run(void *arg);

        void join();
        void detach();

    private:
        Thread(const Thread &) = delete;
        Thread(const Thread &&) = delete;
        Thread &operator=(const Thread &) = delete;

    private:
        pid_t m_id = -1;
        pthread_t m_thread = 0;
        std::function<void()> m_cb;
        std::string m_name;
        Semaphore m_semphore;
    };
}

#endif