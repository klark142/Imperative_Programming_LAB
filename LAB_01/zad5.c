#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main() {
    srand(time(NULL));
    int target = rand() % 101;
    printf("target: %d\n", target);
    int current_num = rand() % 101;
    int lower = 0;
    int upper = 100;
    while (current_num != target) {
        printf("%d\n", current_num);
        if (target > current_num) {
            lower = current_num + 1;
            current_num = (rand() % (upper - lower + 1)) + lower;
        }
        else {
            upper = current_num - 1;
            current_num = rand() % (upper + 1);
        }
    }
    printf("%d\n", current_num);
    return 0;
}