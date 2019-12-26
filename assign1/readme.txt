Name : Nitin Goyal
Bnumber : B00767934
Department : Computer Science
Subject : Operating System
Assignment1 : Creating and Destroying a Process Hierarchy


Write a program that will recursively create a process hierarchy tree that is H levels deep, print out the process tree information as the tree is being created, and then systematically terminate all the processes in the tree.


Firstly created a main function with a argument passed as argc and argv for the command line argument argc is to keep the count of number of arguments whereas argv is for arguments like argv[0] is used to keep the file name. In my program argv[1] is used for height of the tree and argv[2] is used for number of children.

When height of tree is greater than 1 , creating a child processes with the help of fork() system call. fork() system call return 3 type of values, if fork() return a process id as negative value it's failure of fork system call, when fork() return the process id 0 it's the child process for positive process_id it's a parent process.

execlp() system call is used to create a new program image in execution. execlp is passed with number of arguments file pathname (argv[0]), value of h and c since we convert the value of h and c to integers through function called atoi() for passing in execlp command we have to change it to string again for that i used a function call sprintf(). execlp() here is used for recursion.

exit() system call is called in child process which results in terminating of process. exit(0) the return status (0 here) is passed on parent. this is called voluntary termination.

different function like getpid() -> return the process id of process, getppid() -> which returns the parent process id is also used.

