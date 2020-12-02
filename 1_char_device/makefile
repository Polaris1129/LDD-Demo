

obj-m := char_device.o

#内核源码路径，当编译为X86 的驱动时候使用以下这个路径
KDIR :=/lib/modules/$(shell uname -r)/build


#X86内核源码目录
#KDIR=/lib/modules/`uname -r`/build 

cp_dir :=~/Desktop/

all:
	@make -C $(KDIR) M=$(PWD) modules   
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
	@cp -fv *.ko ${cp_dir}
clean:
	@make -C  $(KDIR)   M=$(PWD)  modules  clean
	@rm -f *.ko *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~



