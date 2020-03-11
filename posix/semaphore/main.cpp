#include <iostream>
#include <fstream>
#include <array>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>

constexpr const char* const SEM_PATH  = "/test.sem";

#define CHECK_ERROR(CODE, file, line)                                                    \
if (CODE == -1)                                                                          \
{                                                                                        \
    std::cerr << '[' << (file) << ':' << (line) << "] " << strerror(errno) << std::endl; \
    return -1;                                                                           \
}

#define CHECK(CODE) CHECK_ERROR(CODE, __FILE__, __LINE__)

int main()
{
    mode_t perms = 0666;
    int flags = O_RDWR | O_CREAT;
    auto sem = sem_open(SEM_PATH, O_RDWR | O_CREAT, perms, 66);
    if (sem == SEM_FAILED)
    {
        std::cerr << '[' << __FILE__ << ':' << __LINE__ << "] " << strerror(errno) << std::endl;
        return -1;
    }
    CHECK(sem_close(sem))
    return 0;
}