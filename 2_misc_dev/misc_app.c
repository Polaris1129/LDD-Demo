#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define DEVNAME "/dev/misc_dev_1"

int main(int argc, char *argv[])
{
    int ret;
    int fd;
    const char *filename;
    char data[] = "123456789abcdefg";
    char buf[1024] = {0};

    int i;
    printf("argc:%d\r\n", argc);
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d]:%s\r\n", i, argv[i]);
    }

    if (argc == 1)
    {
        filename = DEVNAME;
    }
    else if (argc == 2)
    {
        filename = argv[1];
    }
    else
    {
        printf("Usage:%s [filename]\r\n", argv[0]);
        return 0;
    }

    printf("Open file [%s]\r\n", filename);

    //打开文件
    fd = open(filename, O_RDWR);
    if (fd < 0)
    {
        perror("open");
        return -1;
    }

    printf("data len:%d\r\n", strlen(data));
    //写入数据
    ret = write(fd, data, strlen(data));
    if (ret < 0)
    {
        perror("write");
        return -1;
    }

    printf("write len:%d\r\n", ret);

    //移动光标到文件开头
    ret = lseek(fd, 0, SEEK_SET);
    if (ret < 0)
    {
        perror("lseek");
        return -1;
    }
    printf("lseek ret:%d\r\n", ret);

    //读取数据
    ret = read(fd, buf, 1000);
    if (ret < 0)
    {
        perror("read");
        return -1;
    }
    printf("buf:%s\r\n", buf);
    printf("read len:%d\r\n", ret);

    //关闭文件，可选，当进程结束会释放它打开所有文件。
    close(fd);

    return 0;
}
