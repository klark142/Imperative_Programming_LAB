// Dana jest liczba całkowita dodatnia n. 
// Napisz program, który znajduje wszystkie liczby pierwsze mniejsze od n, 
// których cyfry tworzą ciąg niemalejący.

#include <stdio.h>
#include <stdbool.h>
#include <math.h>

bool is_prime(int num) {
    if (num == 1) return false;
    if (num == 2 || num == 3) return true;
    if (num % 2 == 0 || num % 3 == 0) return false;
    for (int i = 5; i <= sqrt(num); i += 6) {
        if (num % i == 0 || num % (i + 2) == 0) {
            return false;
        }
    }
    return true;
    
}

bool is_valid(int num) {
    int last_digit = 10;
    while (num > 0) {
        int digit = num % 10;
        if (digit > last_digit) {
            return false;
        }
        last_digit = digit;
        num /= 10;
    }
    return true;
}

int main() {
    int end;
    printf("Podaj liczbę: ");
    scanf("%d", &end);
    for (int liczba = 1; liczba < end; liczba++) {
        if (is_prime(liczba) && is_valid(liczba)) printf("%d\n", liczba);
    }
    return 0;
}



