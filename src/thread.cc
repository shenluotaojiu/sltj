#include "thread.h"
#include "log.h"

namespace sltj
{
    static thread_local Thread *t_thread = nullptr;    // 当前线程
    static thread_local std::string t_name = "UNKNOW"; // 线程名字

    static sltj::Logger::ptr g_logger = SLTJ_LOG_NAME("system");

    Thread *Thread::GetThis()
    {
        return t_thread;
    }
    const std::string Thread::GetName()
    {
        return t_name;
    }
    void Thread::SetName(const std::string &name)
    {
        if (t_thread)
        {
            t_thread->m_name = name;
        }
        t_name = name;
    }

    Thread::~Thread()
    {
        if (m_thread)
        {
            pthread_detach(m_thread);
        }
    }

    void *Thread::run(void *arg)
    {
        Thread *thread = (Thread *)arg;
        t_thread = thread;
        thread->m_id = sltj::GetThreadId();
        pthread_setname_np(pthread_self(), thread->m_name.substr(0, 15).c_str());
        std::function<void()> cb;
        cb.swap(thread->m_cb);
        thread->m_semphore.notify();
        cb();
        return 0;
    }

    void Thread::join()
    {
        if (m_thread)
        {
            int rt = pthread_join(m_thread, nullptr);
            if (rt)
            {
                SLTJ_LOG_ERROR(g_logger) << "pthread_join thread fail, rt=" << rt << "name = " << m_name;
                throw std::logic_error("pthread_join error");
            }
            m_thread = 0;
        }
    }
    void Thread::detach()
    {
        if (m_thread)
        {
            int rt = pthread_detach(m_thread);
            if (rt)
            {
                SLTJ_LOG_ERROR(g_logger) << "pthread_detach thread fail, rt=" << rt << "name = " << m_name;
                throw std::logic_error("pthread_detach error");
            }
            m_thread = 0;
        }
    }

    Thread::Thread(std::function<void()> cb, const std::string &name)
    : m_cb(cb) , m_name(name)
    {
     if (name.empty())
            m_name = "UNKWNO";
        int rt = pthread_create(&m_thread, nullptr, &Thread::run, this);
        if (rt)
        {
            SLTJ_LOG_ERROR(g_logger) << "pthread_create thread fail, rt=" << rt << "name = " << name;
            throw std::logic_error("pthread_create error");
        }
        m_semphore.wait();
    }

    Semaphore::Semaphore(uint32_t count)
    {
        if (sem_init(&m_semaphore, 0, count))
        {
            throw std::logic_error("sem_init error");
        }
    }
    Semaphore::~Semaphore()
    {
        sem_destroy(&m_semaphore);
    }

    void Semaphore::wait()
    {
        if (sem_wait(&m_semaphore))
        {
            throw std::logic_error("sem_wait error");
        }
    }
    void Semaphore::notify()
    {
        if (sem_post(&m_semaphore))
        {
            throw std::logic_error("sem_post error");
        }
    }

}