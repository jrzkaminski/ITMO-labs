#include <stdio.h>
#include "mpi.h"
#define BUFSIZE 100

int file(const char *path, const char *data) {
	FILE *f = fopen(path, "w");
	if (f != NULL) {
		fputs(data, f);
		fclose(f);
	}
	return 0;
}

int main(int argc, char **argv)
{
	int bufsize, num, sum;
	MPI_Init(&argc, &argv);
	MPI_Status status;
	MPI_File fh;
	MPI_Info info;
	char buf[BUFSIZE];
	int del;
	file("file.txt", "Hello World!");
	MPI_File_open(MPI_COMM_WORLD, "file.txt", MPI_MODE_RDONLY, MPI_INFO_NULL, &fh);
	MPI_File_set_view(fh, 0, MPI_CHAR, MPI_CHAR, "native", MPI_INFO_NULL);
	sum = 0;
	do {
		MPI_File_read(fh, buf, BUFSIZE, MPI_CHAR, &status);
		MPI_Get_count(&status, MPI_CHAR, &num);
		printf("buf=%s\n", buf);
		sum += num;
	} while (num >= BUFSIZE);
	printf("Number of chars is %d \n", sum);
	printf("1 - delete file \n0 - keep file \n");
	scanf("%d", &del);
	if (del)
		MPI_File_delete("file.txt", info);
		printf("The file has been deleted \n");
	MPI_File_close(&fh);
	MPI_Finalize();
}