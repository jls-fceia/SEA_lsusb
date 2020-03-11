//============================================================================
// Name        : SEA_lsusb.cpp
// Author      : JLS
// Version     :
// Copyright   : Voi qui entrare qui, lasciate ogni speranze
// Description : Acceso a dispositivos USB en linux
//============================================================================

#include <iostream>
#include </usr/include/libusb-1.0/libusb.h>

using namespace std;

void printdev(libusb_device *dev); 		//declaracion de la funcion

int main() {
	libusb_device **dispositivos;		//puntero a puntero de dispositivos (la lista de dispositivos)
	libusb_context *contexto = nullptr; //esta será la session
	int resultado; 						//para guardar los valores de retorno de funciones
	ssize_t cantidad; 					//numero de dispositivos conectados
	ssize_t i; 							//variable auxiliar

	//inicializo la librería y creo una session: devuelve 0 si todo va bien
	resultado = libusb_init( &contexto );
	if(resultado < 0) {
		cout << "Error al inicializar" << resultado << endl;
		return 1;
	}
	//esto setea el nivel de "verbosidad" de las llamadas a funciones (vea que tiene disponible)
	libusb_set_debug(contexto, LIBUSB_LOG_LEVEL_NONE);

	//obtener la lista de dispositivos (notar la TRIPLE indireccion)
	cantidad = libusb_get_device_list(contexto, &dispositivos);

	if(cantidad < 0) {
		cout<<"Hubo un error"<<endl;
		return 1;
	}
	cout<< cantidad <<" dispositivos en la lista."<<endl;
	//barro la lista
	for(i = 0; i < cantidad; i++) {
		cout << "Dispositivo "<< i+1 <<endl;
		printdev(dispositivos[i]); //muestro algunos datos de los dispositivos
	}
	//libero la lista
	libusb_free_device_list(dispositivos, 1);

	//cierro la sesion
	libusb_exit(contexto);
	return 0;
}


void printdev(libusb_device *dev) {
	libusb_device_descriptor descriptor;

	//obtengo la descripcion del dispositivo
	int resultado = libusb_get_device_descriptor(dev, &descriptor);

	if (resultado < 0) {
		cout <<"No se pudo obtener la descripcion del dispositivor" << endl;
		return;
	}
	cout<<"Configuraciones posibles: "<<(int)descriptor.bNumConfigurations<<" ";
	cout<<"Clase de dispositivo: "<<(int)descriptor.bDeviceClass<<" ";
	cout<<"Fabricante (vendorID): "<<descriptor.idVendor<<" ";
	cout<<"Producto ID: "<<descriptor.idProduct<<endl;
	libusb_config_descriptor *config;
	libusb_get_config_descriptor(dev, 0, &config);
	const libusb_interface *inter;
	const libusb_interface_descriptor *interdesc;
	const libusb_endpoint_descriptor *epdesc;
	cout<<"Interfaces: "<<(int)config->bNumInterfaces<<endl;
	for(int i=0; i<(int)config->bNumInterfaces; i++) {
		cout << "Interface:"<<i<<endl;
		inter = &config->interface[i];
		cout<<"Numero de configuraciones alternativas: "<<inter->num_altsetting<<endl;
		for(int j=0; j<inter->num_altsetting; j++) {
			interdesc = &inter->altsetting[j];
			cout<<"Numero de interfaces: "<<(int)interdesc->bInterfaceNumber<<" | ";
			cout<<"Numero de endpoints: "<<(int)interdesc->bNumEndpoints<<endl;
			for(int k=0; k<(int)interdesc->bNumEndpoints; k++) {
				epdesc = &interdesc->endpoint[k];
				cout<<"Tipo de descriptor: "<<(int)epdesc->bDescriptorType<<" | ";
				cout<<"Direccion del Endpont: "<<(int)epdesc->bEndpointAddress<<" | ";
				cout<<endl;
			}
		}
		cout<<endl;
	}
	cout<<endl<<endl<<endl;
	//libero el descriptor
	libusb_free_config_descriptor(config);
	return;
}
