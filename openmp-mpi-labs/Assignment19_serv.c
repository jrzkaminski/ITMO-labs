#include <stdio.h>
#include "mpi.h"

int main(int argc, char **argv)
{
	int r, message;
	MPI_Init(&argc, &argv);
	char port_name[MPI_MAX_PORT_NAME];
	MPI_Status status;
	MPI_Comm intercomm;
	MPI_Open_port(MPI_INFO_NULL, port_name);
	printf("portname: %s\n", port_name);
	printf("Waiting for client...\n");
	MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_SELF, &intercomm);
	printf("client connected\n");
	scanf("%d", &message);
	MPI_Recv(&r, 1, MPI_INT, 0, 5, intercomm, &status);
	MPI_Send(&message, 1, MPI_INT, 0, 6, intercomm);
	printf("Server sent %d\n", message);
	MPI_Comm_free(&intercomm);
	MPI_Close_port(port_name);
	printf("Client received %d\n", r);
	MPI_Finalize();
	return 0;
}