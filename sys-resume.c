#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 1024

//FUNCIONES PROTOTIPO
void cpuInfo(char *lineacpu, char *modelo, char *velocidad);
double memInfo(char *lineaMem);
void discoInfo(char *lineaDisco, double *disco, double *disUsado);

//FUNCION PRINCIPAL
int main() {
    // Llamadas al sistema
    system("cat /proc/cpuinfo > cpu.txt");
    system("cat /proc/meminfo > mem.txt");
    system("df -h > fich.txt");

    //DECLARACIONES
    char cModelName[MAX] = "model name";
    char cMemTotal[MAX] = "MemTotal";
    char cMemAvailable[MAX] = "Active:";
    char cDevSDA[MAX] = "/dev/sda";

    int iModelName = strlen(cModelName);
    int iMemTotal = strlen(cMemTotal);
    int iMemAvailable = strlen(cMemAvailable);
    int iDevSDA = strlen(cDevSDA);

    char linea[MAX];
    char lineacpu[MAX];
    char lineamem1[MAX];
    char lineamem2[MAX];
    char lineadisco[MAX];

    char modelo[MAX];
    char velocidad[MAX];
    double memoria;
    double memUsada;
    double disco;
    double disUsado;


    FILE *cpuinfo, *meminfo, *sficheros;

    //1.0 COMPRUEBA DISPONIBLIDAD DE CPUINFO
    cpuinfo = fopen("cpu.txt", "r");
    if (cpuinfo == NULL) {
        fprintf(stderr, "No se puede abrir cpuinfo.txt!\n");
        exit(1);
    }

    //1.1 EXTRAE LINEACPU DE CPUINFO
    int i=0;
    while (fgets(linea,MAX,cpuinfo) != NULL) {
        if(strncmp(linea,cModelName,iModelName)==0) {strcpy(lineacpu, linea);}
        i++;
    }
    fclose(cpuinfo);

    //1.2 EXTRAE DATOS DE LINEACPU
    cpuInfo(lineacpu, modelo, velocidad);

    //2.0 COMPRUEBA DISPONIBLIDAD DE MEMINFO
    meminfo = fopen("mem.txt", "r");
    if (meminfo == NULL) {
        fprintf(stderr, "No se puede abrir meminfo.txt!\n");
        exit(1);
    }

    //2.1 EXTRAE LINEAMEMINFO1 Y LINEAMEMINFO2 DE MEMINFO
    i=0;
    while (fgets(linea,MAX,meminfo) != NULL) {
        if(strncmp(linea,cMemTotal,iMemTotal)==0) {strcpy(lineamem1, linea);}
        if(strncmp(linea,cMemAvailable,iMemAvailable)==0) {strcpy(lineamem2, linea);}
        i++;
    }
    fclose(meminfo);

    //2.2 EXTRAE DATOS DE LINEAMEMINFO1 Y LINEAMEMINFO2
    memoria = memInfo(lineamem1);
    memUsada = memInfo(lineamem2);
    memUsada = memUsada*100/memoria;

    //3.0 COMPRUEBA DISPONIBLIDAD DE SFICHEROS
    sficheros = fopen("fich.txt", "r");
    if (sficheros == NULL) {
        fprintf(stderr, "No se puede abrir sficheros.txt!\n");
        exit(1);
    }

    //3.1 EXTRAE LINEADISCO DE SFICHEROS
    i=0;
    int j=0;
    while (fgets(linea,MAX,sficheros) != NULL && j!=1) {
        if(strncmp(linea,cDevSDA,iDevSDA)==0){
            strcpy(lineadisco, linea);
            j=1;
        }
        i++;
    }
    fclose(sficheros);

    //3.2 EXTRAE DATOS DE LINEADISCO
    discoInfo (lineadisco, &disco, &disUsado);
    disUsado = disUsado*100/disco;

    //4.0 SALIDA FINAL DE LOS DATOS
    printf("INFORMACION DEL SISTEMA\n");
    printf("CPU:\t  %s ",modelo); printf("%s\n",velocidad);
    printf("Memoria:  %.2f GB ",memoria); printf("\t\t\t   %.2f\n",memUsada);
    printf("Disco:\t  %.2f GB ",disco); printf("\t\t\t   %.2f\n",disUsado);
    printf("-----------------------------------------------------------\n");

    return 0;
}

void cpuInfo (char *lineacpu, char *modelo, char *velocidad){
    int tLinea = strlen(lineacpu);
    int pF = tLinea-1; //posicion final de la linea
    int pM, pV; //posiciones donde inicia el nombre del modelo y de la velocidad
    char temp;
    int i,j;

    i=0;
    while(i < tLinea){
        temp = lineacpu[i];
        if(temp == ':') {pM = i;}
        if(temp == '@') {pV = i;}
        i++;
    }

    i=pM+2;
    j=0;
    while(i<pV){
        modelo[j] = lineacpu[i];
        i++;
        j++;
    }

    i=pV+2;
    j=0;
    while(i<pF){
        velocidad[j] = lineacpu[i];
        i++;
        j++;
    }
}

double memInfo(char *lineaMem){
    int tLinea = strlen(lineaMem);
    char cMemoria[MAX] = " ";
    int pI, pF; //posicion inicial y final del numero a obtener
    int i,j;

    i=0;
    j=0;
    pF=0;
    while(i < tLinea){
        if(isdigit(lineaMem[i])){
            cMemoria[j] = lineaMem[i];
            pF++;
            //if(pF==1){pI=pF;}
            j++;
        }
        i++;
    }

    char *ptr;
    double dMemoria;

    dMemoria = strtol(cMemoria, &ptr, 10);
    dMemoria = dMemoria/1000000;
    return dMemoria;
}

void discoInfo(char *lineaDisco, double *disco, double *disUsado){
    int tLinea = strlen(lineaDisco);
    char num1[MAX] = " ";
    char num2[MAX] = " ";
    int i,j,h;
    char *ptr;
    double dDisco, dDisUsado;

    i=0;
    j=0;
    h=0;
    while(i < tLinea && h!=1){
        if((isdigit(lineaDisco[i]) && isdigit(lineaDisco[i+1]))||(isdigit(lineaDisco[i]) && lineaDisco[i+1] == ',')||(lineaDisco[i]==',' && isdigit(lineaDisco[i+1]) )){
            num1[j] = lineaDisco[i];
            num1[j+1] = lineaDisco[i+1];

            j++;
        }
        else{
            if(j>0){h=1;}
        }
        i++;
    }

    dDisco = strtol(num1, &ptr, 10);
    *disco = dDisco;


    j=0;
    h=0;
    while(i < tLinea && h!=1){
        if((isdigit(lineaDisco[i]) && isdigit(lineaDisco[i+1]))||(isdigit(lineaDisco[i]) && lineaDisco[i+1] == ',')||(lineaDisco[i]==',' && isdigit(lineaDisco[i+1]) )){
            num2[j] = lineaDisco[i];
            num2[j+1] = lineaDisco[i+1];
            j++;
        }
        else{
            if(j>0){h=1;}
        }
        i++;
    }

    dDisUsado = strtol(num2, &ptr, 10);
    *disUsado = dDisUsado;

}

