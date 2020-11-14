#include <stdio.h>
#include <stdbool.h>

#define W 21
#define n 6

int w[6] = {0, 5, 6, 10, 11, 16 };
int include[5] = { 0, };
int count = 0;

void sum_of_subsets(int i, int weight, int total);
bool promising(int i, int weight, int total);
void print_();

int main() {
	int i = 0;
	int total = 0, weight = 0;
	for (int i = 0; i < n; i++) {
		total += w[i];
	}

	printf("정답 : \n");
	sum_of_subsets(0, 0, total);
	printf("호출되는 노드의 숫자 : %d\n", count);
	return 0;
}
void sum_of_subsets(int i, int weight, int total) {
	if (promising(i, weight, total))
		if (weight == W)
			print_();
		else {
			include[i + 1] = w[i + 1];
			sum_of_subsets(i + 1, weight + w[i + 1], total - w[i + 1]);
			count++;
			include[i + 1] = 0;
			sum_of_subsets(i + 1, weight, total - w[i + 1]);
			count++;
		}
}
bool promising(int i, int weight, int total) {
	return (weight + total >= W) && (weight == W || weight + w[i + 1] <= W);
}
void print_() {
	int i = 0;
	printf("{ ");
	for (i = 0; i < n; i++) {
		if (include[i] != 0)
			printf("%d ", include[i]);
	}
	printf("} \n");
}
