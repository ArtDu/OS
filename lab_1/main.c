#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>

int main(int argc, char* argv[]) {

    int ret;
    const char pathPython[] = "hello.py";
    const char list[] = "list.bin";

    //is it exists?
    ret = access(pathPython, F_OK);
    if (ret == 0)
        printf("access says: %s exists\n", pathPython);
    else
        printf("access says: %s not exists or can not be reached\n",
                pathPython);

    //open file
    int fd = open(list, O_RDWR | O_CREAT);
    if (fd < 0)
        printf("Cannot open file");
    else
        printf("file '%s' opened with file descriptor %d\n", list, fd);

    const int value = 109;
    ret = write(fd, &value, sizeof(value));
    printf("write wrote %d bytes to fd:%d and this is int:%d\n", ret, fd,
            value);

    off_t shift = 0;
    ret = lseek(fd, shift, SEEK_SET);
    printf("lseek set fd:%d offset to %d from begging of file\n", fd, ret);
    int input;
    ret = read(fd, &input, sizeof(input));
    printf("read read %d bytes from fd:%d and this is int:%d\n", ret, fd,
            input);

    struct stat info;
    //read info about file by fd
    //stat does same but file described with char*
    fstat(fd, &info);
    printf(
            "some info about fd:%d\nsize:%ld B\nconsists from %ld blocks of 512 B\nblocksize of system is %ld B\n",
            fd, info.st_size, info.st_blocks, info.st_blksize);

    fchmod(fd, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
    //user - rw
    //group, others - r
    ret = close(fd);
    if (ret < 0) {
        printf("file with fd:%d can not be closed\n", fd);
        return -1;
    }

    time_t t = time(0);

    printf("Hours past from 1970 : %ld\n", t / (3600 * 24));


    return 0;
}

