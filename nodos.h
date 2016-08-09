#ifndef NODOS_H_
# define NODOS_H_

bool ver;
int contadorPasos=1;
int contador=1;
int cantidad;
int contadorClientes=1;
int contadorCarretas=1;
int contadorCajas=1;
char* prueba;
//Destinada a agrupar las estructuras de los nodos a utilizar

//Las carretas
typedef struct nodoCarreta {
//LOS APUNTADORES A NODOS
	struct  nodoCarreta *siguiente;
//LOS ATRIBUTOS
    int id;
} Carreta;

Carreta *inicioPilaCarretas1=NULL;
Carreta *inicioPilaCarretas2=NULL;
Carreta *inicioPilaCarretas3=NULL;
Carreta *inicioPilaCarretas4=NULL;


//Los clientes
typedef struct nodoCliente {
//LOS APUNTADORES A NODOS
	struct  nodoCliente *siguiente;
//LOS ATRIBUTOS
    int edad,carreta,pasosCompra;
    int numeroNombre;
    char genero;
    bool prioridad, estaEmbarazada;
} Cliente;

Cliente *inicioColaEsperaCarretas=NULL;
Cliente *auxiliarInicioColaEsperaCarretas=NULL;
Cliente *inicioColaPagos=NULL;
Cliente *inicioColaPagosOro=NULL;
Cliente *inicioListaCompras=NULL;

//Las Cajas
typedef struct nodoCaja {
//LOS APUNTADORES A NODOS
	struct  nodoCaja *siguiente;
	struct  nodoCaja *anterior;
//LOS ATRIBUTOS
    int numeroAtendidos, numeroCaja, cliente, numeroEspera, carreta;
    bool ocupada;
} Caja;

Caja *inicioListaCajas=NULL;

#endif
