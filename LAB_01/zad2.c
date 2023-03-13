// Napisz program, który dla zadanej liczby naturalnej n odpowiada na pytanie, 
// czy liczba ta jest iloczynem dowolnych dwóch kolejnych wyrazów ciągu Fibonacciego. 
// Zakładamy, że pierwsze dwa wyrazy ciągu Fibonacciego to 0 i 1.

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

bool is_valid(int target) {
    int a = 0;
    int b = 1;
    int c;

    while (b < target) {
        if (a * b == target) return true;
        c = a + b;
        a = b;
        b = c;
    }
    return false;
}

int main() {
    int n;
    printf("Podaj liczbę naturalną: ");
    scanf("%d", &n);
    printf("%s\n", is_valid(n) ? "true" : "false");
    return 0;
}