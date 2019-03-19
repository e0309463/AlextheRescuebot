#include <stdio.h>


int main() {
	long N;
       	scanf("%ld", &N);
	for (long i = 1; i <= N; i += 1) {
		for (long k = 1; k <= N - i; k+=1){
			printf(" ");
		}
		if (i == 1) {
			printf("*\n");
		} else if (i == N) {
			for (long j = 1; j <= 2*i-1; j += 1) {
				printf("*");
			}
			printf("\n");
		} else { 
			for (long j = 1; j <= 2*i-1; j += 1) {
				if (j == 1 || j == 2*i-1) {
					printf("*"); 
				} else { 
					printf("#");
				}

			}
			printf("\n");
		}
	}       
	return 0;
}
