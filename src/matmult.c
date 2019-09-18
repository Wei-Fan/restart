#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/time.h>
#include <inttypes.h>

double* readMatrixFromFile(char* fileName, int height, int width);
int writeMatrixToFile(char* fileName, double* matrix, int height, int width);
void printMatrix(double* matrix, int height, int width);
int64_t utime_now (void);

int main(int argc, char **argv)
{
	if (argc != 5)
	{
		fprintf(stderr, "Input Error\n");
		return 0;
	}

	int a1 = atoi(argv[1]);
	int b1 = atoi(argv[2]);
	int a2 = atoi(argv[3]);
	int b2 = atoi(argv[4]);

	assert(a1>1 && b1>1 && a2>1 && b2>1);
	assert(b1==a2);

	// printf("%d, %d, %d, %d\n", a1,b1,a2,b2);
	char filename1[] = "A.csv";
	char filename2[] = "B.csv";
	char filename3[] = "C.csv";

	double *m1, *m2;
	m1 = readMatrixFromFile(filename1, a1, b1);
	m2 = readMatrixFromFile(filename2, a2, b2);
	// printMatrix(m1, a1, b1);
	// printMatrix(m2, a2, b2);
	int64_t start = utime_now();
	// printf("%" PRId64 "\n", start);

	double *rst = (double*) malloc(a1 * b2 * sizeof(double));
	for (int i = 0; i < a1; ++i)
	{
		for (int j = 0; j < b2; ++j)
		{
			double tmp = 0;
			for (int k = 0; k < a2; ++k)
			{
				tmp += m1[i*b1 + k] * m2[k*b2 + j];
			}
			rst[i*b2 + j] = tmp;
		}
	}

	int64_t end = utime_now();
	// printf("%" PRId64 "\n", end);

	printf("result: \n");
	// printMatrix(rst, a1, b2);

	printf("time used: %" PRId64 " usec\n", end - start);

	writeMatrixToFile(filename3, rst, a1, b2);

	return 0;
}

double* readMatrixFromFile(char* fileName, int height, int width) {
  FILE* fp = fopen(fileName, "r");
  if (fp == NULL) {
	fprintf(stderr, "Can't open %s.\n", fileName);
	return NULL;
  }
  double val;
  double* M = (double*) malloc(height * width * sizeof(double));
  for(int i = 0; i < height; i++) {
	for(int j = 0; j < width; j++) {
  	if (fscanf(fp, " %lf", &val) != 1) {
    	fprintf(stderr, "Couldn't read value.\n");
    	return NULL;
  	}
  	// Discard the comma without checking.
  	fgetc(fp);
  	M[i * width + j] = val;
	}
  }
  fclose(fp);
  return M;
}

int writeMatrixToFile(char* fileName, double* matrix, int height, int width) {
  FILE* fp = fopen(fileName, "w");
  if (fp == NULL) {
	return 1;
  }
 
  for (int i = 0; i < height; i++) {
	for (int j = 0; j < width; j++) {
  	if (j > 0) {
    	fputc(',', fp);
  	}
  	fprintf(fp, "%lf", matrix[i*width +j]);
	}
	fputs("\r\n", fp);
  }
  fclose(fp);
  return 0;
}

void printMatrix(double* matrix, int height, int width){
	printf("\n");
	for (int i = 0; i < height; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			printf("%f ", matrix[i*width+j]);
		}
		printf("\n");
	}
	printf("\n");
}

int64_t utime_now (void){
	struct timeval tv;
	gettimeofday (&tv, NULL);
	return (int64_t) tv.tv_sec * 1000000 + tv.tv_usec;
}

