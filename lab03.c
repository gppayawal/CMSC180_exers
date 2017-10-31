/**
 * gcc lab02.c -lpthread
 **/
#define _GNU_SOURCE
#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include <sched.h>

typedef struct th_args{
	int t_id;
	int n;
	int nSubmatrix;
	int ***submat;
	int *v;
} th_args;

void * column_sum(void *arg){
	th_args *th_data = (th_args *) arg;
	int row, col;
	(th_data->v) = (int *)malloc(th_data->nSubmatrix*sizeof(int));

	for(row = 0; row < th_data->nSubmatrix; row++){
		for(col = 0; col < th_data->n; col++){
			th_data->v[row] = th_data->v[row] + (*th_data->submat)[col][row];
		}
	}

	// for(row = 0; row < th_data->nSubmatrix; row++){
	// 		printf("row %d, id %d: %d\n", row, th_data->t_id, th_data->v[row]);
	// }

	return NULL;
}

void main(){
	int n, t, nSubmatrix; //size of square matrix, number of threads
	int **m; //matrix
	int ***submatrix;
	int **v;	//column sums
	int i, j, row, col, nCpu;
	clock_t begin, end;
	double elapsed;

	printf("Enter size of matrix: ");
	scanf("%d", &n);
	printf("Enter number of threads: ");
	scanf("%d", &t);

	pthread_t thread[t];
	th_args *th_data;

	// core affine
	cpu_set_t cpuset;
	CPU_ZERO(&cpuset);
	nCpu = 4;
	for(i = 0; i < nCpu; i++) {
		CPU_SET(i, &cpuset);
	}

	nSubmatrix = n/t;
	// if odd # of n
	if(n > nSubmatrix*t) nSubmatrix++;

	//Initialize arrays
	m = (int **)malloc(n*sizeof(int *) );
	for(i = 0; i < n; i++){
		m[i] = (int *)malloc(n*sizeof(int));
	}

	submatrix = (int ***)malloc(t*sizeof(int **));
	for(i = 0; i < t; i++){
		submatrix[i] = (int **)malloc(n*sizeof(int *));
		for(j = 0; j < n; j++){
			submatrix[i][j] = (int *)malloc((nSubmatrix/t)*sizeof(int));
		}
	}

	v = (int **)malloc(t*sizeof(int *));
	for(i = 0; i < t; i++){
		v[i] = (int *)malloc(nSubmatrix*sizeof(int));
	}

	th_data = (th_args *)malloc(t*sizeof(th_args));

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
			for(col = 0; col < nSubmatrix; col++){
				submatrix[i][row][col] = m[row][col+(i*(nSubmatrix))];
			}
		}
	}

	//Print matrix
	// for(row = 0; row < n; row++){
	// 	for(col = 0; col < n; col++){
	// 		printf("%3d", m[row][col]);
	// 	}
	// 	printf("\n");
	// }

	//Print submatrix
	// int p,q,r;
	// for(p = 0; p < t; p++){
	// 	for(q = 0; q < n; q++){
	// 		for(r = 0; r < nSubmatrix; r++){
	// 			printf("%3d", submatrix[p][q][r]);
	// 		}
	// 		printf("\n");
	// 	}
	// 	printf("\n");
	// }

	//Initialize **submat of thread arguments, not sure if correct and needed
	for(i = 0; i < t; i++){
		th_data[i].t_id = i;
		th_data[i].n = n;
		th_data[i].nSubmatrix = nSubmatrix;
		th_data[i].submat = &submatrix[i];
	}

	// for(i = 0; i < t; i++){
	// 	printf("id: %d\n", th_data[i].t_id);
	// 	printf("n: %d\n", th_data[i].n);
	// 	printf("sub: %d\n", th_data[i].nSubmatrix);
	// 	for(row = 0; row < n; row++){
	// 		for(col = 0; col < nSubmatrix; col++){
	// 			printf("%d", (*th_data[i].submat)[row][col]);
	// 		} printf("\n");
	// 	}
	// }

	begin = clock();
	for(i = 0; i < t; i++){
		pthread_create(&thread[i], NULL, column_sum, (void *)&th_data[i]);
		pthread_setaffinity_np(thread[i], sizeof(cpu_set_t), &cpuset);
	}

	for(i = 0; i < t; i++){
		pthread_join(thread[i], NULL);
	}
	end =  clock();

	// copy value to local v
	for(i = 0; i < t; i++){
		for(j = 0; j < nSubmatrix; j++) {
			v[i][j] = th_data[i].v[j];
		}
	}

	elapsed = (double) (end - begin) / CLOCKS_PER_SEC;	//column_sum timing

	printf("Elapsed: %f sec\n", elapsed);

	//Print sums
	// for(i = 0; i < t; i++){
	// 	for(j = 0; j < nSubmatrix; j++) {
	// 		if(v[i][j]!=0) printf("%3d", v[i][j]);
	// 	}
	// }
	// printf("\n");


	//Free memory
	for(i = 0; i < n; i++){
		free(m[i]);
	}
	free(m);
	free(v);

	pthread_exit(NULL);
}
