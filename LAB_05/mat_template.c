#include <stdio.h>
#include <math.h>
#include "stdlib.h"

#define SIZE 40

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

void read_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		scanf("%lf", x++);
	}
}

void print_vector(double x[], int n) {
	for(int i = 0; i < n; ++i) {
		printf("%.4f ", x[i]);
	}
	printf("\n");
}

void read_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			scanf("%lf", &A[i][j]);
		}
	}
}

void print_mat(double A[][SIZE], int m, int n) {
	for(int i = 0; i < m; ++i) {
		for(int j = 0; j < n; ++j) {
			printf("%.4f ", A[i][j]);
		}
		printf("\n");
	}
}

void print_mat_perm(double A[][SIZE], int perm[], int n) {
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            printf("%.4f ", A[perm[i]][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// 1. Calculate matrix product, AB = A X B
// A[m][p], B[p][n], AB[m][n]
void mat_product(double A[][SIZE], double B[][SIZE], double AB[][SIZE], int m, int p, int n) {
    for (int a_row = 0; a_row < m; a_row++) {
        for (int b_col = 0; b_col < n; b_col++) {
            AB[a_row][b_col] = 0;
            for (int element = 0; element < p; element++) {
                AB[a_row][b_col] += A[a_row][element] * B[element][b_col];
            }
        }
    }
}


// 2. Matrix triangulation and determinant calculation - simplified version
// (no rows' swaps). If A[i][i] == 0, function returns NAN.
// Function may change A matrix elements.
double gauss_simplified(double A[][SIZE], int n) {
    for (int main_row = 0; main_row < n; main_row++) {
        double main_element = A[main_row][main_row];
        if (main_element == 0) {
            return NAN;
        }
        for (int lower_row = main_row + 1; lower_row < n; lower_row++) {
            double scalar = -(A[lower_row][main_row] / main_element);
            for (int lower_row_cols = 0; lower_row_cols < n; lower_row_cols++) {
                A[lower_row][lower_row_cols] += scalar * A[main_row][lower_row_cols];
            }
        }
    }
    double determinant = A[0][0];
    for (int i = 1; i < n; i++) {
        determinant *= A[i][i];
    }
    if (determinant == 0) return NAN;
    return determinant;
}

void backward_substitution_index(double A[][SIZE], const int indices[], double x[], int n) {
}

// 3. Matrix triangulation, determinant calculation, and Ax = b solving - extended version
// (Swap the rows so that the row with the largest, leftmost nonzero entry is on top. While
// swapping the rows use index vector - do not copy entire rows.)
// If max A[i][i] < eps, function returns 0.
// If det != 0 && b != NULL && x != NULL then vector x should contain solution of Ax = b.
void swap(int tab[], int i, int j) {
    int temp = tab[j];
    tab[j] = tab[i];
    tab[i] = temp;
}

double gauss(double A[][SIZE], const double b[], double x[], const int n, const double eps) {
    int row_perm[n];
    double B[n];
    int swap_count = 0;
    for (int i = 0; i < n; i++) {
        row_perm[i] = i;
        B[i] = b[i];
    }


//    print_mat_perm(A, row_perm, n);
    for (int main_row = 0; main_row < n; main_row++) {
        for (int lower_row = main_row + 1; lower_row < n; lower_row++) {
            if (fabs(A[row_perm[lower_row]][main_row]) > fabs(A[row_perm[main_row]][main_row])) {
                swap(row_perm, lower_row, main_row);
                swap_count++;
            }
        }

//        print_mat_perm(A, row_perm, n);

        if (fabs(A[row_perm[main_row]][main_row]) < eps) {
            return 0;
        }
        double main_element = A[row_perm[main_row]][main_row];
        for (int lower_row = main_row + 1; lower_row < n; lower_row++) {
            double scalar = -(A[row_perm[lower_row]][main_row] / main_element);
            for (int lower_row_cols = 0; lower_row_cols < n; lower_row_cols++) {
                A[row_perm[lower_row]][lower_row_cols] += scalar * A[row_perm[main_row]][lower_row_cols];
            }
            B[row_perm[lower_row]] += scalar * B[row_perm[main_row]];
        }

//        print_mat_perm(A, row_perm, n);
    }
    double determinant = A[row_perm[0]][0];
    for (int i = 1; i < n; i++) {
        determinant *= A[row_perm[i]][i];
    }

//    for (int i = 0; i < n; i++) {
//        printf("%d\n", row_perm[i]);
//    }
//    print_mat_perm(A, row_perm, n);


    if (determinant != 0 && b != NULL && x != NULL) {
        for (int row = n - 1; row >= 0; row--) {
            double right_side = B[row_perm[row]];
            for (int i = row + 1; i < n; i++) {
                right_side -= A[row_perm[row]][i] * x[i];
            }
            x[row] = right_side / A[row_perm[row]][row];
        }
    }
    return (swap_count % 2 == 0) ? determinant : -determinant;

}

// 4. Returns the determinant; B contains the inverse of A (if det(A) != 0)
// If max A[i][i] < eps, function returns 0.
double matrix_inv(double A[][SIZE], double B[][SIZE], int n, double eps) {
    int row_perm[n];
    double b[n][n];
    int swap_count = 0;
    for (int i = 0; i < n; i++) {
        row_perm[i] = i;
        b[i][i] = 1;
    }


//    print_mat_perm(A, row_perm, n);
    for (int main_row = 0; main_row < n; main_row++) {
        for (int lower_row = main_row + 1; lower_row < n; lower_row++) {
            if (fabs(A[row_perm[lower_row]][main_row]) > fabs(A[row_perm[main_row]][main_row])) {
                swap(row_perm, lower_row, main_row);
                swap_count++;
            }
        }

//        print_mat_perm(A, row_perm, n);

        if (fabs(A[row_perm[main_row]][main_row]) < eps) {
            return 0;
        }
        double main_element = A[row_perm[main_row]][main_row];
        for (int lower_row = main_row + 1; lower_row < n; lower_row++) {
            double scalar = -(A[row_perm[lower_row]][main_row] / main_element);
            for (int lower_row_cols = 0; lower_row_cols < n; lower_row_cols++) {
                A[row_perm[lower_row]][lower_row_cols] += scalar * A[row_perm[main_row]][lower_row_cols];
                b[row_perm[lower_row]][lower_row_cols] += scalar * b[row_perm[main_row]][lower_row_cols];
            }
        }
    }

    double determinant = A[row_perm[0]][0];
    for (int i = 1; i < n; i++) {
        determinant *= A[row_perm[i]][i];
    }
//    print_mat_perm(A, row_perm, n);
//    print_mat_perm(B, row_perm, n);



    if (determinant != 0) {
        for (int main_row = n - 1; main_row >= 0; main_row--) {
            double to_one = 1 / A[row_perm[main_row]][main_row];
            for (int cols = n - 1; cols >= 0; cols--) {
                A[row_perm[main_row]][cols] *= to_one;
                b[row_perm[main_row]][cols] *= to_one;
            }
        }

//    print_mat_perm(A, row_perm, n);

        for (int main_row = n - 1; main_row >= 0; main_row--) {
            for (int upper_rows = main_row - 1; upper_rows >= 0; upper_rows--) {
                double scalar = -(A[row_perm[upper_rows]][main_row]);
                for (int cols = 0; cols < n; cols++) {
                    A[row_perm[upper_rows]][cols] += scalar * A[row_perm[main_row]][cols];
                    b[row_perm[upper_rows]][cols] += scalar * b[row_perm[main_row]][cols];
                }
            }
        }

//    print_mat_perm(A, row_perm, n);
//    for (int i = 0; i < n; i++) {
//        printf("%d\n", row_perm[i]);
//    }
        for (int i = 0; i < n; i++) {
            int dest = row_perm[i];
            for (int j = 0; j < n; j++) {
                B[i][j] = b[dest][j];
            }
        }
    }

    return (swap_count % 2 == 0) ? determinant : -determinant;
}

int main(void) {

	double A[SIZE][SIZE], B[SIZE][SIZE], C[SIZE][SIZE];
	double b[SIZE], x[SIZE], det, eps = 1e-13;

	int to_do;
	int m, n, p;

	scanf ("%d", &to_do);

	switch (to_do) {
		case 1:
			scanf("%d %d %d", &m, &p, &n);
			read_mat(A, m, p);
			read_mat(B, p, n);
			mat_product(A, B, C, m, p, n);
			print_mat(C, m, n);
			break;
		case 2:
			scanf("%d", &n);
			read_mat(A, n, n);
			printf("%.4f\n", gauss_simplified(A, n));
			break;
		case 3:
			scanf("%d", &n);
			read_mat(A,n, n);
			read_vector(b, n);
			det = gauss(A, b, x, n, eps);
			printf("%.4f\n", det);
			if (det) print_vector(x, n);
			break;
		case 4:
			scanf("%d", &n);
			read_mat(A, n, n);
			det = matrix_inv(A, B, n, eps);
			printf("%.4f\n", det);
			if (det) print_mat(B, n, n);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

