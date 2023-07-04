// MANUEL COBOS RODRIGUEZ 49367964M PA1


#include <stdio.h>
#include "indice.h"

int busquedaBinariaIndice(int numMat, char *dni, FILE *f){
	if (f==NULL){
		return -3;
	}
	tipoIndice dev;
	int cuantos;
	int valMedio;
	while(!feof(f)){
		fread(&dev, sizeof(tipoIndice), 1, f);
		//printf("\nDNI: %s\tMATRICULA: %d", dev.dni, dev.numMat);
	}
	rewind(f);
	fseek(f, 0, SEEK_END);
	cuantos = ftell(f) / sizeof(tipoIndice);
	int izda =0 , der = cuantos-1;
	int indMedio;
	rewind(f);
	while (izda <= der){
		indMedio = (izda+der)/2;
		fseek(f, indMedio*sizeof(tipoIndice), SEEK_SET);
		fread(&dev, sizeof(tipoIndice), 1, f);

		if(dev.numMat == numMat){
			strcpy(dni, dev.dni);
			return 0;
		}
		if(dev.numMat < numMat){
			izda=indMedio+1;
			rewind(f);
		}
		else{
			der=indMedio-1;
			rewind(f);
		}

	}
	return -1;
}
tipoAlumno * busqueda(int numMat, char *ficheroDatos,char *ficheroIndice, int *error){
	FILE *find;
	FILE *fdat;
	tipoIndice indice;
	tipoAlumno *alumno;
	int numReg;
	char *dni;

	if((alumno=(tipoAlumno *)malloc(sizeof(tipoAlumno)))==NULL){
		*error=4;
		return NULL;
	}

	if((find=fopen(ficheroIndice ,"rb"))==NULL){
		*error=3;
		return NULL;
	}

	if((fdat=fopen(ficheroDatos, "rb"))==NULL){
		*error=-2;
		return NULL;
	}

	if(busquedaBinariaIndice(&numMat, dni, ficheroIndice)!=0){
		*error=4;
		return NULL;
	}
	while(fread(alumno, sizeof(tipoAlumno *), 1, fdat)){
		if((strcmp(alumno->dni, dni))==0){
			*error=0;
			return alumno;

		}
		else{
			if((fseek(fdat, sizeof(tipoAlumno), SEEK_CUR))!=0){
				*error = 4;
				return NULL;
			}
			if((fread(alumno, sizeof(tipoAlumno *), 1, fdat))!=1){
				*error=1;
				return NULL;
			}
		}
	}
	fclose(find);
	fclose(fdat);

}
