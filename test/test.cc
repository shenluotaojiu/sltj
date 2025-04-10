#include <iostream>
#include "../src/log.h"
#include "../src/util.h"

int main(int argc, char **argv)
{
    // sltj::Logger::ptr logger(new sltj::Logger());
    // logger->addAppender(sltj::LogAppender::ptr(new sltj::StdoutLogAppender()));

    // sltj::Logger::ptr logger1(new sltj::Logger("王鸿炜"));
    // logger1->addAppender(sltj::LogAppender::ptr(new sltj::StdoutLogAppender()));

    // SLTJ_LOG_DEBUG(logger) << "debug";
    // SLTJ_LOG_ERROR(logger) << "error";
    // SLTJ_LOG_INFO(logger) << "info";
    // SLTJ_LOG_FATAL(logger) << "fatal";
    // SLTJ_LOG_WARN(logger) << "warning";

    // SLTJ_LOG_DEBUG(logger1) << "debug";
    // SLTJ_LOG_ERROR(logger1) << "error";
    // SLTJ_LOG_INFO(logger1) << "info";
    // SLTJ_LOG_FATAL(logger1) << "fatal";
    // SLTJ_LOG_WARN(logger1) << "warning";

    // SLTJ_LOG_FMT_DEBUG(logger, "test macro fmt error %d hhh", 16);

    auto l = sltj::LoggerMgr::GetInstance()->getLogger("");
    SLTJ_LOG_DEBUG(l)<<"test 111";

    std::cout << "hello log" << std::endl;

    return 0;
}