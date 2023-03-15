#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };

int rand_from_interval(int a, int b) {
    if (a > b) return INT_MIN;
    if (b - a > RAND_MAX) return INT_MAX;
    if (a == b) return a;
    else {
        int num = (rand() % (b - a + 1)) + a;
        return num;
    }
}

void swap (int array[], int i, int k) {
    int temp = array[i];
    array[i] = array[k];
    array[k] = temp;
}

// random permutation of integers from [0, n-1]
void rand_permutation(int n, int array[]) {
    if (n >= 0) {
        for (int i = 0; i < n; i ++) {
            array[i] = i;
        }

        for (int i = 0; i < n - 1; i ++) {
            int k = rand_from_interval(i, n - 1);
            swap(array, i, k);
        }
    }
}

// queue with cyclic buffer
#define CBUFF_SIZE 52

int cbuff_a[CBUFF_SIZE];
int cbuff_b[CBUFF_SIZE];
int out_a = 0, out_b = 0, len_a = 0, len_b = 0;


int cbuff_push_a(int card_nr) {
    if (len_a == CBUFF_SIZE) return OVERFLOW;
    cbuff_a[(out_a + len_a) % (CBUFF_SIZE)] = card_nr;
    len_a++;
    return OK;
}

int cbuff_push_b(int card_nr) {
    if (len_b == CBUFF_SIZE) return OVERFLOW;
    cbuff_b[(out_b + len_b) % (CBUFF_SIZE)] = card_nr;
    len_b++;
    return OK;
}

int cbuff_pop_a() {
    if (len_a == 0) return UNDERFLOW;
    int out_nr = cbuff_a[out_a];
    out_a = (out_a + 1) % (CBUFF_SIZE);
    len_a--;
    return out_nr;
}

int cbuff_pop_b() {
    if (len_b == 0) return UNDERFLOW;
    int out_nr = cbuff_b[out_b];
    out_b = (out_b + 1) % (CBUFF_SIZE);
    len_b--;
    return out_nr;
}

int cbuff_state_a(void) {
    return len_a;
}

int cbuff_state_b(void) {
    return len_b;
}

void cbuff_print_a(void) {
    int i = len_a;
    int curr_index = out_a;
    while (i) {
        printf("%d ", cbuff_a[curr_index]);
        curr_index = (curr_index + 1) % (CBUFF_SIZE);
        i--;
    }
}

void cbuff_print_b(void) {
    int i = len_b;
    int curr_index = out_b;
    while (i) {
        printf("%d ", cbuff_b[curr_index]);
        curr_index = (curr_index + 1) % (CBUFF_SIZE);
        i--;
    }
}

// war

#define DECK_SIZE 52

int shuffle_array[DECK_SIZE];
int war_cards = 0;
int current_war_cards_a[DECK_SIZE];
int current_war_cards_b[DECK_SIZE];
int conflicts = 0;

void shuffle_and_distribute(int main_deck[]) {
    rand_permutation(DECK_SIZE, main_deck);

    // player A deck
    for (int i = 0; i < DECK_SIZE / 2; i++) {
        cbuff_push_a(main_deck[i]);
    }
    // player B deck
    for (int i = DECK_SIZE / 2; i < DECK_SIZE; i++) {
        cbuff_push_b(main_deck[i]);
    }
}

void collect_war_cards_a() {
    // collect A war cards
    for (int i = 0; i < war_cards; i++) {
        cbuff_push_a(current_war_cards_a[i]);
    }
    // collect B war cards
    for (int i = 0; i < war_cards; i++) {
        cbuff_push_a(current_war_cards_b[i]);
    }
    war_cards = 0;
}

void collect_war_cards_b() {
    // collect B war cards
    for (int i = 0; i < war_cards; i++) {
        cbuff_push_b(current_war_cards_b[i]);
    }

    // collect A war cards
    for (int i = 0; i < war_cards; i++) {
        cbuff_push_b(current_war_cards_a[i]);
    }
    war_cards = 0;
}

int war_standard(int max_conflicts) {
    int current_a_card;
    int current_b_card;
    int current_a_card_nr;
    int current_b_card_nr;
    bool was_war;

    while (len_a > 0 && len_b > 0 && conflicts < max_conflicts) {
        was_war = false;
        current_a_card_nr = cbuff_a[out_a];
        current_b_card_nr = cbuff_b[out_b];
        current_a_card = cbuff_pop_a() / 4;
        current_b_card = cbuff_pop_b() / 4;


        if (current_a_card > current_b_card) {
            cbuff_push_a(current_a_card_nr);
            cbuff_push_a(current_b_card_nr);
            conflicts++;
        }
        else if (current_a_card < current_b_card) {
            cbuff_push_b(current_b_card_nr);
            cbuff_push_b(current_a_card_nr);
            conflicts++;
        }
        else { // war
            while (current_a_card == current_b_card && len_a > 0 && len_b > 0 && conflicts < max_conflicts) {
                if (!was_war) {
                    conflicts++;
                    current_war_cards_a[war_cards] = current_a_card_nr;
                    current_war_cards_b[war_cards] = current_b_card_nr;
                    war_cards++;
                }
                // second card face down
                current_a_card_nr = cbuff_a[out_a];
                current_b_card_nr = cbuff_b[out_b];
                current_a_card = cbuff_pop_a() / 4;
                current_b_card = cbuff_pop_b() / 4;
                current_war_cards_a[war_cards] = current_a_card_nr;
                current_war_cards_b[war_cards] = current_b_card_nr;
                war_cards++;
                if (len_a == 0 || len_b == 0) return 2; // b
                if (conflicts >= max_conflicts) return 1; // a
                // third card face up
                conflicts++;
                current_a_card_nr = cbuff_a[out_a];
                current_b_card_nr = cbuff_b[out_b];
                current_a_card = cbuff_pop_a() / 4;
                current_b_card = cbuff_pop_b() / 4;
                current_war_cards_a[war_cards] = current_a_card_nr;
                current_war_cards_b[war_cards] = current_b_card_nr;
                war_cards++;
                if (current_a_card > current_b_card) {
                    collect_war_cards_a();
                }
                else if (current_a_card < current_b_card) {
                    collect_war_cards_b();
                }
                was_war = true;
            }
            if (len_a == 0 || len_b == 0) return 2; // b
            if (conflicts >= max_conflicts) return 1; // a
        }
    }
    if (conflicts >= max_conflicts) {
        return 1; // a
    }
    else if (len_a == 0 && len_b == 0) {
        return 2; // b
    }
    else if (len_a != 0 && len_b == 0) {
        return 3; // c
    }
    else {
        return 4; // d
    }

}

int war_simplified(int max_conflicts) {
    int current_a_card;
    int current_b_card;
    int current_a_card_nr;
    int current_b_card_nr;
    while (len_a > 0 && len_b > 0 && conflicts < max_conflicts) {
        current_a_card_nr = cbuff_a[out_a];
        current_b_card_nr = cbuff_b[out_b];
        current_a_card = cbuff_pop_a() / 4;
        current_b_card = cbuff_pop_b() / 4;
        conflicts++;
        if (current_a_card > current_b_card) {
            cbuff_push_a(current_a_card_nr);
            cbuff_push_a(current_b_card_nr);
        }
        else if (current_a_card < current_b_card) {
            cbuff_push_b(current_b_card_nr);
            cbuff_push_b(current_a_card_nr);
        }
        else if (current_a_card == current_b_card) {
            cbuff_push_a(current_a_card_nr);
            cbuff_push_b(current_b_card_nr);
        }
    }
    if (conflicts >= max_conflicts) {
        return 1; // a
    }
    else if (len_a == 0 && len_b == 0) {
        return 2; // b
    }
    else if (len_a != 0 && len_b == 0) {
        return 3; // c
    }
    else {
        return 4; // d
    }
}



int main(void) {
    int seed, version, max_conflicts;

    scanf("%d %d %d", &seed, &version, &max_conflicts);
    srand((unsigned) seed); // set the seed

    shuffle_and_distribute(shuffle_array);

    // simplified
    if (version == 1) {
        switch(war_simplified(max_conflicts)) {
            case 1:
                printf("0\n");
                printf("%d\n", len_a);
                printf("%d", len_b);
                break;
            case 2:
                printf("1\n");
                printf("%d\n", len_a);
                printf("%d\n", len_b);
                break;
            case 3:
                printf("2\n");
                printf("%d", conflicts);
                break;
            case 4:
                printf("3\n");
                cbuff_print_b();
                break;
        }
    }
    // standard
    else if (version == 0) {
        switch(war_standard(max_conflicts)) {
            case 1:
                printf("0\n");
                printf("%d\n", len_a + war_cards);
                printf("%d\n", len_b + war_cards);
                cbuff_print_b();
                break;
            case 2:
                printf("1\n");
                printf("%d\n", len_a + war_cards);
                printf("%d\n", len_b + war_cards);
                break;
            case 3:
                printf("2\n");
                printf("%d", conflicts);
                break;
            case 4:
                printf("3\n");
                cbuff_print_b();
                break;
        }
    }

    return 0;
}