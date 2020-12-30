#ifndef _CHAR_DRV_H_
#define _CHAR_DRV_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>



static int char_drv_open(struct inode *pinode, struct file *pfile);
static int char_drv_release(struct inode *pinode, struct file *pfile);
static loff_t char_drv_llseek(struct file *pfile, loff_t offset, int whence);
static ssize_t char_drv_read(struct file *pfile, char __user *buf, size_t count, loff_t *offset);
static ssize_t char_drv_write(struct file *pfile, const char __user *buf, size_t count, loff_t *offset);




#define CHAR_DEV_NAME "char_devs" //设备名

#define DRV_BUF_LEN     100         // 驱动中的数组长度


typedef struct char_drv_param
{
    u32 major;          //主设备号
    u32 minor;          //次设备号
    u32 dev_count;      //设备号数量
    dev_t dev_nr;       //设备号
    struct cdev* pcdev; //字符设备核心结构指针
} char_drv_param_t;






#endif // _CHAR_DRV_H_