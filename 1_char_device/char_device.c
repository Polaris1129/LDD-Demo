#include <linux/delay.h>
#include <linux/ide.h>
#include <linux/init.h>
#include <linux/module.h>

static int chrtest_open(struct inode *inode, struct file *filp);
static ssize_t chrtest_read(struct file *filp, char __user *buf, size_t cnt, loff_t *offt);
static ssize_t chrtest_write(struct file *filp, const char __user *buf, size_t cnt, loff_t *offt);
static int chrtest_release(struct inode *, struct file *);

dev_t chr_dev_num;


u8 dev_data[100] = {0};

static struct file_operations test_fops = { 
    .owner = THIS_MODULE,
    .open = chrtest_open,
    .read = chrtest_read,
    .write = chrtest_write,
    .release = chrtest_release,
};

/**
 * @brief   打开操作
 * @param   p_inode 
 * @param   p_file 
 * @return  0成功，其他失败
*/
static int chrtest_open(struct inode* p_inode, struct file* p_file)
{

    return 0;
}

/**
 * @brief   读操作
 * @param   p_file  文件描述符
 * @param   p_buf   用户读取数据缓冲区
 * @param   cnt     读取字节数
 * @param   p_offt  读取位置
 * @return  
*/
static ssize_t chrtest_read(struct file * p_file, char __user * p_buf, size_t cnt, loff_t * p_offt)
{
    int retvalue;

    retvalue = copy_to_user(p_buf, &dev_data[*p_offt], cnt);
	if(retvalue == 0)
    {
		printk("kernel send data ok!\r\n");
	}
    else
    {
		printk("kernel send data failed!\r\n");
	}
    return 0;
}

/**
 * @brief   写操作
 * @param   p_file  文件描述符
 * @param   p_buf   用户写数据缓冲区
 * @param   cnt     写字节数
 * @param   p_offt  写位置
 * @return  
*/
static ssize_t chrtest_write(struct file *p_file, const char __user * p_buf, size_t cnt, loff_t *p_offt)
{
    int retvalue;

    retvalue = copy_from_user(&dev_data[*p_offt], p_buf, cnt);

	if(retvalue == 0)
    {
		printk("kernel recevdata:%s\r\n", p_buf);
	}
    else
    {
		printk("kernel recevdata failed!\r\n");
	}
    return 0;
}

/**
 * @brief   
 * @param   
 * @return  
*/
static int chrtest_release(struct inode* p_inode, struct file * p_file)
{

}

static int __init chrtest_init(void)
{
    alloc_chrdev_region(&chr_dev_num, 0, 1, "chr_test");
    return 0;
}

/**
 * @brief   
 * @param   
 * @return  
*/
static void __exit chrtest_exit(void)
{
    unregister_chrdev_region(&chr_dev_num, 1);
}

module_init(chrtest_init);
module_exit(chrtest_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Bruce Hill");