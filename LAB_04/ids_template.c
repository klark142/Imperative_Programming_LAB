#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define IN_LINE_COMMENT 1
#define IN_BLOCK_COMMENT 2
#define IN_STRING 4
#define IN_ID 8

#define MAX_ID_LEN 64
#define MAX_IDS 1024

int index_cmp(const void*, const void*);
int cmp(const void*, const void*);

char idents[MAX_IDS][MAX_ID_LEN];

const char *keywords[] = {
	"auto", "break", "case", "char",
	"const", "continue", "default", "do",
	"double", "else", "enum", "extern",
	"float", "for", "goto", "if",
	"int", "long", "register", "return",
	"short", "signed", "sizeof", "static",
	"struct", "switch", "typedef", "union",
	"unsigned", "void", "volatile", "while"
};

bool is_valid_ident_char(char c) {
    return isalnum(c) || c == '_' || c == '#';
}

int find_idents() {
    char ident[MAX_ID_LEN];
    int ident_count = 0, len, i;
    bool ident_exists = false, keyword_error = false, in_comment = false, in_line_comment = false;

    while (scanf("%s", ident) == 1) {
        printf("%c\n", ident[strlen(ident)]);
        len = strlen(ident);
        // komentarz blokowy
        if (in_comment) {
            if (strstr(ident, "*/") != NULL) {
                in_comment = false;
            }
            continue;
        }

        // komentarz liniowy
        if (in_line_comment) {
            int c;
            while ((c = getchar()) != '\n') {
                continue;
            }
            in_line_comment = false;
            continue;
        }

        // początek blokowego
        if (strstr(ident, "/*") != NULL) {
            in_comment = true;
            continue;
        }

        // początek liniowego
        if (strstr(ident, "//") != NULL) {
            in_line_comment = true;
            continue;
        }

        // pomijaj cudzysłowy
        if (ident[0] == '"') {
            continue;
        }

        // pomijaj nawiasy
        if (ident[0] == '(' || ident[strlen(ident) - 1] == ')') {
            continue;
        }

        if (!isalpha(ident[0]) && ident[0] != '_') {
            continue;
        }

        for (i = 1; i < len; i++) {
            if (!is_valid_ident_char(ident[i])) {
                continue;
            }
        }

        for (int j = 0; j < MAX_IDS; j++) {
            if (strcmp(ident, idents[j]) == 0) {
                continue;
            }
        }

        for (int k = 0; k < 32; k++) {
            if (strcmp(ident, keywords[k]) == 0) {
                continue;
            }
        }


        strcpy(idents[ident_count++], ident);
        printf("%s, pog", ident);


    }

    return ident_count;
}


int cmp(const void* first_arg, const void* second_arg) {
	char *a = *(char**)first_arg;
	char *b = *(char**)second_arg;
	return strcmp(a, b);
}

//int index_cmp(const void* first_arg, const void* second_arg) {
//	int a = *(int*)first_arg;
//	int b = *(int*)second_arg;
//	return strcmp(identifiers[a], identifiers[b]);
//}



int main(void) {
	printf("%d\n", find_idents());
	return 0;
}

