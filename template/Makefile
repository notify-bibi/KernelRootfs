CONFIG_MODULE_SIG=n
obj-m += test_src.o

KDIR =/home/u18/source/linux-4.4.72

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.* *.symvers *.order
