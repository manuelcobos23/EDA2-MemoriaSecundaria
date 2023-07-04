CC = cc

archivoPrueba: dispersion.o  indice.o archivoPrueba.o
	$(CC) -g archivoPrueba.o dispersion.o indice.o -o archivoPrueba

archivoPrueba.o: archivoPrueba.c dispersion.h indice.h
	$(CC) -c archivoPrueba.c -o archivoPrueba.o

dispersion.o: dispersion.c dispersion.h
	$(CC) -c dispersion.c -o dispersion.o

indice.o: indice.c indice.h
	$(CC) -c indice.c -o indice.o

limpiar:
	rm *.o archivoPrueba
