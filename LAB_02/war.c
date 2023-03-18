#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define DECK_SIZE 52

enum state { OK = 0, UNDERFLOW = -1, OVERFLOW = -2 };
enum result {
    CONFLICTS_LIMIT_EXCEEDED = 0,
    WAR_UNRESOLVED = 1,
    PLAYER_A_WON = 2,
    PLAYER_B_WON = 3,
};

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
#define CBUFF_SIZE DECK_SIZE
struct Queue {
    int cbuff[CBUFF_SIZE];
    int out;
    int len;
};

int cbuff_push(struct Queue* q, int value) {
    if (q->len == CBUFF_SIZE) return OVERFLOW;
    q->cbuff[(q->out + q->len) % (CBUFF_SIZE)] = value;
    q->len++;
    return OK;
}

int cbuff_pop(struct Queue* q) {
    int value = q->cbuff[q->out];
    q->out = (q->out + 1) % (CBUFF_SIZE);
    q->len--;
    return value;
}

void cbuff_print(struct Queue* q) {
    int i = q->len;
    int curr_index = q->out;
    while (i) {
        printf("%d ", q->cbuff[curr_index]);
        curr_index = (curr_index + 1) % (CBUFF_SIZE);
        i--;
    }
}


struct Player {
    struct Queue hand;

    int current_war_cards[CBUFF_SIZE];
    int current_card_number;
    int current_card;
};

int war_cards = 0;

void add_to_hand(struct Player* player, int card_nr){
    cbuff_push(&player->hand, card_nr);
}

void hand_to_table(struct Player* player){
    player->current_card_number = cbuff_pop(&player->hand);
    player->current_card = player->current_card_number / 4;
    player->current_war_cards[war_cards] = player->current_card_number;
}


void shuffle_and_distribute(int main_deck[], struct Player* first_player, struct Player* second_player) {
    rand_permutation(DECK_SIZE, main_deck);

    for (int i = 0; i < DECK_SIZE / 2; i++) {
        add_to_hand(first_player, main_deck[i]);
    }

    for (int i = DECK_SIZE / 2; i < DECK_SIZE; i++) {
        add_to_hand(second_player, main_deck[i]);
    }
}

void collect_war_cards(struct Player* winner, struct Player* loser) {
    for (int i = 0; i < war_cards; i++) {
        add_to_hand(winner,  winner->current_war_cards[i]);
    }
    for (int i = 0; i < war_cards; i++) {
        add_to_hand(winner, loser->current_war_cards[i]);
    }
    war_cards = 0;
}

struct Player player_a;
struct Player player_b;
int conflicts = 0;

int war(int max_conflicts, bool never_war) {
    bool compare = true;
    bool war_cards_just_collected = false;
    while (player_a.hand.len > 0 && player_b.hand.len > 0 && conflicts < max_conflicts) {
        hand_to_table(&player_a);
        hand_to_table(&player_b);
        war_cards++;

        if (compare || never_war) {
            conflicts++;
            if (player_a.current_card > player_b.current_card) {
                collect_war_cards(&player_a, &player_b);
                war_cards_just_collected = true;
            } else if (player_a.current_card < player_b.current_card) {
                collect_war_cards(&player_b, &player_a);
                war_cards_just_collected = true;
            } else if (never_war){
                add_to_hand(&player_a, player_a.current_card_number);
                add_to_hand(&player_b, player_b.current_card_number);
            }else{
                compare = false;
                war_cards_just_collected = false;
            }
        } else {
            compare = true;
            war_cards_just_collected = false;
        }
    }

    if (conflicts >= max_conflicts) {
        return CONFLICTS_LIMIT_EXCEEDED;
    }
    else if ((player_a.hand.len == 0 || player_b.hand.len == 0) && !war_cards_just_collected) {
        return WAR_UNRESOLVED;
    }
    else if (player_a.hand.len != 0 && player_b.hand.len == 0) {
        return PLAYER_A_WON;
    }
    else {
        return PLAYER_B_WON;
    }
}


int main(void) {
    int seed, version, max_conflicts;
    scanf("%d %d %d", &seed, &version, &max_conflicts);
    srand((unsigned) seed);

    int shuffle_array[DECK_SIZE];
    shuffle_and_distribute(shuffle_array, &player_a, &player_b);
    int result = version == 0 ? war(max_conflicts, false) : war(max_conflicts, true);

    switch(result) {
        case CONFLICTS_LIMIT_EXCEEDED:
            printf("0 ");
            printf("%d ", player_a.hand.len + war_cards);
            printf("%d ", player_b.hand.len + war_cards);
            break;
        case WAR_UNRESOLVED:
            printf("1 ");
            printf("%d ", player_a.hand.len + war_cards);
            printf("%d ", player_b.hand.len + war_cards);
            break;
        case PLAYER_A_WON:
            printf("2 ");
            printf("%d", conflicts);
            break;
        case PLAYER_B_WON:
            printf("3\n");
            cbuff_print(&player_b.hand);
            break;
    }

    return 0;
}