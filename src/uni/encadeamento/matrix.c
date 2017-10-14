#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include "matrix.h"

struct node {
    int value;

    // Position of the node in the matrix
    uint32_t pos_x;
    uint32_t pos_y;

    MatrixNode *top;
    MatrixNode *bottom;
    MatrixNode *left;
    MatrixNode *right;
};

/**
 * @brief Checks if a given pointer is NULL.
 * @returns An integer representing whether the pointer is NULL or not, the
 * return possibilities are:
 * 1. The number 1 representing it is NULL
 * 2. The number 0 representing it isn't NULL
 */
static int check_pointer(void *p) {
    return p == NULL;
}

/**
 * @brief Prints a formatted error.
 * @param m The message to be printed.
 * @param line The line where the error was issued.
 */
static void pferror(const char *m, const int line) {
#ifdef __linux__
    fprintf(stderr, "\n%sError: %s | matrix.c on line %d%s\n", ANSI_COLOR_RED,
            m, line, ANSI_COLOR_RESET);
#elif
    fprintf(stderr, "\nError: %s | matrix.c on line %d\n", m, line);
#endif // __linux__
}

/**
 * @brief Prints a formatted info.
 * @param m The message to be printed.
 */
static void pfinfo(const char *m) {
#ifdef __linux__
    fprintf(stderr, "\n%sInfo: %s%s", ANSI_COLOR_GREEN, m, ANSI_COLOR_RESET);
#elif
    fprintf(stderr, "\nInfo: %s", m);
#endif // __linux__
}

/**
 * @brief Recursive function that prints information about each head of the
 * Matrix.
 */
static void print_heads(MatrixNode *mn) {
    if (mn == NULL) {
        return;
    }

    printf("\n\t--(%d)--\n\t%4s", mn->value, "|");
    print_heads(mn->bottom);
}

Matrix *matrix_init(int x, int y) {
    Matrix *m = (Matrix*) malloc(sizeof(Matrix*));

    if (check_pointer(m)) {
        pferror("Couldn't initialise the Matrix, exiting.", __LINE__);
        exit(1);
    }

    // The process of creating the Matrix is create all the lines of the list
    // first and then create a double linked list and for each pair of lists
    // concatenate (link) those sublist together.


    // Walks along the HEADs.
    MatrixNode* walker = NULL;

    // Create every HEAD
    for (int i = 0; i < x; i++) {
        // Creating the HEAD node of the Matrix
        MatrixNode *HEAD = malloc(sizeof(MatrixNode));

        if (check_pointer(HEAD)) {
            pferror("Creating a MatrixNode failed, no memory?", __LINE__);
            exit(1);
        }

        // Setting its position on the Matrix
        HEAD->pos_x = i;
        // The HEAD element is always the first of its column
        HEAD->pos_y = 0;

        // Only for testing purposes
        HEAD->value = i;

        if (*m == NULL) { // Empty Matrix
            HEAD->top = HEAD->bottom = NULL;
            *m = HEAD;
        } else {
            // The walker is ...
            walker->top = HEAD;
            HEAD->bottom = walker;
        }

        HEAD->left = HEAD->right = NULL;

        walker = HEAD;

#ifdef DEBUG
        const char *message = "Creating HEAD %d. Walker is %p";
        int size = snprintf(NULL, 0, message, i, walker->bottom);
        char buffer[size+1];
        snprintf(buffer, size, message, i, walker->bottom);
        pfinfo(buffer);
#endif // DEBUG
    }

    //print_heads(walker);

    for (int i = 0; i < x; i++) {
        MatrixNode* HEAD = NULL;
        if (i == 0) {
            // points to the first HEAD available
            HEAD = walker;
        } else {
            if (walker->bottom != NULL) {
                HEAD = walker->bottom;
                walker = walker->bottom;
            }

            HEAD = walker;
        }

        for (int j = 1; j < y; j++) {
            MatrixNode *e = malloc(sizeof(MatrixNode));

            if (check_pointer(e)) {
                pferror("Creating a MatrixNode failed, no memory?", __LINE__);
                exit(1);
            }

            e->left = HEAD;
            e->right = NULL;
            e->bottom = e->top = NULL;
            // not sure if the below will work well
            e->pos_x = i+1;
            e->pos_y = j;
            e->value = i+j;

            HEAD->right = e;

            HEAD = e;
        }
    }

    matrix_print(*m);

    return m;
}


int matrix_print(Matrix m) {
    MatrixNode *walker = m;

    while(walker != NULL) {
        while (walker->right != NULL) {
            printf("\n\t--(%d)--\n\t%4s", walker->value, "|");
            walker = walker->right;
        }
        walker = walker->bottom;
    }

    return 1;
}


void matrix_free(Matrix *m) {
    //The free method is described in the Free section of the paper.
    MatrixNode *walker = *m;

    // Walker starts at 0x0 if not we have to reset its position.
    while(walker != NULL) {
        while (walker->right != NULL) {
        }
        walker = walker->bottom;
    }
}