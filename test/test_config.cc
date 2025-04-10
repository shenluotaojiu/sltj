#include "../src/log.h"
#include "../src/config.h"

sltj::ConfigVar<int>::ptr g_int_config = sltj::Config::Lookup("system.port",(int)8080,"system.port");

sltj::ConfigVar<float>::ptr g_float_config = sltj::Config::Lookup("system.value",(float)3.14,"system.value");

int main(int argc,char** argv)
{
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << g_int_config->getValue();
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << g_int_config->toString();

    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << g_float_config->getValue();
    SLTJ_LOG_INFO(SLTJ_LOG_ROOT()) << g_float_config->toString();

    return 0;
}