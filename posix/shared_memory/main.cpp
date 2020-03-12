#include <iostream>

#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>

constexpr const char* const SH_MEM_PATH  = "/test.shm";
constexpr off_t FILE_SIZE = 1024 * 1024;

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
    auto fd = shm_open(SH_MEM_PATH, O_RDWR | O_CREAT, perms);
    CHECK(fd)
    CHECK(ftruncate(fd, FILE_SIZE))
    auto raw_ptr = mmap(nullptr, FILE_SIZE, PROT_WRITE, MAP_SHARED, fd, 0);
    if (raw_ptr == MAP_FAILED)
    {
        std::cerr << '[' << __FILE__ << ':' << __LINE__ << "] " << strerror(errno) << std::endl;
        return -1;
    }
    memset(raw_ptr, 13, FILE_SIZE);
    CHECK(munmap(raw_ptr, FILE_SIZE))
    //CHECK(shm_unlink(SH_MEM_PATH))
    return 0;
}