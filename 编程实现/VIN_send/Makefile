vnisend:vnisend.c macSendf.o macRecvf.o
	gcc -o vnisend vnisend.c macSendf.o macRecvf.o
macSendf.o:macSendf.c macSendf.h
	gcc -c -o macSendf.o macSendf.c
macRecvf.o:macRecvf.c macRecvf.h
	gcc -c -o macRecvf.o macRecvf.c
.PHONY:clean
clean:
	-rm -f vnisend macSendf.o macRecvf.o