#include "char_drv.h"


/**
 * @brief   设备操作集合
*/
static struct file_operations char_dev_fops = {
    .owner = THIS_MODULE,
    .open = char_drv_open,
    .release = char_drv_release,
    .read = char_drv_read,
    .write = char_drv_write,
    .llseek = char_drv_llseek,
};

char_drv_param_t char_drv_param = {
    .major = 0,
    .minor = 0,
    .dev_count = 1,
};

u8 drv_buf[DRV_BUF_LEN] = "hello linux"; //内核空间的测试数据


/**
 * @brief   对应系统调用的open
 * @param   
 * @return  
*/
static int char_drv_open(struct inode *pinode, struct file *pfile)
{
    printk("open char_drv successfully\r\n");
    return 0;
}

/**
 * @brief   对应系统调用的close
 * @param   
 * @return  
*/
static int char_drv_release(struct inode *pinode, struct file *pfile)
{
    printk("release char_drv successfully\r\n");
    return 0;
}

/**
 * @brief   对应系统调用的llseek
 * @param   
 * @return  
*/
static loff_t char_drv_llseek(struct file *pfile, loff_t offset, int whence)
{
    printk("llseek\r\n");
    return 0;
}

/**
 * @brief   对应系统调用的read
 * @param   
 * @return  
*/
static ssize_t char_drv_read(struct file *pfile, char __user *buf, size_t count, loff_t *offset)
{

    printk("char_drv, read successfully");

    if (count == 0)
    {
        return 0;
    }

    if (count > strlen(drv_buf))
    {
        count = strlen(drv_buf);
    }

    memcpy(buf, drv_buf, count);

    return count;
}

/**
 * @brief   对应系统调用的write
 * @param   
 * @return  
*/
static ssize_t char_drv_write(struct file *pfile, const char __user *buf, size_t count, loff_t *offset)
{
    printk("write data to drv_buf");

    memcpy(drv_buf, buf, count);
    return count;
}


/**
 * @brief   模块初始化函数
 * @param   
 * @return  
*/
static int __init char_dev_init(void)
{
    int ret = 0;    // 申请设备号的返回值

    if (char_drv_param.major > 0)   // 手动申请设备号
    {
        ret = register_chrdev_region(MKDEV(char_drv_param.major, char_drv_param.minor),\
                                     char_drv_param.dev_count, CHAR_DEV_NAME);
        char_drv_param.dev_nr = MKDEV(char_drv_param.major, char_drv_param.minor);
    }
    else                            // 自动申请设备号
    {
        ret = alloc_chrdev_region(&char_drv_param.dev_nr, char_drv_param.minor, char_drv_param.dev_count, CHAR_DEV_NAME);
    }

    if (ret < 0)                    // 设备号申请失败
    {
        printk(" char_drv dev_t request error!");
        return ret;
    }
    
    char_drv_param.pcdev = cdev_alloc();    // 动态分配cdev空间
    if (char_drv_param.pcdev == NULL)       // 申请失败
    {
        printk(" cdev alloc failed!");
        ret = -ENOMEM; //设置正确的错误码
        unregister_chrdev_region(char_drv_param.dev_nr, char_drv_param.dev_count);
        return ret;
    }

    cdev_init(char_drv_param.pcdev, &char_dev_fops);        // 初始化cdev

    ret = cdev_add(char_drv_param.pcdev, char_drv_param.dev_nr, char_drv_param.dev_count);  // 注册cdev
    if (ret < 0)
    {
        printk("cdev add failed!");
        kfree(char_drv_param.pcdev); //释放cdev空间
        unregister_chrdev_region(char_drv_param.dev_nr, char_drv_param.dev_count);

        return ret;
    }

    printk("cdev add sucess!\r\n ");
    printk("major = %d,minor = %d\r\n", MAJOR(char_drv_param.dev_nr), MINOR(char_drv_param.dev_nr));
}

/**
 * @brief   模块卸载函数
 * @param   
 * @return  
*/
static void __exit char_dev_exit(void)
{
    unregister_chrdev_region(char_drv_param.dev_nr, char_drv_param.dev_count);

    cdev_del(char_drv_param.pcdev);

    printk("Line:%d,char_dev_exit  success!\r\n", __LINE__);
}

module_init(char_dev_init);
module_exit(char_dev_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Polaris");
MODULE_VERSION("1.001");
MODULE_DESCRIPTION("char driver demo");
