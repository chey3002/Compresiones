
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
/* Tipo nodo para �rbol */
typedef struct _nodo
{
   char letra;             /* Letra a la que hace referencia el nodo */
   unsigned long int bits; /* Valor de la codificaci�n de la letra */
   char nbits;             /* N�mero de bits de la codificaci�n */
   struct _nodo *cero;            /* Puntero a la rama cero de un �rbol */
   struct _nodo *uno;             /* Puntero a la rama uno de un �rbol */
} tipoNodo;                /* Nombre del tipo */

/* Funciones prototipo */
void BorrarArbol(tipoNodo *n);

int main(int argc, char *argv[])
{
	int pasadastiempos=0;
for(pasadastiempos=0;pasadastiempos<10;pasadastiempos++){
		clock_t start = clock();
   tipoNodo *Arbol;        /* Arbol de codificaci�n */
   long int Longitud;      /* Longitud de fichero */
   int nElementos;         /* Elementos de �rbol */
   unsigned long int bits; /* Almacen de bits para decodificaci�n */
   FILE *fe, *fs;          /* Ficheros de entrada y salida */

   tipoNodo *p, *q;        /* Auxiliares */
   unsigned char a;
   int i, j;


   /* Crear un arbol con la informaci�n de la tabla */
   Arbol = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
   Arbol->letra = 0;
   Arbol->uno = Arbol->cero = NULL;
   fe = fopen("comprimido.txt", "rb");
   fread(&Longitud, sizeof(long int), 1, fe); /* Lee el n�mero de caracteres */
   fread(&nElementos, sizeof(int), 1, fe); /* Lee el n�mero de elementos */
   for(i = 0; i < nElementos; i++) /* Leer todos los elementos */
   {
      p = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
      fread(&p->letra, sizeof(char), 1, fe); /* Lee el car�cter */
      fread(&p->bits, sizeof(unsigned long int), 1, fe); /* Lee el c�digo */
      fread(&p->nbits, sizeof(char), 1, fe); /* Lee la longitud */
      p->cero = p->uno = NULL;
      /* Insertar el nodo en su lugar */
      j = 1 << (p->nbits-1);
      q = Arbol;
      while(j > 1)
      {
         if(p->bits & j) /* es un uno*/
            if(q->uno) q = q->uno;   /* Si el nodo existe, nos movemos a �l */
            else                     /* Si no existe, lo creamos */
            {
               q->uno = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
               q = q->uno;
               q->letra = 0;
               q->uno = q->cero = NULL;
            }
         else /* es un cero */
            if(q->cero) q = q->cero; /* Si el nodo existe, nos movemos a �l */
            else                     /* Si no existe, lo creamos */
            {
               q->cero = (tipoNodo *)malloc(sizeof(tipoNodo)); /* un nodo nuevo */
               q = q->cero;
               q->letra = 0;
               q->uno = q->cero = NULL;
            }
         j >>= 1;  /* Siguiente bit */
      }
      /* Ultimo Bit */
      if(p->bits & 1) /* es un uno*/
         q->uno = p;
      else            /* es un cero */
         q->cero = p;
   }
   /* Leer datos comprimidos y extraer al fichero de salida */
   bits = 0;
   fs = fopen("descomprimido.txt", "w");
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   bits <<= 8;
   fread(&a, sizeof(char), 1, fe);
   bits |= a;
   j = 0; 
   q = Arbol;
   
   do {
      if(bits & 0x80000000) q = q->uno; else q = q->cero; /* Rama adecuada */
      bits <<= 1;           /* Siguiente bit */
      j++;
      if(8 == j)            /* Cada 8 bits */
      {
         i = fread(&a, sizeof(char), 1, fe); /* Leemos un byte desde el fichero */
         bits |= a;                    /* Y lo insertamos en bits */
         j = 0;                        /* No quedan huecos */
      }                                
      if(!q->uno && !q->cero)          /* Si el nodo es una letra */
      {
         putc(q->letra, fs);           /* La escribimos en el fich de salida */
         Longitud--;                   /* Actualizamos longitud que queda */
         q=Arbol;                      /* Volvemos a la raiz del �rbol */
      }
   } while(Longitud);                  /* Hasta que acabe el fichero */
   /* Procesar la cola */

   fclose(fs);                         /* Cerramos ficheros */
   fclose(fe);

   BorrarArbol(Arbol);                 /* Borramos el �rbol */
   printf("Tiempo transcurrido: %fseg\n", ((double)clock() - start) / CLOCKS_PER_SEC);/*tiempo transcurrido*/
	}
   system("pause");
   return 0;
}

/* Funci�n recursiva para borrar un arbol */
void BorrarArbol(tipoNodo *n)
{
   if(n->cero) BorrarArbol(n->cero);
   if(n->uno)  BorrarArbol(n->uno);
   free(n);
}

