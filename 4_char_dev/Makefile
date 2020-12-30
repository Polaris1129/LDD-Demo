.PHONY : all insmod app clean

obj-m := char_drv.o

KDIR :=/lib/modules/$(shell uname -r)/build

APP_SRC := char_app.c
APP_NAME := char_app

cp_dir :=$(PWD)

all: app
	@make -C $(KDIR) M=$(PWD) modules   
	@rm -fr  .tmp_versions *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~ .*.*.cmd .*.*.*.cmd
	gcc $(APP_SRC) -o $(APP_NAME)

insmod:
	sudo insmod char_drv.ko

app:
	gcc $(APP_SRC) -o $(APP_NAME)

clean:
	@make -C  $(KDIR)   M=$(PWD)  modules  clean
	@rm -f *.ko *.o *.mod.o *.mod.c  *.bak *.symvers *.markers *.unsigned *.order *~



