Name - Nitin Goyal
Bnumber - B00767934
Bmail - ngoyal1@binghamton.edu



To run:

	In terminal type make and then using super user privilege instructions type:
	insmod numpipe.ko bufsize=10
	./producer /dev/numpipe
	./consumer /dev/numpipe