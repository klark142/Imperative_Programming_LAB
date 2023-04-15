#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>

// consider chars from [FIRST_CHAR, LAST_CHAR)
#define FIRST_CHAR 33
#define LAST_CHAR 127
#define MAX_CHARS (LAST_CHAR - FIRST_CHAR)
#define MAX_BIGRAMS ((LAST_CHAR - FIRST_CHAR) * (LAST_CHAR - FIRST_CHAR))

#define NEWLINE '\n'
#define IN_WORD 1

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2

int count[MAX_BIGRAMS] = { 0 };

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp (const void *a, const void *b) {
    int va = *(int*)a;
    int vb = *(int*)b;
    if (count[va] == count[vb]) return va - vb;
    return count[vb] - count[va];
}

// sort indices according to their respective counts.
// sort alphabetically if counts equal
int cmp_di (const void *a, const void *b) {
	int va = *(int*)a;
	int vb = *(int*)b;
	// sort according to second char if counts and the first char equal
	if (count[va] == count[vb] && va / MAX_CHARS == vb / MAX_CHARS) return va % MAX_CHARS - vb % MAX_CHARS;
	// sort according to first char if counts equal
	if (count[va] == count[vb]) return va / MAX_CHARS - vb / MAX_CHARS;
	return count[vb] - count[va];
}

// count lines, words & chars in a given text file
void wc(int *nl, int *nw, int *nc) {
    int c;
    int in_word = 0;
    (*nl) = 0;
    (*nw) = 0;
    (*nc) = 0;
    while ((c = getchar()) != EOF) {
        (*nc)++;
        if (c == '\n') {
            (*nl)++;
        }
        if (isspace(c) || c == '\t') {
            in_word = 0;
        }
        else {
            if (!in_word) {
                in_word = 1;
                (*nw)++;
            }
        }
    }
    (*nl)++;
    (*nc)++;
}

void char_count(int char_no, int *n_char, int *cnt) {
    int c;
    int indexes[LAST_CHAR - FIRST_CHAR];
    for (int i = 0; i < LAST_CHAR - FIRST_CHAR; i ++) {
        indexes[i] = i;
    }
    while ((c = getchar()) != EOF) {
        if ((int) c >= FIRST_CHAR && (int) c < LAST_CHAR) {
            count[(int) c - FIRST_CHAR] += 1;
        }
    }
    int size = sizeof(indexes) / sizeof(indexes[0]);
    qsort((void*)indexes, size, sizeof(indexes[0]), cmp);
    *cnt = count[indexes[char_no - 1]];
    *n_char = (unsigned char) (indexes[char_no - 1] + FIRST_CHAR);
}

bool is_valid_char(int *a) {
    *a -= FIRST_CHAR;
    if (*a >= FIRST_CHAR && *a < LAST_CHAR) {
        return true;
    }
    return false;
}
void bigram_count(int bigram_no, int bigram[]) {
    int c0 = EOF;
    int c1;
    int width = (LAST_CHAR - FIRST_CHAR);
    int indexes[width * width];
    for (int i = 0; i < width * width ; i++) {
        indexes[i] = i;
    }
    while ((c1 = getchar()) != EOF) {
        if (is_valid_char(&c1) && is_valid_char(&c0)) {
            count[c0 * width + c1] += 1;
        }
        c0 = c1;
    }

    int size = sizeof(indexes) / sizeof(indexes[0]);
    qsort((void*)indexes, size, sizeof(indexes[0]), cmp);
    int max_ind = indexes[bigram_no - 1];
    int max_row = max_ind / width;
    int max_col = max_ind % width;
    bigram[0] = (unsigned char) max_row + FIRST_CHAR;
    bigram[1] = (unsigned char) max_col + FIRST_CHAR;
    bigram[2] = count[max_ind];
}

void find_comments(int *line_comment_counter, int *block_comment_counter) {
    int ch, last_ch = EOF, in_line_comment = false, in_block_comment = false;
    *line_comment_counter = 0;
    *block_comment_counter = 0;
    while ((ch = getchar()) != EOF) {
        if (ch == '/' && last_ch == '/' && !in_block_comment) {
            in_line_comment = true;
            (*line_comment_counter)++;
        } else if (last_ch == '/' && ch == '*' && !in_line_comment) {
            in_block_comment = true;
            (*block_comment_counter)++;
            while (last_ch != '*' && ch != '/') {
                last_ch = ch;
                ch = getchar();
            }
            in_block_comment = false;
        } else if (in_line_comment && ch == '\n') {
            in_line_comment = false;
        }
        last_ch = ch;
    }
}

#define MAX_LINE 128

int read_int() {
	char line[MAX_LINE];
	fgets(line, MAX_LINE, stdin); // to get the whole line
	return (int)strtol(line, NULL, 10);
}

int main(void) {
	int to_do;
	int nl, nw, nc, char_no, n_char, cnt;
	int line_comment_counter, block_comment_counter;
	int bigram[3];

	to_do = read_int();
	switch (to_do) {
		case 1: // wc()
			wc (&nl, &nw, &nc);
			printf("%d %d %d\n", nl, nw, nc);
			break;
		case 2: // char_count()
			char_no = read_int();
			char_count(char_no, &n_char, &cnt);
			printf("%c %d\n", n_char, cnt);
			break;
		case 3: // bigram_count()
			char_no = read_int();
			bigram_count(char_no, bigram);
			printf("%c%c %d\n", bigram[0], bigram[1], bigram[2]);
			break;
		case 4:
			find_comments(&line_comment_counter, &block_comment_counter);
			printf("%d %d\n", block_comment_counter, line_comment_counter);
			break;
		default:
			printf("NOTHING TO DO FOR %d\n", to_do);
			break;
	}
	return 0;
}

