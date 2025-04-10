#include "log.h"

#include <map>
#include <iostream>
#include <functional>
#include <time.h>
#include <string.h>
#include <stdarg.h>

// %m -- 消息体
// %p -- level
// %r -- 启动后的时间
// %n -- 回车换行
// %d -- 时间
// %f -- 文件名
// %l -- 行号
// %t -- 线程Id
// %T -- Tab
// %F -- 协程Id
// %N -- 日志器名字

namespace sltj
{
    LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::Level level, const char *file, int32_t line, uint32_t elapse, uint32_t threadId, uint32_t fiberId, uint32_t time)
        : m_file(file),
          m_line(line),
          m_elapse(elapse),
          m_threadId(threadId),
          m_fiberId(fiberId),
          m_time(time),
          m_logger(logger),
          m_level(level)
    {
        m_name = logger->getName();
    }

    Logger::Logger(const std::string &name)
        : m_name(name), m_level(LogLevel::DEBUG)
    {
        // 默认格式：时间 等级 文件 行 内容 换行符
        m_formatter.reset(new LogFormatter("%d{%Y-%m-%d %H:%M:%S} %T%t %T%F%T [%p] %T [%N] %T %f %l %T %m %n"));
    }

    void Logger::log(LogLevel::Level level, LogEvent::ptr &event)
    {
        // 事件等级够，有appender输出日志
        if (level >= m_level)
        {
            auto self = shared_from_this();
            MutexType::Lock lock(m_mutex);
            if (!m_appenders.empty())
            {
                for (auto &i : m_appenders)
                {
                    i->log(level, event);
                }
            }
        }
    }

    LogEventWrap::~LogEventWrap()
    {
        // 避免log的显示调用
        m_event->getLogger()->log(m_event->getLevel(), m_event);
    }

    LogEvent::~LogEvent()
    {
    }

    void LogEvent::format(const char *fmt, ...)
    {
        va_list al;
        va_start(al, fmt);
        format(fmt, al);
        va_end(al);
    }
    void LogEvent::format(const char *fmt, va_list al)
    {
        char *buf = nullptr;
        int len = vasprintf(&buf, fmt, al);
        if (len != -1)
        {
            m_ss << std::string(buf, len);
            free(buf);
        }
    }

    const char *LogLevel::ToString(LogLevel::Level level)
    {
        switch (level)
        {
#define H(name)          \
    case LogLevel::name: \
        return #name;    \
        break;

            H(DEBUG)
            H(ERROR)
            H(FATAL)
            H(INFO)
            H(WARN)
#undef H
        // case LogLevel::DEBUG:
        //     return "DEBUG";
        //     break;
        // case LogLevel::ERROR:
        //     return "ERROR";
        //     break;
        // case LogLevel::FATAL:
        //     return "FATAL";
        //     break;
        // case LogLevel::INFO:
        //     return "INFO";
        //     break;
        // case LogLevel::WARN:
        //     return "WARN";
        //     break;
        default:
            return "UNKNOW";
            break;
        }
    }

    LogLevel::Level LogLevel::FromString(const std::string &str)
    {
#define XX(level, v)            \
    if (str == #v)              \
    {                           \
        return LogLevel::level; \
    }

        XX(DEBUG, debug);
        XX(INFO, info);
        XX(WARN, warn);
        XX(ERROR, error);
        XX(FATAL, fatal);

        XX(DEBUG, DEBUG);
        XX(INFO, INFO);
        XX(WARN, WARN);
        XX(ERROR, ERROR);
        XX(FATAL, FATAL);
        return LogLevel::UNKNOW;
#undef XX
    }

    void Logger::debug(LogEvent::ptr event)
    {
        log(LogLevel::DEBUG, event);
    }
    void Logger::info(LogEvent::ptr event)
    {
        log(LogLevel::INFO, event);
    }
    void Logger::warn(LogEvent::ptr event)
    {
        log(LogLevel::WARN, event);
    }
    void Logger::fatal(LogEvent::ptr event)
    {
        log(LogLevel::FATAL, event);
    }
    void Logger::error(LogEvent::ptr event)
    {
        log(LogLevel::ERROR, event);
    }
    void Logger::addAppender(LogAppender::ptr appender)
    {
        MutexType::Lock lock(m_mutex);
        if (appender->getFormatter() == nullptr)
        {
            MutexType::Lock ll(appender->m_mutex);
            appender->setFormatter(m_formatter);
        }
        m_appenders.push_back(appender);
    }
    void Logger::delAppender(LogAppender::ptr appender)
    {
        MutexType::Lock lock(m_mutex);
        for (auto ite = m_appenders.begin();
             ite != m_appenders.end(); ite++)
        {
            if (*ite == appender)
            {
                m_appenders.erase(ite);
                break;
            }
        }
    }

    void StdoutLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            MutexType::Lock lock(m_mutex);
            std::cout << m_formatter->format(level, event);
        }
    }
    void FileLogAppender::log(LogLevel::Level level, LogEvent::ptr event)
    {
        if (level >= m_level)
        {
            MutexType::Lock lock(m_mutex);
            m_filestream << m_formatter->format(level, event);
        }
    }

    bool FileLogAppender::reopen()
    {
        MutexType::Lock lock(m_mutex);
        if (m_filestream)
            m_filestream.close();
        m_filestream.open(m_filename);
        return !!m_filestream; // !!表示非0为1,0仍然为0
    }

    std::string LogFormatter::format(LogLevel::Level level, LogEvent::ptr event)
    {
        std::stringstream ss;
        for (auto &ite : m_items)
        {
            ite->format(ss, level, event);
        }
        return ss.str();
    }

    class MessageFormatItem : public LogFormatter::FormatItem
    {
    public:
        MessageFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getContent();
        }
    };

    class LevelFormatItem : public LogFormatter::FormatItem
    {
    public:
        LevelFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatItem
    {
    public:
        ElapseFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getElapse();
        }
    };

    class TabFormatItem : public LogFormatter::FormatItem
    {
    public:
        TabFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << "\t";
        }
    };

    class FileFormatItem : public LogFormatter::FormatItem
    {
    public:
        FileFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatItem
    {
    public:
        LineFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getLine();
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        ThreadIdFormatItem(const std::string &str = "") {}
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getThreadId();
        }
    };

    class TimeFormatItem : public LogFormatter::FormatItem
    {
    public:
        TimeFormatItem(const std::string &format = "%Y-%m-%d %H:%M:%S")
            : m_format(format)
        {
            if (m_format.empty())
                m_format = "%Y-%m-%d %H:%M:%S";
        }
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            struct tm tm;
            time_t time = event->getTime();
            localtime_r(&time, &tm);
            char buf[64];
            strftime(buf, sizeof(buf), m_format.c_str(), &tm);

            os << buf;
        }

    private:
        std::string m_format;
    };

    class NewLineFormatItem : public LogFormatter::FormatItem
    {
    public:
        NewLineFormatItem(const std::string &format = "")
        {
        }
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << std::endl;
        }
    };

    class NameFormatItem : public LogFormatter::FormatItem
    {
    public:
        NameFormatItem(const std::string &format = "")
        {
        }
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getName();
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatItem
    {
    public:
        FiberIdFormatItem(const std::string &format = "")
        {
        }
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << event->getFiberId();
        }
    };

    class StringFormatItem : public LogFormatter::FormatItem
    {
    public:
        StringFormatItem(const std::string &str)
            : m_string(str)
        {
        }
        void format(std::ostream &os, LogLevel::Level level, LogEvent::ptr event)
        {
            os << m_string;
        }

    private:
        std::string m_string;
    };

    // 分析所给格式形式
    void LogFormatter::init()
    {
        // str, format, type
        std::vector<std::tuple<std::string, std::string, int>> vec;
        std::string nstr;
        for (size_t i = 0; i < m_pattern.size(); ++i)
        {
            if (m_pattern[i] != '%')
            {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            if ((i + 1) < m_pattern.size())
            {
                if (m_pattern[i + 1] == '%')
                {
                    nstr.append(1, '%');
                    continue;
                }
            }

            size_t n = i + 1;
            int fmt_status = 0;
            size_t fmt_begin = 0;

            std::string str;
            std::string fmt;
            while (n < m_pattern.size())
            {
                if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{' && m_pattern[n] != '}'))
                {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if (fmt_status == 0)
                {
                    if (m_pattern[n] == '{')
                    {
                        str = m_pattern.substr(i + 1, n - i - 1);
                        // std::cout << "*" << str << std::endl;
                        fmt_status = 1; // 解析格式
                        fmt_begin = n;
                        ++n;
                        continue;
                    }
                }
                else if (fmt_status == 1)
                {
                    if (m_pattern[n] == '}')
                    {
                        fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                        // std::cout << "#" << fmt << std::endl;
                        fmt_status = 0;
                        ++n;
                        break;
                    }
                }
                ++n;
                if (n == m_pattern.size())
                {
                    if (str.empty())
                    {
                        str = m_pattern.substr(i + 1);
                    }
                }
            }

            if (fmt_status == 0)
            {
                if (!nstr.empty())
                {
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            }
            else if (fmt_status == 1) // 无法解析的格式
            {
                std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                // m_error = true;
                vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
            }
        }

        if (!nstr.empty())
        {
            vec.push_back(std::make_tuple(nstr, "", 0));
        }
        static std::map<std::string, std::function<FormatItem::ptr(const std::string &str)>> s_format_items = {
#define XX(str, C)                                                               \
    {                                                                            \
        #str, [](const std::string &fmt) { return FormatItem::ptr(new C(fmt)); } \
    }

            XX(m, MessageFormatItem),  // m:消息
            XX(p, LevelFormatItem),    // p:日志级别
            XX(r, ElapseFormatItem),   // r:累计毫秒数
            XX(t, ThreadIdFormatItem), // t:线程id
            XX(n, NewLineFormatItem),  // n:换行
            XX(d, TimeFormatItem),     // d:时间
            XX(f, FileFormatItem),     // f:文件名
            XX(l, LineFormatItem),     // l:行号
            XX(T, TabFormatItem),      // T:Tab
            XX(F, FiberIdFormatItem),  // F:协程id
            XX(N, NameFormatItem)      // N:日志器名字

#undef XX
        };

        for (auto &i : vec)
        {
            if (std::get<2>(i) == 0)
            {
                m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
            }
            else
            {
                auto it = s_format_items.find(std::get<0>(i));
                if (it == s_format_items.end())
                {
                    m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                    // m_error = true;
                }
                else
                {
                    m_items.push_back(it->second(std::get<1>(i)));
                }
            }

            // std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
        }
        // std::cout << m_items.size() << std::endl;
    }

    LogManager::LogManager()
    {
        m_root.reset(new Logger());
        m_root->addAppender(LogAppender::ptr(new StdoutLogAppender()));
    }
    Logger::ptr LogManager::getLogger(const std::string &str)
    {
        auto ite = m_loggers.find(str);
        return ite == m_loggers.end() ? m_root : ite->second;
    }
    void LogManager::init()
    {
    }

    Logger::ptr LogManager::getRoot() const
    {
        return m_root;
    }

    LogFormatter::ptr LogAppender::getFormatter()
    {
        MutexType::Lock lock(m_mutex);
        return m_formatter;
    }

    void Logger::setFormatter(LogFormatter::ptr formatter)
    {
        MutexType::Lock lock(m_mutex);
        m_formatter = formatter;
    }
    LogFormatter::ptr Logger::getFormatter() const
    {
        return m_formatter;
    }

}