#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

inline bool IsExist(const std::string& name) noexcept
{
    struct stat buffer;
    auto res = stat (name.c_str(), &buffer);
    if (res == -1)
    {
        std::cerr << '[' << __FILE__ << ':' << __LINE__ << "] " << strerror(errno) << std::endl;
        return false;
    }
    return res == 0; 
}

constexpr const char* const SEM_PATH  = "/tmp/sem.temp";
constexpr int PROJ_ID                 = 1;
constexpr int SEM_COUNT               = 16;

#define CHECK_ERROR(CODE, file, line)                                                    \
if (CODE == -1)                                                                          \
{                                                                                        \
    std::cerr << '[' << (file) << ':' << (line) << "] " << strerror(errno) << std::endl; \
    return -1;                                                                           \
}

#define CHECK(CODE) CHECK_ERROR(CODE, __FILE__, __LINE__)

int main()
{
    if (!IsExist(SEM_PATH))
    {
        std::ofstream o(SEM_PATH);
    }
    const auto key = ftok(SEM_PATH, PROJ_ID);
    CHECK(key)
    constexpr int semFlags = IPC_CREAT | 0666;
    auto semId = semget(key, SEM_COUNT, semFlags);

    for(int i = 0; i < SEM_COUNT; ++i)
    {
        CHECK(semctl(semId, i, SETVAL, i))
    }

    CHECK(semId)
    CHECK(semctl(semId, SEM_COUNT, IPC_RMID, nullptr))
    return 0;
}