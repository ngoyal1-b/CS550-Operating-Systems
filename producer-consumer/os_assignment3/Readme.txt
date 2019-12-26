Part A:

Kernel Modules : Modules are pieces of code that can be loaded and unloaded into the kernel upon demand. They extend the functionality of the kernel without the need to reboot the system.

In linux operating system we can see what modules are initially loaded into the kernel by running a command called lsmod 

modprobe - add or remove modules from the linux kernel
kmod - program to manage linux kernel modules

when kernel needs a feature that is not resident in the kernel, the kernel module daemon kmod execs modprobe to lead the module in modprobe to lead the module.

in modprobe a string is passed in 2 forms:
1) module name
2) a generic identifier like char -major -10 -30

when modprobe is handed a generic identifier, it will look for the string in the file /etc/modules.conf and after that modprobe looks throught the file /lib/modules/version/modules.dep, to see if other modules to be loaded before the requested module to be loaded. Lastly, modprobe uses insmod to first load any prerequisite modules into the kernel, and then the requested module. modprobe directs insmod to /lib/modules/version, the standard directory for modules.

Linux distors provide modprobe, insmod and depmod as a package called modutils or mod-utils

Part B:

1)Running 1 consumer and 1 producer concurrently:

when we run producer.c and consumer. c on different terminal for producer and consumer to work we have to create a pipe using mkfifo command. When i run my program i have a created a pipe called buf and called an object of producer.c(./producer) and consumer.c(./consumer) and passed a pipe as an argument. In producer function there were 4 bytes written and in consumer function 4 bytes were read.

Killing the producer and leaving consumer running: When we kill producer the consumer function displays an error because producer is not able to produce an item so howcome consumer will consume one there is nothing to read for a consumer.

Killing the consumer and leaving producer running : when we kill consumer the producer function displays an error something like pipe broken that shows there is an error in writing a bytes to a pipe.


2) Running 1 consumer and multiple producer concurrently:

when we run 3 producer on different terminal and a consumer on one terminal the producers are producing an item every second whereas the consumers consumes like 3 items in a second and when we terminate any producer the consumer starts displaying data for 2 producers. Because nuber of items produces by producer are consumed by consumer.

3) Running 1 producer and multiple consumer concurrently:

when i run 3 consumer on different terminal and a producer on one terminal the race condition will occur because we observed there is a single producer which produces an item in let's say 1 sec and there are multiple consumer to consume that particular item so at particular time any consumer consume that item, some consumer has to remain sit idle while other get data continuously

4) Running multiple consumer and multiple producer concurrently:

When we run same number of consumer and same number of producer then the data produced by one producer will be equal to amount of data consumed by one consumer whereas we if the number of producer are more than the number of consumers then some consumers get less data (race condition) on the other hand if the number of consumer is more than the number of producers then some consumer will get data while some consumer will not get data. 




