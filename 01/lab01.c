#include<stdio.h>
#include<stdlib.h>
#include<time.h>

int * column_sum(int **m, int n){
	int row, col;
	int *v;

	v = (int *)malloc(n*sizeof(int));

	
	for(row = 0; row < n; row++){
		v[row] = 0;
		for(col = 0; col < n; col++){
			v[row] = v[row] + m[col][row];
		}
	}
	return v;
}

void main(){
	int n;	//size of square matrix
	int **m; //main matrix
	int *v;	//array of column sums
	int i, row, col;
	clock_t begin, end;
	double elapsed;

	printf("Enter size of matrix: ");
	scanf("%d", &n);

	//Allocate memory space for the main matrix given nxn dimension
	m = (int **)malloc(n*sizeof(int *));
	for(i = 0; i < n; i++){
		m[i] = (int *)malloc(n*sizeof(int));
	}

	//Allocate memory space for array of column sums
	v = (int *)malloc(n*sizeof(int));
	for(i = 0; i < n; i++){
		v[i] = 0;
	}

	//Generate random number for the matrix
	srand(time(NULL));
	for(row = 0; row < n; row++){
		for(col = 0; col < n; col++){
			m[row][col] = rand() % 10;
		}
	}

	begin = clock();
	v = column_sum(m, n);
	end =  clock();
	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;	//column_sum timing

	printf("Elapsed: %f sec\n", elapsed);
}