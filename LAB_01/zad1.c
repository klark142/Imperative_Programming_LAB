// Napisz program, który wczytuje liczbę całkowitą N a następnie oblicza 
// i drukuje na ekran wartość N! Uwaga: silnia bardzo szybko powoduje przepełnienie całkowitoliczbowe. 
// Sprawdź dla jakich wartości N program wyprowadza prawidłowy wynik. Przetestuj działanie programu dla różnych typów 
// danych (dla ułatwienia można zastosować definicję typedef).

#include <stdio.h>
#include <stdlib.h>

typedef long long int ll;

ll factorial(int n) {
    if (n == 0) return 1;
    ll result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

int main() {
    int n;
    printf("Podaj liczbę całkowitą: ");
    scanf("%d", &n);
    printf("%d! = %lld\n", n, factorial(n));
    return 0;
}

