#include <stdio.h>
#include <string.h>
#include "utils.h"

void clearInputBuffer(void) {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        /* discard trailing characters */
    }
}

void readString(char text[], int max_len) {
    if (fgets(text, max_len, stdin) != NULL) {
        int len = (int)strlen(text);
        if (len > 0 && text[len - 1] == '\n') {
            text[len - 1] = '\0';
        } else {
            clearInputBuffer();
        }
    } else {
        text[0] = '\0';
    }
}
