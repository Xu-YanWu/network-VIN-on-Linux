vinrecv:vinrecv.c macRecvf.o
	gcc -o vinrecv vinrecv.c macRecvf.o
macRecvf.o:macRecvf.c macRecvf.h
	gcc -c -o macRecvf.o macRecvf.c
.PHONY:clean
clean:
	-rm -f vinrecv macRecvf.o