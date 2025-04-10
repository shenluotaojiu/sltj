#ifndef __SLTJ_LOG_H__
#define __SLTJ_LOG_H__

#include <string>
#include <stdint.h>
#include <memory>
#include <list>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <stdarg.h>
#include <map>

#include "singleton.h"
#include "util.h"
#include "thread.h"

// 流式=======================================
#define SLTJ_LOG_LEVEL(logger, level)                                                                             \
    if (logger->getLevel() <= level)                                                                              \
    sltj::LogEventWrap(sltj::LogEvent::ptr(new sltj::LogEvent(logger, level, __FILE__, __LINE__, 0,               \
                                                              sltj::GetThreadId(), sltj::GetFiberId(), time(0)))) \
        .getSS()

#define SLTJ_LOG_DEBUG(logger) SLTJ_LOG_LEVEL(logger, sltj::LogLevel::DEBUG)
#define SLTJ_LOG_INFO(logger) SLTJ_LOG_LEVEL(logger, sltj::LogLevel::INFO)
#define SLTJ_LOG_WARN(logger) SLTJ_LOG_LEVEL(logger, sltj::LogLevel::WARN)
#define SLTJ_LOG_ERROR(logger) SLTJ_LOG_LEVEL(logger, sltj::LogLevel::ERROR)
#define SLTJ_LOG_FATAL(logger) SLTJ_LOG_LEVEL(logger, sltj::LogLevel::FATAL)
// ==================================================================

// 格式化===============================================
#define SLTJ_LOG_FMT_LEVEL(logger, level, fmt, ...)                                                               \
    if (logger->getLevel() <= level)                                                                              \
    sltj::LogEventWrap(sltj::LogEvent::ptr(new sltj::LogEvent(logger, level, __FILE__, __LINE__, 0,               \
                                                              sltj::GetThreadId(), sltj::GetFiberId(), time(0)))) \
        .getEvent()                                                                                               \
        ->format(fmt, __VA_ARGS__)

#define SLTJ_LOG_FMT_DEBUG(logger, fmt, ...) SLTJ_LOG_FMT_LEVEL(logger, sltj::LogLevel::DEBUG, fmt, __VA_ARGS__)
#define SLTJ_LOG_FMT_INFO(logger, fmt, ...) SLTJ_LOG_FMT_LEVEL(logger, sltj::LogLevel::INFO, fmt, __VA_ARGS__)
#define SLTJ_LOG_FMT_WARN(logger, fmt, ...) SLTJ_LOG_FMT_LEVEL(logger, sltj::LogLevel::WARN, fmt, __VA_ARGS__)
#define SLTJ_LOG_FMT_ERROR(logger, fmt, ...) SLTJ_LOG_FMT_LEVEL(logger, sltj::LogLevel::ERROR, fmt, __VA_ARGS__)
#define SLTJ_LOG_FMT_FATAL(logger, fmt, ...) SLTJ_LOG_FMT_LEVEL(logger, sltj::LogLevel::FATAL, fmt, __VA_ARGS__)
// ==========================================================

// 获得主日志器
#define SLTJ_LOG_ROOT() sltj::LoggerMgr::GetInstance()->getRoot()
// 获得名为name的日志器
#define SLTJ_LOG_NAME(name) sltj::LoggerMgr::GetInstance()->getLogger(name)

namespace sltj
{
    class Logger;

    // 日志级别
    class LogLevel
    {
    public:
        enum Level
        {
            UNKNOW = 0,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL
        };

        static const char *ToString(LogLevel::Level level);
        static LogLevel::Level FromString(const std::string &string);
    };

    // 日志事件
    class LogEvent
    {
    public:
        using ptr = std::shared_ptr<LogEvent>;
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file,
                 int32_t line, uint32_t elapse, uint32_t threadId,
                 uint32_t fiberId, uint32_t time);
        ~LogEvent();

        const char *getFile() const { return m_file; }
        int32_t getLine() const { return m_line; }
        uint32_t getElapse() const { return m_elapse; }
        uint32_t getThreadId() const { return m_threadId; }
        uint32_t getFiberId() const { return m_fiberId; }
        uint32_t getTime() const { return m_time; }
        std::string getName() const { return m_name; }
        std::string getContent() const { return m_ss.str(); }
        std::stringstream &&getSS() { return std::move(m_ss); }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }
        LogLevel::Level getLevel() const { return m_level; }

        void format(const char *fmt, ...);
        void format(const char *fmt, va_list al);

    private:
        const char *m_file = nullptr; // 文件名
        int32_t m_line = 0;           // 行号
        uint32_t m_elapse = 0;        // 程序启动后到现在的毫秒数
        uint32_t m_threadId = 0;      // 线程ID
        uint32_t m_fiberId = 0;       // 协程ID
        uint32_t m_time;              // 时间戳
        std::stringstream m_ss;       // 内容
        std::string m_name;           // 日志器名称
        std::shared_ptr<Logger> m_logger;
        LogLevel::Level m_level;
    };

    class LogEventWrap
    {
    public:
        LogEventWrap(LogEvent::ptr event)
            : m_event(event)
        {
        }
        ~LogEventWrap();
        std::stringstream &&getSS() { return std::move(m_event->getSS()); }
        LogEvent::ptr getEvent() const { return m_event; }

    private:
        LogEvent::ptr m_event;
    };

    // 日志格式器
    class LogFormatter
    {
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        LogFormatter(const std::string &pattern) : m_pattern(pattern)
        {
            init();
        }
        ~LogFormatter() = default;
        std::string format(LogLevel::Level level, LogEvent::ptr event);
        void init();

    public:
        // 格式类型
        class FormatItem
        {
        public:
            using ptr = std::shared_ptr<FormatItem>;
            virtual ~FormatItem() {}
            virtual void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event) = 0;
        };

    private:
        std::string m_pattern; // 格式字符串
        std::vector<FormatItem::ptr> m_items;
    };

    // 日志输出器
    class LogAppender
    {
    friend class Logger;
    public:
        using ptr = std::shared_ptr<LogAppender>;
        using MutexType = NullMutex;

        LogAppender() = default;
        virtual ~LogAppender() {}
        virtual void log(LogLevel::Level level, LogEvent::ptr event) = 0;
        void setFormatter(LogFormatter::ptr formatter) { m_formatter = formatter; }
        LogFormatter::ptr getFormatter();
        void setLevel(LogLevel::Level level) { m_level = level; }

    protected:
        LogLevel::Level m_level;
        LogFormatter::ptr m_formatter;
        MutexType m_mutex;
    };

    // 日志器
    class Logger : public std::enable_shared_from_this<Logger>
    {
    public:
        using ptr = std::shared_ptr<Logger>;
        using MutexType = NullMutex;

        Logger(const std::string &name = "root");
        ~Logger() = default;

        void log(LogLevel::Level level, LogEvent::ptr &event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);
        void error(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);
        LogLevel::Level getLogLevel() const { return m_level; }
        std::string getName() const { return m_name; }
        void setLevel(LogLevel::Level level) { m_level = level; }
        LogLevel::Level getLevel() const { return m_level; }
        void setFormatter(LogFormatter::ptr formatter);
        LogFormatter::ptr getFormatter() const;

    private:
        std::string m_name;                      // 日志器名称
        LogLevel::Level m_level;                 // 日志等级
        std::list<LogAppender::ptr> m_appenders; // appender父类指针集合
        LogFormatter::ptr m_formatter;           
        MutexType m_mutex;
    };

    // 输出到控制台
    class StdoutLogAppender : public LogAppender
    {
    public:
        using ptr = std::shared_ptr<StdoutLogAppender>;
        virtual ~StdoutLogAppender() {}
        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;

    private:
    };

    // 输出到文件
    class FileLogAppender : public LogAppender
    {
    public:
        using ptr = std::shared_ptr<FileLogAppender>;
        FileLogAppender(const std::string filename) : m_filename(filename) {}
        virtual ~FileLogAppender() {}
        virtual void log(LogLevel::Level level, LogEvent::ptr event) override;
        bool reopen(); // 重新打开文件，成功返回true

    private:
        std::string m_filename;     // 文件名
        std::ofstream m_filestream; // 文件输出流
    };

    class LogManager
    {
    public:
        using MutexType = NullMutex;
        LogManager();
        Logger::ptr getLogger(const std::string& str);
        Logger::ptr getRoot() const;

        void init();
    private:
        std::map<std::string, Logger::ptr> m_loggers;
        Logger::ptr m_root;
        MutexType m_mutex;
    };

    using LoggerMgr = sltj::SingletonPtr<LogManager>;    

}

#endif
