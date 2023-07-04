// MANUEL COBOS RODRIGUEZ 49367964M PA1

#include "dispersion.h"
#include "indice.h"
int main(){

  /* CODIGO PARA PROBAR QUE FUNCIONAN LAS FUNCIONES DE ORGANIZACIÓN DIRECTA. DISPERSIÓN*/
   tipoAlumno *encontrado;
   int nCubo, ncuboDes, posReg, error=-1, control=0;
   FILE *f;
   //Lo primero que hago es crear el fichero con organización directa "alumnos.hash"
   printf("Deseas reinicial el fichero \"alumnos.hash\"??\n->(1)Si\n->(Otra cosa)No\n");
   scanf("%d", &control);
   if(control)
     creaHash("alumnos.dat","alumnos.hash");
   //Una vez creado busco por el hash correspondiente
   leeHash("alumnos.hash");
   f = fopen("alumnos.hash", "rb");
   if(f == NULL){
     printf("Error al abrir el fichero alumnos.hash\n");
     return -1;
   }
   encontrado = busquedaHash(f, "12515152", &nCubo, &ncuboDes, &posReg, &error);
   fclose(f);
   if(error == 0){
     printf("[*] Registro:");
     printf("\t %s %s %s %s %s\n\n",encontrado->dni,encontrado->nombre,encontrado->ape1,encontrado->ape2,encontrado->provincia);
     printf("[->] El numero de cubo es: %d\n", nCubo);
     printf("[->] El numero de cubo de desborde es: %d\n", ncuboDes);
     printf("[->] La posicion del registro es (empezando en 0): %d\n", posReg);
   }else if(error == -1){
     printf("No existe el registro\n");
   }else{
     printf("Error en la funcon: Cod. error [%d]\n", error);
   }
   control = modificarReg("alumnos.hash", "12515152", "AGUILAR DE");
   if(control == 0){
     printf("Modificado!!!\n");
   }
	int val;
	char * dni;
	int numMat;
	tipoAlumno * puntAlumno;
	FILE *d;
	d = fopen("numMat.idx", "rb");
	printf("****Busqueda binaria mediante indices****\n\n");
	numMat= 9172;
	val = busquedaBinariaIndice(numMat, dni, d);
	if (val == 0){//Registro encontrado
		printf("\n\nEl DNI del numero de matricula %d es %s.\n\n", numMat, dni);
	}
	if (val == -1){
		printf("\n\nEl numero de matricula %d no fue encontrado.\n\n", numMat);
	}
	fclose(d);

	/*int * er;
	puntAlumno = busqueda(numMat, "alumnos.hash","numMat.idx", er);

	if (puntAlumno!=NULL && *er==0){
	printf("\nLos datos del alumno con matricula %d son:\n\n", numMat);
	printf("%s\t%s %s %s \t %s\n", puntAlumno->dni, puntAlumno->nombre, puntAlumno->ape1, puntAlumno->ape2, puntAlumno->provincia);
	}

	*/

}
