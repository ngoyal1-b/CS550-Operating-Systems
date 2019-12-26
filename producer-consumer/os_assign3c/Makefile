obj-m = numpipe.o

KBUILD_CFLAGS += -w

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules $(KBUILD_CFLAGS)
	gcc producer_numbers.c -o producer
	gcc consumer_numbers.c -o consumer
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm -f ./producer ./consumer

