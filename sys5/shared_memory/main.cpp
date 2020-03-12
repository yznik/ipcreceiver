#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
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

constexpr const char* const SHM_MEM_PATH  = "/tmp/mem.temp";
constexpr int PROJ_ID                     = 1;
constexpr int FILE_SIZE                   = 1024 * 1024;

#define CHECK_ERROR(CODE, file, line)                                                    \
if (CODE == -1)                                                                          \
{                                                                                        \
    std::cerr << '[' << (file) << ':' << (line) << "] " << strerror(errno) << std::endl; \
    return -1;                                                                           \
}

#define CHECK(CODE) CHECK_ERROR(CODE, __FILE__, __LINE__)

int main()
{
    if (!IsExist(SHM_MEM_PATH))
    {
        std::ofstream o(SHM_MEM_PATH);
    }
    const auto key = ftok(SHM_MEM_PATH, PROJ_ID);
    CHECK(key)
    constexpr int shmMemFlags = IPC_CREAT | 0666;
    auto shmId = shmget(key, FILE_SIZE, shmMemFlags);
    CHECK(shmId)

    auto ptr = shmat(shmId, nullptr, 0);

    if (ptr == reinterpret_cast<void*>(-1))
    {
        std::cerr << '[' << __FILE__ << ':' << __LINE__ << "] " << strerror(errno) << std::endl;
        return -1;  
    }

    memset(ptr, 42, FILE_SIZE);

    CHECK(shmdt(ptr))
    CHECK(shmctl(shmId, IPC_RMID, nullptr))
    return 0;
}