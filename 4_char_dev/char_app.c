#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

typedef unsigned char   u8;
typedef unsigned short  u16;
typedef unsigned int    u32;

#define DEVNAME "/dev/char_dev_1"

u8 usr_cmd[10];

int main(int argc, char *argv[])
{
    int ret;
    int fd;                     // 文件描述符
    char* filename = NULL;      // 文件名
    char data[] = "123456789abcdefg";
    char buf[1024] = {0};

    printf("argc:%d\r\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d]:%s\r\n", i, argv[i]);
    }

    if (argc == 1)
    {
        filename = DEVNAME;
    }
    else
    {
        filename = argv[1];
    }

    while(1)
    {
        scanf("%s", usr_cmd);

        if (0 == strcmp(usr_cmd, "open"))
        {
            fd = open(filename, O_RDWR);            // 打开设备文件
            if (fd < 0)
            {
                perror("open");
                printf("open %s, failed\r\n", filename);
                return -1;
            }
            printf("open %s, successful\r\n", filename);
        }
        else if (0 == strcmp(usr_cmd, "write"))
        {
            ret = write(fd, data, strlen(data));    // 执行写操作
            if (ret < 0)
            {
                perror("write");
                printf("write %s, failed\r\n", filename);
                return -1;
            }

            printf("write %s, successful, len: %d\r\n", filename, (int)strlen(data));
        }
        else if (0 == strcmp(usr_cmd, "read"))
        {
            ret = read(fd, buf, 1000);
            if (ret < 0)
            {
                perror("read");
                return -1;
            }
            printf("buf:%s\r\n", buf);
            printf("read len:%d\r\n", ret);
        }
        else if (0 == strcmp(usr_cmd, "close"))
        {
            close(fd);
        }
    }
}
