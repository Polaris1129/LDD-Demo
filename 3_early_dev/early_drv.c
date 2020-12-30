#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

char kbuf[100] = "hello linux"; //内核空间的测试数据

#define EARLY_DEV_NAME "early_char_demo" //设备名

static unsigned int major = 111; //major==0 表示由内核动态分配一个可用的主设备号

/**
 * @brief   对应系统调用的open
 * @param   
 * @return  
*/
static int early_drv_open(struct inode *pinode, struct file *pfile)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的close
 * @param   
 * @return  
*/
static int early_drv_release(struct inode *pinode, struct file *pfile)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的llseek
 * @param   
 * @return  
*/
static loff_t early_drv_llseek(struct file *pfile, loff_t offset, int whence)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的read
 * @param   
 * @return  
*/
static ssize_t early_drv_read(struct file *pfile, char __user *buf, size_t count, loff_t *offset)
{

    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);

    //参数检测
    if (count == 0)
    {
        return 0;
    }
    //如果用户需要读取的数量大于设备能提供数据字节数量，修改为最大数量
    if (count > strlen(kbuf))
    {
        count = strlen(kbuf);
    }

    //这里仅做测试用，说明系统调用和驱动接口的参数传递关系，
    //这种直接使用用户空间传递的指针写数据是不安全，后面会修改为专用函数
    strncpy(buf, kbuf, count);

    return count;
}

/**
 * @brief   对应系统调用的write
 * @param   
 * @return  
*/
static ssize_t early_drv_write(struct file *pfile, const char __user *buf, size_t count, loff_t *offset)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    printk("[user space data]:%s\r\n", buf);
    memcpy(kbuf, buf, count);
    return count;
}

/**
 * @brief   设备操作集合
*/
static struct file_operations early_dev_fops = {
    .owner = THIS_MODULE,
    .open = early_drv_open,
    .release = early_drv_release,
    .read = early_drv_read,
    .write = early_drv_write,
    .llseek = early_drv_llseek,
};


/**
 * @brief   模块初始化函数
 * @param   
 * @return  
*/
static int __init early_dev_init(void)
{
    int ret;

    ret = register_chrdev(major, EARLY_DEV_NAME, &early_dev_fops);
    major = ret;


    if (ret < 0)
    {
        printk("Line:%d,error early_register\r\n", __LINE__);
        return ret;
    }

    printk("Line:%d,early_dev_init  success!\r\n", __LINE__);

    return 0;
}

/**
 * @brief   模块卸载函数
 * @param   
 * @return  
*/
static void __exit early_dev_exit(void)
{
    unregister_chrdev(major, EARLY_DEV_NAME);

    printk("Line:%d,early_dev_exit  success!\r\n", __LINE__);
}

module_init(early_dev_init);
module_exit(early_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Polaris");
MODULE_VERSION("1.001");
MODULE_DESCRIPTION("early demo test");
