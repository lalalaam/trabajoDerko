#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define maxAutos 100

struct cliente{
    char *nombre;
    int edad;
    char *rut;
    int id;
    struct autos **coleccion;
};
struct nodoClientes{
    struct cliente *datosCliente;
    struct nodoClientes *ant,*sig;
};

struct autos{
    char *marca;
    char *modelo;
    int stockAuto;
    int valor;
};
struct nodoAutos{
    int vendidos;
    struct autos *datosAuto;
    struct nodoAutos *ant,*sig;
};

struct Marca
{
    char *nombre;
    struct nodoAutos *autos;
    struct Marca *sig;
};

struct almacen{
    struct Marca *autosEnStock;
};

struct planta
{
    int idPlanta;
    int ventasMensuales;
    struct nodoClientes *clientesFrecuentes;
    struct Marca *autos;
};
struct plantasDistribucion{
    struct planta *datosArbol;
    struct plantasDistribucion *izq,*der;
};

struct Comunas
{
    char *Nombre;
    struct plantasDistribucion *Sucursales;
};
struct listaComunas{
    struct Comunas *datosComuna;
    struct listaComunas *ant, *sig;
};
struct nodoRegiones
{
    char *nombreRegion;
    struct listaComunas *comunas;
    struct nodoRegiones *sig;

};

// Struct General de la distribuidora
struct DercoDistribuidora{
    struct nodoRegiones *regiones;
    struct almacen *stockGeneral;
};

struct planta *crearPlanta(){
    struct planta *retorno;
    retorno= (struct planta *)malloc(sizeof(struct planta));
    printf("Id de la Planta : ");
    scanf("%d",&(retorno->idPlanta));
    printf("Ventas Mensual(hasta la fecha): ");
    scanf("%d",&(retorno->ventasMensuales));
    retorno->autos=NULL;
    retorno->clientesFrecuentes=NULL;

    return retorno;
}
struct autos *crearAuto(){
    struct autos *nuevo;
    char buffer[100];
    nuevo=(struct autos *)malloc(sizeof(struct autos));

    getchar();
    printf("Ingrese Marca : ");
    scanf("%100[^\n]s",buffer);
    getchar();
    nuevo->marca=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(nuevo->marca,buffer);

    printf("\nIngrese Modelo : ");
    scanf("%100[^\n]s",buffer);
    getchar();
    nuevo->modelo=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(nuevo->modelo,buffer);

    int valor;
    printf("\nIngrese valor : ");
    scanf("%d",&valor);
    getchar();
    nuevo->valor= valor;
    nuevo->stockAuto=0;

    return nuevo;
}
struct cliente *crearCliente(){
    struct cliente *nuevo;
    char buffer[100];
    int data;
    getchar();
    nuevo=(struct cliente*)malloc(sizeof(struct cliente));
    printf("\nIngrese Los datos del Cliente \n");
    printf("-------------------------------\n");

    printf("Nombre : ");
    scanf("%100[^\n]s",buffer);
    nuevo->nombre=(char *)malloc(sizeof(char)*(strlen(buffer)+1));
    strcpy(nuevo->nombre,buffer);
    getchar();
    printf("\nRut : ");

    scanf("%100[^\n]s",buffer);
    nuevo->rut=(char *)malloc(sizeof (char)*(strlen(buffer)+1));
    strcpy(nuevo->rut,buffer);
    getchar();

    printf("\nEdad :");
    scanf("%d",&data);
    nuevo->edad=data;

    printf("\nId : ");
    scanf("%d",&data);
    nuevo->id=data;

    nuevo->coleccion=NULL;


    return nuevo;
}

void editarAutos(struct nodoAutos **autos, int precioNuevo, char modelo[100]){
    if(autos!=NULL){
        editarAutos(&((*autos)->sig), precioNuevo, modelo);
        if(strcmp(modelo, (*autos)->datosAuto->modelo)==0){
            (*autos)->datosAuto->valor = precioNuevo;
            printf("Valor de auto cambiado exitosamente.\n");
        }
    }
}
void editarMarcas(struct Marca **marcas, int precioNuevo, char modelo[100])
{
    if(marcas!=NULL){
        editarMarcas(&((*marcas)->sig), precioNuevo, modelo);
        editarAutos(&((*marcas)->autos), precioNuevo, modelo);
    }
}
void editarSucursales(struct plantasDistribucion **sucursales, int nuevoPrecio, char modelo[100])
{
    if(sucursales!=NULL){
        editarSucursales(&((*sucursales)->izq), nuevoPrecio, modelo);
        editarSucursales(&((*sucursales)->der), nuevoPrecio, modelo);
        editarMarcas(&((*sucursales)->datosArbol->autos),nuevoPrecio,modelo);
    }
}
void editarComunas(struct listaComunas **comunas, int nuevoPrecio, char modelo[100])
{
    if(comunas!=NULL){
        editarComunas(&((*comunas)->sig), nuevoPrecio, modelo);
        editarSucursales(&((*comunas)->datosComuna->Sucursales), nuevoPrecio, modelo);
    }
}
void editarRegiones(struct nodoRegiones **regiones, int nuevoPrecio, char modelo[100])
{
    if(regiones!=NULL){
        editarRegiones(&((*regiones)->sig), nuevoPrecio, modelo);
        editarComunas(&((*regiones)->comunas), nuevoPrecio, modelo);
    }
}
void cambiarValorAutoSucursales(struct DercoDistribuidora **derco, int nuevoPrecio, char modelo[100])
{
    if((*derco)->regiones!=NULL){
        struct nodoRegiones *recRegiones = (*derco)->regiones;
        editarRegiones(&(recRegiones), nuevoPrecio, modelo);
    }
}
void cambiarValorAuto(int nuevoPrecio, char modelo[100],struct DercoDistribuidora **derco){
    if((*derco)->stockGeneral->autosEnStock->autos!=NULL){

        struct Marca *recMarcas = (*derco)->stockGeneral->autosEnStock;
        editarMarcas(&recMarcas, nuevoPrecio, modelo);

        cambiarValorAutoSucursales(&(*derco), nuevoPrecio, modelo);
    }
}


void mostrarClientes(struct nodoClientes *head){
    struct nodoClientes *rec;
    if(head!=NULL){
        rec=head;
        do{
            puts(rec->datosCliente->nombre);
            rec=rec->sig;
        }while(rec!=head);
    }
}
void mostrarClientesPlantas(struct plantasDistribucion *head){
    if(head!=NULL){
        printf("\n%d",head->datosArbol->idPlanta);
        mostrarClientes(head->datosArbol->clientesFrecuentes);
        mostrarClientesPlantas(head->izq);
        mostrarClientesPlantas(head->der);
        return;
    }
}
void mostrarClientesComunas(struct listaComunas *head){
    struct listaComunas *rec=head;
    while(rec!=NULL && rec->datosComuna!=NULL){
        puts(rec->datosComuna->Nombre);
        mostrarClientesPlantas(rec->datosComuna->Sucursales);
        rec=rec->sig;
    }
}
void mostrarClientesRegiones(struct nodoRegiones *regiones){
    struct nodoRegiones *rec;
    char buffer[100];
    getchar();
    printf("Por privacidad de nuestros clientes ,ingrese contraseña para acceder a esta informacion : ");
    scanf("%100[^\n]s",buffer);
    getchar();
    if(strcmp(buffer,"derco833")==0){
        if(regiones!=NULL && regiones->comunas!=NULL){
            rec=regiones;
            while(rec!=NULL){
                printf("\n");
                puts(regiones->nombreRegion);
                printf("\n---------------------------------\n");
                mostrarClientesComunas(rec->comunas);
                rec=rec->sig;
            }
        }
    }
    else{
        printf("ACCESO DENEGADO");
    }

}


void mostrarAutos(struct nodoAutos *head){
    struct nodoAutos *rec;
    int cont=1;
    if(head!=NULL){
        rec=head;
        while(rec!=NULL){
            printf("\nAuto %d : \n",cont);
            printf("- ");
            puts(rec->datosAuto->modelo);
            printf("Valor : %d",rec->datosAuto->valor);
            printf("\nDisponibilidad de articulo : %d\n",rec->datosAuto->stockAuto);
            cont++;
            rec=rec->sig;
        }
    }
}
void mostrarMarcas(struct almacen *stockGeneral){
    struct Marca *rec;
    if(stockGeneral!=NULL && stockGeneral->autosEnStock!=NULL){
        rec=stockGeneral->autosEnStock;
        printf("Autos Disponibles : \n");
        while(rec!=NULL){
            puts(rec->autos->datosAuto->marca);
            printf("-------------------------------");
            mostrarAutos(rec->autos);
            rec=rec->sig;
        }

    }

}


void listarComunas(struct listaComunas *head){
    struct listaComunas *rec;
    rec=head;
    while(rec!=NULL){
        printf("\n");
        printf("- ");
        puts(rec->datosComuna->Nombre);
        rec=rec->sig;
    }
}
void listarRegiones(struct nodoRegiones *head){
    struct nodoRegiones *rec;
    printf("\nComunas en las cuales nos puedes encontrar");
    printf("\n-------------------------------------------");
    if(head!=NULL ){
        rec=head;
        while(rec!=NULL){
            printf("\nRegion : ");
            puts(rec->nombreRegion);
            if(rec->comunas!=NULL){
                listarComunas(rec->comunas);
            }
            rec=rec->sig;
        }
    }
}



int buscarPlanta(struct plantasDistribucion *head, int id){
    if(head->datosArbol->idPlanta==id) return 1;
    else{
        if(head->datosArbol->idPlanta<id) return buscarPlanta(head->izq,id);
        else return buscarPlanta(head->der,id);
    }
}
int agregarPlanta(struct plantasDistribucion **head,struct planta *nuevo){
    if((*head)==NULL){
        (*head)=(struct plantasDistribucion *)malloc(sizeof(struct plantasDistribucion));
        (*head)->datosArbol=(struct planta *)malloc(sizeof(struct planta));
        (*head)->datosArbol=nuevo;
        return 1;
    }
    else{
        if((*head)->datosArbol->idPlanta<nuevo->idPlanta) return agregarPlanta(&(*head)->izq,nuevo);
        else return agregarPlanta(&(*head)->der,nuevo);
    }
}
int agregarPlantaComuna(struct listaComunas **head,char comuna[100]){
    struct listaComunas *rec;
    if((*head)!=NULL){
        rec=(*head);
        while(rec!=NULL && rec->datosComuna->Sucursales!=NULL){
            if(strcmp(rec->datosComuna->Nombre,comuna)==0){
                struct planta *nuevo= crearPlanta();
                if(buscarPlanta((*head)->datosComuna->Sucursales,nuevo->idPlanta)==0){
                    if(agregarPlanta(&(rec->datosComuna->Sucursales),nuevo)==1){
                        return 1;
                    }
                }
            }
            rec=rec->sig;
        }
    }
    return 0;
}
void agregarPlantaRegion(struct nodoRegiones **head){
    struct nodoRegiones *rec;
    char comuna[100];
    printf("Comuna En la cual se encontrara la nueva sucursal : ");
    scanf("%100[^\n]s",comuna);
    if((*head)!=NULL){
        rec=(*head);
        while(rec!=NULL){
            if(agregarPlantaComuna(&(rec->comunas),comuna)==1){
                printf("SE AGREGO EXITOSAMENTE");
                break;
            }
            rec=rec->sig;
        }
    }
    printf("NO SE PUDO AGREGAR");
}


void sucursalesEnComuna(struct plantasDistribucion *sucursal, int *cont)
{
    if(sucursal!=NULL){
        sucursalesEnComuna(sucursal->izq, cont);
        sucursalesEnComuna(sucursal->der, cont);
        (*cont)+=1;
    }
}
int cantidadSucursalesEnRegion(struct listaComunas *comunas)
{
    int sucursales=0, cont=0;

    while(comunas!=NULL){
        sucursalesEnComuna(comunas->datosComuna->Sucursales, &cont);
        sucursales+=cont;
        comunas=comunas->sig;
    }
    return sucursales;
}
struct nodoRegiones *buscarRegionMayor(struct nodoRegiones *regiones, int *sucursales)
{
    struct nodoRegiones *regionMayor;
    int mayor, cont;

    while(regiones!=NULL){
        cont = cantidadSucursalesEnRegion(regiones->comunas);
        if(cont>mayor) {
            regionMayor = regiones;
            mayor = cont;
        }
        regiones=regiones->sig;
    }
    *sucursales=mayor;
    return regionMayor;
}
void regionConMasSucursales(struct nodoRegiones *regiones)
{
    if(regiones!=NULL&&regiones->comunas!=NULL){
        int cantSucursales;
        struct nodoRegiones *regionMayor;

        regionMayor = buscarRegionMayor(regiones, &cantSucursales);
        printf("La regiÃ³n con mas sucursales es ");
        puts(regionMayor->nombreRegion);
        printf("\n con un total de %d plantas.", cantSucursales);
    }
}

int contarAutos(struct cliente* cliente) {
    int cont=0;
    int i;

    for (i = 0; i < maxAutos; i++) {
        if (cliente->coleccion[i] != NULL) {
            cont++;
        }
    }
    return cont;
}
struct cliente* clienteMasAutos(struct nodoClientes* clientes) {

    struct nodoClientes* rec = clientes;
    struct cliente* masAutos = rec->datosCliente;

    while (rec != NULL) {
        if (contarAutos(rec->datosCliente) > contarAutos(masAutos)) {
            masAutos = rec->datosCliente;
        }
        rec = rec->sig;
    }

    return masAutos;
}
void buscarClienteMasAutosSucursal(struct plantasDistribucion *abbSucursales, struct cliente **cliente)
{
    if(abbSucursales!=NULL){
        struct cliente *clienteLocal = clienteMasAutos(abbSucursales->datosArbol->clientesFrecuentes);

        if(contarAutos(clienteLocal) > contarAutos((*cliente)))
            (*cliente) = clienteLocal;

        buscarClienteMasAutosSucursal(abbSucursales->izq, &(*cliente));
        buscarClienteMasAutosSucursal(abbSucursales->der, &(*cliente));
    }
}
void clienteConMasAutos(struct nodoRegiones *regiones){

    struct nodoRegiones *recRegiones = regiones;

    while(recRegiones!=NULL){
        puts(recRegiones->nombreRegion); printf(":\n");
        struct listaComunas *recComunas = recRegiones->comunas;

        while(recComunas!=NULL){
            printf("Comuna "); puts(recComunas->datosComuna->Nombre); printf(":\n");
            struct cliente *clienteMayor;

            buscarClienteMasAutosSucursal(recComunas->datosComuna->Sucursales, &clienteMayor);
            if(clienteMayor==NULL)
                printf("NO SE ENCONTRO CLIENTE.");
            else
                puts(clienteMayor->nombre); printf("%d auto(s).\n", contarAutos(clienteMayor));
            recComunas=recComunas->sig;
        }
        recRegiones=recRegiones->sig;
    }
}



int buscarAuto(struct nodoAutos *head, char *modelo){
    struct nodoAutos *rec= head;
    while(rec!=NULL){
        if(strcmp(rec->datosAuto->modelo,modelo)==0)return 1;
    }
    return 0;
}
int agregarAutoMarca(struct nodoAutos **headAutos,struct autos *nuevo){
    if(buscarAuto((*headAutos),nuevo->modelo)==0){
        if((*headAutos)==NULL){
            (*headAutos)=(struct nodoAutos *)malloc(sizeof(struct nodoAutos));
            (*headAutos)->datosAuto=nuevo;
            printf("\nCantidad De ventas de este modelo(Hasta la fecha) : ");
            scanf("%d",&((*headAutos)->vendidos));
            (*headAutos)->sig=(*headAutos)->ant=NULL;
            return 1;
        }
        else{
            struct nodoAutos *rec =(*headAutos);
            while(rec->sig!=NULL){
                rec=rec->sig;
            }
            rec->sig=(struct nodoAutos *)malloc(sizeof(struct nodoAutos));
            rec->sig->datosAuto=nuevo;
            printf("\nCantidad De ventas de este modelo(Hasta la fecha) : ");
            scanf("%d",&(rec->sig->vendidos));
            rec->sig->ant=rec;
            rec->sig->sig=NULL;
            return 1;
        }
    }
    else{
        return 0;
    }
}
void agregarAutoAlmacen(struct almacen **autos){
    struct Marca *rec;
    struct autos *nuevo= crearAuto();
    if((*autos)!=NULL && (*autos)->autosEnStock!=NULL){
        rec= (*autos)->autosEnStock;
        while(rec!=NULL){
            if(strcmp(rec->nombre,nuevo->marca)==0){
                if(agregarAutoMarca(&(rec->autos),nuevo)==1){
                    printf("\n Auto Agregado Correctamente");
                }
                else break;
            }
            rec=rec->sig;
        }
    }
    printf("No se pudo agregar");

}

int eliminarNodoAuto(struct nodoAutos **head,struct autos *eliminado){
    struct nodoAutos *rec;
    if((*head)!=NULL){
        rec=(*head);
        while(rec!=NULL){
            if(strcmp(rec->datosAuto->modelo,eliminado->modelo)==0){
                rec->ant->sig=rec->sig;
                rec->sig->ant=rec->ant;
                return 1;
            }
            rec=rec->sig;
        }
    }
    return 0;
}
void eliminarAuto(struct almacen **autos){
    struct autos *elminado = crearAuto();
    struct Marca *rec;
    if((*autos)!=NULL && (*autos)->autosEnStock!=NULL){
        rec= (*autos)->autosEnStock;
        while(rec!=NULL){
            if(strcmp(rec->nombre,elminado->marca)==0){
                if(eliminarNodoAuto(&(rec->autos),elminado)==1){
                    printf("Eliminado Correctamente");
                }
                else break;
            }
        }
    }
    else{
        printf("ERROR!! no hay elementos para eliminar");
    }
    printf("No se pudo Eliminar");
}

int agregarCliente(struct nodoClientes **headClientes){
    struct nodoClientes *rec;
    struct cliente *nuevo =crearCliente();
    if((*headClientes)!=NULL){
        rec=(*headClientes);
        do{
            rec=rec->sig;
        }while(rec->sig!=(*headClientes));
        rec->sig->datosCliente=nuevo;
        rec->sig->sig=(*headClientes);
        (*headClientes)->ant=rec->sig;
        rec->sig->ant=rec;
        return 1;
    }
    else return 0;
}
int agregarClientePlanta(struct plantasDistribucion **head,int id){
    if((*head)!=NULL){
        if((*head)->datosArbol->idPlanta==id){
            if(agregarCliente(&((*head)->datosArbol->clientesFrecuentes))==1)return 1;

        }
        if((*head)->datosArbol->idPlanta<id){
            return agregarClientePlanta(&((*head)->izq),id);
        }
        else  return agregarClientePlanta(&((*head)->der),id);
    }
    return 0;
}
int agregarClienteComuna(struct listaComunas **head){
    struct listaComunas *rec;
    char buffer[100];
    int id;
    printf("\nEn que Comuna se encuentra : ");
    scanf("%100[^\n]s",buffer);
    printf("\nId de la planta : ");
    scanf("%d",&id);

    if((*head)!=NULL){
        rec=(*head);
        while(rec!=NULL){
            if(rec->datosComuna!=NULL && strcmp(rec->datosComuna->Nombre,buffer)==0){
                if(agregarClientePlanta(&(rec->datosComuna->Sucursales),id)==1){
                    return 1;
                }
                else break;
            }
        }
    }
    return 0;
}
void agregarClienteRegion(struct nodoRegiones **headRegiones){
    struct nodoRegiones *rec;
    char buffer[100];
    printf("En que Region se ubica la sucursal : ");
    scanf("%100[^\n]s",&buffer);
    if((*headRegiones)!=NULL && (*headRegiones)->comunas!=NULL){
        rec= (*headRegiones);
        while(rec!=NULL){
            if(strcmp(buffer,rec->nombreRegion)==0){
                if(agregarClienteComuna(&(rec->comunas))==1){
                    printf("Se agrego Correctamente");
                }
                else break;
            }
        }
    }
    printf("No se pudo agregar");

}


int eliminarCliente(struct nodoClientes **headClientes,struct cliente *eliminado){
    struct nodoClientes *rec;
    if((*headClientes)!=NULL){
        rec=(*headClientes);
        if((*headClientes)->datosCliente->id==eliminado->id){
            (*headClientes)->sig->ant=(*headClientes)->ant;
            (*headClientes)->ant->sig=(*headClientes)->sig;
            (*headClientes)=(*headClientes)->sig;
            return 1;
        }
        do{
            if(rec->datosCliente->id==eliminado->id){
                rec->ant->sig=rec->sig;
                rec->sig->ant=rec->ant;
                return 1;
            }
            rec=rec->sig;
        }while(rec!=(*headClientes));
        return 0;

    }
    else return 0;
}
int eliminarClienteSucursal(struct plantasDistribucion **headAbb,int idBuscado,struct cliente *eliminado){
    if((*headAbb)!=NULL){
        if((*headAbb)->datosArbol->idPlanta==idBuscado){
            if(eliminarCliente(&((*headAbb)->datosArbol->clientesFrecuentes),eliminado)==1)return 1;
            else return 0;
        }
        if((*headAbb)->datosArbol->idPlanta<idBuscado) return eliminarClienteSucursal(&((*headAbb)->izq),idBuscado,eliminado);
        else return eliminarClienteSucursal(&((*headAbb)->der),idBuscado,eliminado);

    }
    else return 0;
}
int eliminarClienteComuna(struct listaComunas **headComunas){
    struct listaComunas *rec=(*headComunas);
    char comuna[100];
    int idBuscado;
    printf("\nEn que comuna Se encuentra : ");
    scanf("%100[^\n]s",comuna);
    printf("\nId de la planta en la que se encuentra :");
    scanf("%d",&idBuscado);
    struct cliente *nuevo= crearCliente();

    while(rec!=NULL){
        if(rec->datosComuna!=NULL){
            if(strcmp(rec->datosComuna->Nombre,comuna)==0){
                if(eliminarClienteSucursal(&(rec->datosComuna->Sucursales),idBuscado,nuevo)==1)return 1;
                else break;
            }
        }
        rec=rec->sig;
    }
    return 0;
}
void eliminarClienteRegion(struct nodoRegiones **headRegiones){
    char region[100];
    struct nodoRegiones *rec;
    if((*headRegiones)!=NULL){
        printf("\nEn que region se encuentra el cliente : ");
        scanf("%100[^\n]s",region);
        rec=(*headRegiones);
        while(rec!=NULL){
            if(strcmp(rec->nombreRegion,region)==0){
                if(rec->comunas!=NULL){
                    if(eliminarClienteComuna(&(rec->comunas))==1) printf("\nEliminado Exitosamente");
                }
            }
            rec=rec->sig;
        }
        printf("\nNo se pudo eliminar");
    }
    else{
        printf("No existen elementos para eliminar");
    }
}


void borrarSucursal(struct plantasDistribucion **arbol, struct planta *sucursalBuscada) {

    struct plantasDistribucion *aux = *arbol;

    if (!(*arbol))
        return;

    if ((*arbol)->datosArbol == sucursalBuscada) {
        *arbol = (*arbol)->der;
        return;
    }
    borrarSucursal(&(*arbol)->izq, sucursalBuscada);
    borrarSucursal(&(*arbol)->der, sucursalBuscada);
}

void eliminarSucursal(struct nodoRegiones **headRegiones) {

    struct planta *aEliminar = crearPlanta();
    struct listaComunas *comuna = (*headRegiones)->comunas;

    while (*headRegiones != NULL) {
        while (comuna != NULL) {
            borrarSucursal(&(comuna->datosComuna->Sucursales), aEliminar);
            comuna = comuna->sig;
        }
        *headRegiones = (*headRegiones)->sig;
    }
}

void editarNuevoValorPlantas(struct plantasDistribucion *plantas, int nuevoValor, int idPlanta, int *esta)
{
    if(plantas!=NULL) {
        editarNuevoValorPlantas(plantas->izq, nuevoValor, idPlanta, esta);
        editarNuevoValorPlantas(plantas->der, nuevoValor, idPlanta, esta);

        if (plantas->datosArbol->idPlanta == idPlanta){
            plantas->datosArbol->ventasMensuales = nuevoValor;
            printf("Precio actualizado correctamente.");
            (*esta)=0;
            return;
        }
    }
}
void cambiarVentasDePlanta(struct DercoDistribuidora **derco, int nuevoValor, int idPlanta)
{
    struct nodoRegiones *recRegiones = (*derco)->regiones;
    int esta=1;
    while(recRegiones!=NULL){
        struct listaComunas *recComunas = recRegiones->comunas;

        while(recComunas!=NULL){
            editarNuevoValorPlantas(recComunas->datosComuna->Sucursales, nuevoValor, idPlanta, &esta);
            recComunas=recComunas->sig;
        }
        recRegiones=recRegiones->sig;
    }
    if(esta==1)
        printf("Sucursal no encontrada.");
}

int ingresarVehiculo(struct planta **head, struct autos *nuevoAuto, struct cliente *clienteBuscado) {

    struct nodoClientes *rec = (*head)->clientesFrecuentes;
    int i;

    while (rec != NULL) {
        if (rec->datosCliente->id == clienteBuscado->id) {

            for (i = 0; i < maxAutos; i++) {
                if (rec->datosCliente->coleccion[i] == NULL) {
                    rec->datosCliente->coleccion[i] = nuevoAuto;
                    return 1;
                }
            }
        }
        rec = rec->sig;
    }
    return 0;
}


struct autos *menorVentas(struct Marca *head){

    struct nodoAutos *menosVendido = head->autos;

    while(head != NULL){
        while(head->autos != NULL){
            if((head->autos->vendidos) < menosVendido->vendidos)
                menosVendido = head->autos;

            head->autos = head->autos->sig;
        }
        head = head->sig;
    }
    return menosVendido->datosAuto;
}
struct autos *mayorVentas(struct Marca *head){

    struct nodoAutos *masVendido = head->autos;

    while(head != NULL){
        while(head->autos != NULL){
            if((head->autos->vendidos) > masVendido->vendidos)
                masVendido = head->autos;

            head->autos = head->autos->sig;
        }
        head = head->sig;
    }
    return masVendido->datosAuto;
}
int agregarVehiculoArr(struct autos ** coleccion)
{
  if(coleccion != NULL)
  {
    int i;
    struct autos *nuevoAuto = crearAuto();
    for(i = 0; i<maxAutos;i++)
    {
      if(coleccion[i] == NULL)
      {
        coleccion[i] = nuevoAuto;
      }
    }
  }
  return 0;
}
int agregarVehiculo(struct nodoClientes **headClientes){
    struct nodoClientes *rec;
    int idBusc;
    if((*headClientes)!=NULL){
      printf("Escriba la id del cliente que quiere agregar un vehiculo");
      scanf("%i",&idBusc);
      do
      {
          if(rec->datosCliente->id == idBusc)
          {
            if(agregarVehiculoArr(rec->datosCliente->coleccion) == 1)
            {
              return 1;
            }
          }
          rec = rec->sig;
      }while(rec!=(*headClientes));
    }
    else return 0;
}
int agregarVehiculoCliente(struct plantasDistribucion **head,int id){
    if((*head)!=NULL){
        if((*head)->datosArbol->idPlanta==id){
            if(agregarVehiculo(&((*head)->datosArbol->clientesFrecuentes))==1)return 1;

        }
        if((*head)->datosArbol->idPlanta<id){
            return agregarVehiculoCliente(&((*head)->izq),id);
        }
        else  return agregarVehiculoCliente(&((*head)->der),id);
    }
    return 0;
}
int agregarVehiculoBuscandoComuna(struct listaComunas *comunas ,char *comunaBusc)
{
  int id;
  if(comunas !=  NULL)
  {
    struct listaComunas *rec = comunas;
    while(rec != NULL)
    {
      if(strcmp(rec->datosComuna->Nombre,comunaBusc) == 0)
      {
        printf("Ingresar el ID del cliente");
        scanf("%d", &id);
        if((agregarVehiculoCliente(&(rec->datosComuna->Sucursales),id) == 1))
        {
          return 1;
        }
        else{
          return 0;
        }
      }
      rec = rec->sig;
    }
    return 0;
  }
  return 0;
}

int agregarVehiculoColec(struct nodoRegiones *regiones, char *buscado)
{
  if(regiones != NULL)
  {
    struct nodoRegiones *rec = regiones;
    while(rec!=NULL)
      {
        if(strcmp(rec->nombreRegion,buscado) == 0)
        {
          char *comunaBusc;
          printf("Region encontrada, porfavor escriba la comuna del cliente\n");
          scanf("%100[^\n]s",comunaBusc);
          if(agregarVehiculoBuscandoComuna(rec->comunas,comunaBusc) == 1)
          {
            return 1;
          }
          return 0;
        }
        rec = rec->sig;
      }
  }
  return 0;
}

void buscarVentasAbb(struct plantasDistribucion **abb, int *mayor)
{
  if((*abb) != NULL)
  {
    if((*abb)->datosArbol->ventasMensuales)
    {
      mayor += (*abb)->datosArbol->ventasMensuales;
    }
    buscarVentasAbb(&(*abb)->izq,&(*mayor));
    buscarVentasAbb(&(*abb)->der,&(*mayor));
  }
  return;
}

struct Comunas buscarVentasComuna(struct listaComunas *comunas,int *mayor)
{
  struct Comunas *comunaMayor = NULL;
  struct listaComunas *rec;
  int mayorLocal = 0;
  if(comunas !=  NULL)
  {
    rec = comunas;
    while(rec!=NULL)
      {
        buscarVentasAbb(&(rec->datosComuna->Sucursales),&(*mayor));
        if(mayorLocal < mayor)
        {
          comunaMayor = rec->datosComuna;
          mayorLocal = (*mayor);
        }
        mayor = 0;
        rec = rec->sig;
      }
    (*mayor) = mayorLocal;
    return (*comunaMayor);
  }
  return (*comunaMayor);
}


struct Comunas comunaConMasVentas(struct nodoRegiones *regiones)
{
  int mayor = 0;
  struct Comunas *mayorComunaVentas = NULL;
  struct nodoRegiones *rec;
  if(regiones !=  NULL)
  {
    while(rec != NULL)
      {
        (*mayorComunaVentas) = buscarVentasComuna(regiones->comunas,&mayor);
        rec = rec->sig;
      }
    
    return (*mayorComunaVentas);
  }
  return (*mayorComunaVentas);
}

// Menú ------------------------------------------

void menuAgregar(struct DercoDistribuidora *derco){
    int opcion;
    printf("\nQue deseas agregar");
    printf("\n---------------------");
    printf("\n1. Auto\n2. Cliente\n3. Sucursal\n");
    scanf("%d",&opcion);
    switch (opcion) {
        case 1:
            agregarAutoAlmacen(&(derco->stockGeneral));///OK
            break;
        case 2:
            agregarClienteRegion(&(derco->regiones));///OK
            break;
        case 3:
            agregarPlantaRegion(&(derco->regiones));///OK
            break;
    }
}
void menuEliminar(struct DercoDistribuidora *derco){
    int opcion;
    printf("\nQue Deseas Elminar");
    printf("\n--------------------");
    printf("\n1. Auto de nuestro Almacen\n2. Cliente De una Sucursal\n3. Sucursal de una comuna");
    scanf("%d",&opcion);
    switch (opcion) {
        case 1:
            eliminarAuto(&(derco->stockGeneral));///OK
            break;
        case 2:
            eliminarClienteRegion(&(derco->regiones));///OK
            break;
        case 3:
            eliminarSucursal(&(derco->regiones));
            break;
    }
}

void menuCambiar(struct DercoDistribuidora *derco){
    int opcion;
    char *busc;
    char buffer[100];
    printf("\nQue deseas cambiar");
    printf("\n1. Valor de un Auto\n|2. Ingresar un auto a la coleccion de un cliente\n3. Cantidad de ventas de una planta\n");
    scanf("%d",&opcion);
    switch(opcion){
        case 1:
            printf("\nModelo del auto a cambiar : ");
            scanf("%100[^\n]s",&buffer);
            printf("\nNuevo valor a ingresar : ");
            int nuevoValor;
            scanf("%d",&nuevoValor);
            cambiarValorAuto(nuevoValor,buffer,&derco);
            break;
        case 2:
          printf("\nIngrese el nombre de la region del cliente");
          scanf("%100[^\n]s",busc);
          if(agregarVehiculoColec(derco->regiones,busc) == 1);
          {
            printf("Se pudo agregar el auto a la coleccion\n");
          }

            break;
        case 3:
            printf("\nQue planta requiere ser actualizada sus ventas : ");
            int idPlanta;
            scanf("%d",&idPlanta);
            printf("\nCuales Son las ventas actualizadas : ");
            int ventasActualizadas;
            scanf("%d",&ventasActualizadas);
            cambiarVentasDePlanta(&derco,ventasActualizadas,idPlanta);
            break;
    }
}
void menuModificar(struct DercoDistribuidora *derco){
    int opcion;
    printf("\nQue deseas hacer");
    printf("\n--------------------");

    printf("\n1. Agregar \n2. Eliminar\n3. Modificar un dato\n");
    scanf("%d",&opcion);

    switch(opcion){
        case 1:
            menuAgregar(derco);
            break;
        case 2:
            if (derco == NULL)
            {
                printf("No se puede eliminar ya que el sistema esta vacio.\n");
                break;
            }
            menuEliminar(derco);
            break;
        case 3:
            if (derco == NULL)
            {
                printf("No se puede cambiar ya que el sistema esta vacio.\n");
                break;
            }
            menuCambiar(derco);
            break;
    }
}
void menuListar(struct DercoDistribuidora *derco){
    int opcion;
    printf("\nQue deseas ver");
    printf("\n-----------------");
    printf("\n\n1. Autos Disponibles\n2. Clientes de una sucursal\n3. Comunas Activas\n");
    scanf("%d",&opcion);
    switch(opcion){
        case 1:
            mostrarMarcas(derco->stockGeneral);///OK
            break;
        case 2:
            mostrarClientesRegiones(derco->regiones);///OK
            break;
        case 3:
            listarRegiones(derco->regiones);///OK
            break;
    }
}
void menuOtros(struct DercoDistribuidora *derco){
    int opcion;
    struct autos *menosVendido;
    struct autos *masVendido;
    struct Comunas *comunaMayor;
    printf("\n1. Auto Mas Vendido\n4. Auto Menos Vendido\n3. Comuna Con mas sucursales\n4. Cliente Con Mas Autos\n5. La planta con mas ventas mensuales");
    scanf("%d",&opcion);
    switch (opcion) {
        case 1:
            masVendido= mayorVentas(derco->stockGeneral->autosEnStock);
            puts(masVendido->marca);
            puts(masVendido->marca);
            break;
        case 2:
            menosVendido= menorVentas(derco->stockGeneral->autosEnStock);
            puts(menosVendido->marca);
            puts(menosVendido->modelo);
            break;
        case 3:
            regionConMasSucursales(derco->regiones);
            break;
        case 4:
            clienteConMasAutos(derco->regiones);
            break;
        case 5:
            *comunaMayor = comunaConMasVentas(derco->regiones);
            if(comunaMayor!=NULL)
            {
              printf("La planta con mas ventas mensuales es: ");
              puts(comunaMayor->Nombre);
            }
            else{
              printf("No se pudo encontrar una planta\n");
            }
            
    }
}
void menu(struct DercoDistribuidora *derco){

    int opcion;
    printf("Hola bienvenido a Derco, ingrese un numero segun la accion que quiera realizar");
    printf("\n_______________________________________________________________________________");
    printf("\n1. Modificar \n2. Listar\n3. Otros\n");
    scanf("%d",&opcion);
    while(opcion > 3 || opcion < 1)
      {
        printf("Opcion invalida, porfavor escriba un numero denuevo");
        printf("\n1. Modificar \n2. Listar\n3. Otros\n");
        scanf("%d",&opcion);
      }

    switch(opcion){
        case 1:
            menuModificar(derco);
            break;
        case 2:
            if (derco == NULL)
            {
                printf("No se puede listar ya que el sistema esta vacio.\n");
                break;
            }
            menuListar(derco);
            break;
        case 3:
            if (derco == NULL)
            {
                printf("No se puede acceder otros ya que el sistema esta vacio.\n");
                break;
            }
            menuOtros(derco);
            break;
    }

}


//-------------------------------------------------------------------------------

int main(){
    struct DercoDistribuidora *derco=NULL;
    menu(derco);
}