obj-m += kern_mod.o

all:
    make -C /lib/modules/$(shell 5.19.9)/build M=$(PWD) modules

clean:
    make -C /lib/modules/$(shell 5.19.9)/build M=$(PWD) clean