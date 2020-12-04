#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>

char kbuf[] = "hello linux"; //内核空间的测试数据

/**
 * @brief   对应系统调用的open
 * @param   
 * @return  
*/
static int misc_drv_open(struct inode *pinode, struct file *pfile)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的close
 * @param   
 * @return  
*/
static int misc_drv_release(struct inode *pinode, struct file *pfile)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的llseek
 * @param   
 * @return  
*/
static loff_t misc_drv_llseek(struct file *pfile, loff_t offset, int whence)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    return 0;
}

/**
 * @brief   对应系统调用的read
 * @param   
 * @return  
*/
static ssize_t misc_drv_read(struct file *pfile, char __user *buf, size_t count, loff_t *offset)
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
static ssize_t misc_drv_write(struct file *pfile, const char __user *buf, size_t count, loff_t *offset)
{
    printk("Line:%d, %s() is call!\r\n", __LINE__, __FUNCTION__);
    printk("[user space data]:%s\r\n", buf);
    return count;
}

/**
 * @brief   设备操作集合
*/
static struct file_operations misc_dev_fops = {
    .owner = THIS_MODULE,
    .open = misc_drv_open,
    .release = misc_drv_release,
    .read = misc_drv_read,
    .write = misc_drv_write,
    .llseek = misc_drv_llseek,
};

/**
 * @brief   设备描述
*/
static struct miscdevice misc_dev_dsc = {
    .minor = 255, //表示由内核动态分配一个可用的次设备号
    .name = "misc_dev",
    .fops = &misc_dev_fops};

/**
 * @brief   模块初始化函数
 * @param   
 * @return  
*/
static int __init misc_dev_init(void)
{
    int ret;

    //注册杂项设备
    ret = misc_register(&misc_dev_dsc);
    if (ret < 0)
    {
        printk("Line:%d,error misc_register\r\n", __LINE__);
        return ret;
    }

    printk("Line:%d,misc_dev_init  success!\r\n", __LINE__);

    return 0;
}

/**
 * @brief   模块卸载函数
 * @param   
 * @return  
*/
static void __exit misc_dev_exit(void)
{
    misc_deregister(&misc_dev_dsc);

    printk("Line:%d,misc_dev_exit  success!\r\n", __LINE__);
}

module_init(misc_dev_init);
module_exit(misc_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Polaris");
MODULE_VERSION("1.001");
MODULE_DESCRIPTION("misc demo test");
