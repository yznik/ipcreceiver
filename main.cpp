#include <iostream>
#include <fstream>
#include <array>

#include <fcntl.h>
#include <sys/stat.h>
#include <mqueue.h>
#include <string.h>

constexpr const char* const IN_PATH  = "/test.mq";
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
    mode_t perms = 0666;
    int flags = O_RDWR | O_CREAT;
    auto queueId = mq_open(IN_PATH, O_RDWR | O_CREAT, perms, nullptr);
    CHECK(queueId)
    std::ofstream f(OUT_PATH);
    while (true)
    {
        std::array<char, 8192> msg;
        auto received = mq_receive(queueId, msg.data(), msg.size(), nullptr);
        CHECK(received)
        f << msg.data() << std::endl;
    }
    CHECK(mq_close(queueId))
    return 0;
}