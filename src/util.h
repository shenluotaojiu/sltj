#ifndef __SLTJ_UTIL_H__
#define __SLTJ_UTIL_H__

#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>

namespace sltj
{
    pid_t GetThreadId();
    uint32_t GetFiberId();


} // namespace sltj
#endif