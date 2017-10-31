#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

typedef struct th_args{
	int t_id;
	int n;
	int t;
	int ***submat;
} th_args;

void * column_sum(void *arg){
	th_args *data = (th_args *) arg;
	int *v;
	/*int row, col;
	int *v;

	v = (int *)malloc(n*sizeof(int));

	for(row = 0; row < n; row++){
	v[row] = 0;
	for(col = 0; col < n; col++){
		v[row] = v[row] + m[col][row];
	}
	}*/
	return v;
}

void main(){
	int n, t; //size of square matrix, number of threads
	int **m; //matrix
	int ***submatrix;
	int *v;	//column sums
	int i, j, row, col;
	pthread_t thread[t];
	th_args th_data[t];

	printf("Enter size of matrix: ");
	scanf("%d", &n);
	printf("Enter number of threads: ");
	scanf("%d", &t);


	//Initialize arrays
	m = (int **)malloc(n*sizeof(int *) );
	for(i = 0; i < n; i++){
		m[i] = (int *)malloc(n*sizeof(int));
	}

	submatrix = (int ***)malloc(t*sizeof(int **));
	for(i = 0; i < t; i++){
		submatrix[i] = (int **)malloc(n*sizeof(int *));
		for(j = 0; j < n; j++){
			submatrix[i][j] = (int *)malloc((n/t)*sizeof(int));
		}
	}

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

	//Generate submatrices
	for(i = 0; i < t; i++){
		for(row = 0; row < n; row++){
			for(col = 0; col < n/t; col++){
				submatrix[i][row][col] = m[row][col+(i*(n/t))];   			
			}
		}
	}

	//Initialize **submat of thread arguments, not sure if correct and needed
	for(i = 0; i < t; i++){
		th_data[i].submat = (int ***)malloc(t*sizeof(int **));
		for(i = 0; i < n; i++){
			th_data[i].submat[i] = (int **)malloc(n*sizeof(int *));
			for(j = 0; j < n/t; j++){
				th_data[i].submat[i][j] = (int *)malloc((n/t)*sizeof(int));
			}
		}
	}
		
	//paglalagay ng values into thread arguments' variables	
	for(i = 0; i < t; i++){
		th_data[i].t_id = i;
		th_data[i].n = n;
		th_data[i].t = t;
		for(row = 0; row < n; row++){
			for(col = 0; col < n/t; col++){
				th_data[i].submat[i][row][col] = submatrix[i][row][col];
			}
		}
		//v = pthread_create(&thread[i], NULL, column_sum, &th_data[i]);
	}


	//Print matrix
	for(row = 0; row < n; row++){
		for(col = 0; col < n; col++){
			printf("%3d", m[row][col]);
		}
		printf("\n");
	}

	//Print submatrix
	int p,q,r;
	for(p = 0; p < t; p++){
		for(q = 0; q < n; q++){
			for(r = 0; r < n/t; r++){
				printf("%3d", submatrix[p][q][r]);
			}
			printf("\n");
		}
		printf("\n");
	}

	//Print sums
	for(i = 0; i < n; i++){
		printf("%3d", v[i]);
	}
	printf("\n");

	//pang check if tama ba napupunta sa thread arguments
	printf("=================================\n");
	for(i = 0; i < t; i++){
		printf("%d\n", th_data[i].t_id);
		printf("%d\n", th_data[i].n);
		printf("%d\n", th_data[i].t);
		for(p = 0; p < n; p++){
			for(q = 0; p < n/t; p++){
				printf("%3d", submatrix[i][p][q]);
			}
			printf("\n");
		}
		printf("\n");
	}

	//Free memory
	for(i = 0; i < n; i++){
		free(m[i]);
	}
	free(m);
	free(v);
}