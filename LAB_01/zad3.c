// Liczba doskonała jest to taka liczba naturalna, która jest sumą wszystkich swych 
// dzielników właściwych (to znaczy od niej mniejszych). 
// Najmniejszą liczbą doskonałą jest 6, ponieważ jej dzielnikami właściwymi 
// są 1, 2, 3 i 1 + 2 + 3 = 6. Napisz program, który znajduje wszystkie liczby 
// doskonałe w zadanym przedziale oraz ich liczbę.

#include <stdio.h>

void print_perfect_numbers(int start, int end) {
    for (int liczba = start; liczba <= end; liczba++) {
        int curr_sum = 0;
        int i = 1;
        while (i < liczba) {
            if (liczba % i == 0) curr_sum += i;
            i++;
        }
        if (curr_sum == liczba) printf("%d\n", liczba);
    }
}

int main() {
    int start;
    int end;
    printf("Podaj początek przedziału: ");
    scanf("%d", &start);
    printf("Podaj koniec przedziału: ");
    scanf("%d", &end);
    print_perfect_numbers(start, end);
    return 0;
}