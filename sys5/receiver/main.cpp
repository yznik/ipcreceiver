#include <iostream>
#include <fstream>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

template<size_t SIZE>
struct message {
    long m_type;
    char m_data[SIZE] = {0};
    inline size_t capacity() const noexcept
    {
        return SIZE;
    }
    inline const char* data() const noexcept
    {
        return m_data;
    }
};

inline bool IsExist(const std::string& name) noexcept
{
    struct stat buffer;
    auto res = stat (name.c_str(), &buffer);
    if (res == -1)
    {
        std::cerr << '[' << __FILE__ << ':' << __LINE__ << "] " << strerror(errno) << std::endl;
        return true;
    }
    return res == 0; 
}

constexpr const char* const IN_PATH  = "/tmp/msg.temp";
constexpr int PROJ_ID                = 1;
constexpr const char* const OUT_PATH = "/home/box/message.txt";

#define CHECK_ERROR(CODE, file, line)                                                    \
if (CODE == -1)                                                                          \
{                                                                                        \
    std::cerr << '[' << (file) << ':' << (line) << "] " << strerror(errno) << std::endl; \
    return -1;                                                                           \
}

#define CHECK(CODE) CHECK_ERROR(CODE, __FILE__, __LINE__)

int main()
{
    if (!IsExist(IN_PATH))
    {
        std::ofstream o(IN_PATH);
    }
    const auto key = ftok(IN_PATH, PROJ_ID);
    CHECK(key)
    int queueFlags = IPC_CREAT | 0666;
    auto queueId = msgget(key, queueFlags);
    CHECK(queueId)
    std::ofstream f(OUT_PATH);
    while (true)
    {
        message<80> msg;
        auto received = msgrcv(queueId, &msg, msg.capacity(), 0, 0);
        CHECK(received)
        f << msg.data() << std::endl;
    }
    CHECK(msgctl(queueId, IPC_RMID, nullptr))
    return 0;
}