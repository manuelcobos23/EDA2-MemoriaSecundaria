// MANUEL COBOS RODRIGUEZ 49367964M PA1


#include "dispersion.h"

tipoAlumno * busquedaHash(FILE *f, char *dni, int *nCubo, int *ncuboDes, int *posReg, int *error){
	*nCubo = atoi(dni)%CUBOS;
	*posReg = 0;
	int total;
	tipoCubo cubo;
	tipoAlumno *encontrado = malloc(sizeof(tipoAlumno));
	fseek(f, (*nCubo)*sizeof(tipoCubo), SEEK_SET);
	fread(&cubo, sizeof(tipoCubo), 1, f);
	while((*posReg) < C && (*posReg) < cubo.numRegAsignados ){
		fread(encontrado, sizeof(tipoAlumno), 1, f);
		if(strcmp(encontrado->dni, dni) == 0){
			*ncuboDes = -1;
			*error=0;
			return encontrado;
		}else{
			(*posReg)++;
		}
	}
	//Si sale de el while significa que el registro no esta en su cubo correspondiente unicamente pordrá estar en los de desborde
	total = CUBOS;
	int i;
	while(!feof(f)){
		i=0;
		fseek(f, total*sizeof(tipoCubo), SEEK_SET);
		while(i < C){
			fread(encontrado, sizeof(tipoAlumno),1, f);
			if(strcmp(encontrado->dni, dni) == 0){
				*ncuboDes = total;
				*posReg = i;
				*error=0;
				return encontrado;
			}else{
				i++;
			}
		}//Secuencialmente todos los registros del cubo total;
		total++;
	}
	*error = -1;
	return NULL;
}

int modificarReg(char *fichero, char *dni, char *provincia){
	FILE *fd;
	tipoAlumno * encontrado = malloc(sizeof(tipoAlumno));
	int nCubo, ncuboDes, error, posReg;
	fd = fopen(fichero, "rb+");
	if(fd == NULL){
		printf("Error al abrir el fichero.\n");
		return -2;
	}
	encontrado = busquedaHash(fd, dni, &nCubo, &ncuboDes, &posReg, &error);
	if(error == 0){
		strcpy(encontrado->provincia, provincia);
		if(ncuboDes == -1){
			fseek(fd, nCubo*sizeof(tipoCubo), SEEK_SET);
		}
		else{
			fseek(fd, ncuboDes*sizeof(tipoCubo), SEEK_SET);
		}
		fseek(fd, posReg*sizeof(tipoAlumno), SEEK_CUR);
		fwrite(encontrado, sizeof(tipoAlumno), 1, fd);
		fclose(fd);
		return 0;
	}else if(error == -1){
		printf("El registro no existe\n");
		return -1;
	}else{
		printf("Error en la funcion busquedaHash\n");
		return -2;
	}
}

int creaHash(char *fichEntrada,char *fichHash){
	FILE *fEntrada, *fHash;
	tipoAlumno reg;
	tipoCubo cubo;
	fpos_t pos;
	int i,numDesb=0;

	creaHvacio(fichHash);

	fEntrada = fopen(fichEntrada, "rb");
	fHash = fopen(fichHash, "r+b");
	fread(&reg, sizeof(reg),1,fEntrada);

	while(!feof(fEntrada)){
		// Procesamiento, la clave es fseek
		//Calcular la funcion para asignar el cubo
		int numCubo = atoi(reg.dni)%CUBOS;
		//printf("%d->%d\n",an, numCubo);
		fseek(fHash,numCubo*sizeof(cubo), SEEK_SET);
		fgetpos(fHash,&pos);
		fread(&cubo,sizeof(cubo),1,fHash);
		i=cubo.numRegAsignados;
		if(i >=C){
			numDesb++;
			cubo.numRegAsignados++;
			//Tratar desbordes
			if(!desborde(fHash,reg,CUBOS,CUBOSDESBORDE)) printf("registro no almacenado");

		}else{
			cubo.reg[i]= reg;
			cubo.numRegAsignados++;
		}
		fsetpos(fHash,&pos); //Me vuelvo a situal en el cubo en el que estaba
		//fseek(fHash,numcubo*sizeof(cubo), SEEK_SET); tambien se puede hacer asi
		fwrite(&cubo, sizeof(cubo), 1, fHash); //Tengo que hacer esto para modificarlo en memoria secundaria

		fread(&reg, sizeof(reg),1,fEntrada);
	}
	fclose(fEntrada);
	fclose(fHash);
	return numDesb;
}

int desborde(FILE * fHash, tipoAlumno reg, int cubos, int cubosDesborde){
	int i, nCubos;
	fpos_t pos;
	tipoCubo cubo;
	memset(&cubo,0,sizeof(cubo));
	nCubos=cubos;
	while(nCubos < cubos+cubosDesborde) {
		fseek(fHash, nCubos*sizeof(cubo), SEEK_SET);
		fgetpos(fHash, &pos);
		fread(&cubo, sizeof(cubo),1,fHash);
		i = cubo.numRegAsignados;
		if(i < C){
			cubo.reg[i] = reg;
			cubo.numRegAsignados++;
			fsetpos(fHash, &pos);
			fwrite(&cubo,sizeof(cubo),1,fHash);
			return 1;
		}else{
			nCubos++;
		}

	}
	return 0;
}

void creaHvacio(char *fichHash)
{ FILE *fHash;
  tipoCubo cubo;
  int j;
  int numCubos =CUBOS+CUBOSDESBORDE;

  memset(&cubo,0,sizeof(cubo)); //Inicializa las variables a su valor nulo, enteros a 0 y cadenas a NULL;

  fHash = fopen(fichHash,"wb");
  for (j=0;j<numCubos;j++) fwrite(&cubo,sizeof(cubo),1,fHash);
  fclose(fHash);
}

int leeHash(char *fichHash)
{ FILE *f;
  tipoCubo cubo;
  int j,i=0;
  size_t numLee;

   f = fopen(fichHash,"rb");
   rewind(f);
   fread(&cubo,sizeof(cubo),1,f);
   while (!feof(f)){
		for (j=0;j<C;j++) {
        	if (j==0)	printf("Cubo %2d (%2d reg. ASIGNADOS)",i,cubo.numRegAsignados);
        	else  	printf("\t\t\t");
		if (j < cubo.numRegAsignados)
		    printf("\t%s %s %s %s %s\n",
	    	cubo.reg[j].dni,
			cubo.reg[j].nombre,
			cubo.reg[j].ape1,
		  	cubo.reg[j].ape2,
  	        cubo.reg[j].provincia);
		else printf ("\n");
    	}
       i++;
       fread(&cubo,sizeof(cubo),1,f);
   }
   fclose(f);
   return i;
}
