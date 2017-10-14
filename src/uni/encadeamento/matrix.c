#include <stdio.h>
#include <stdlib.h>

#include <inttypes.h>

#include "macros.h"
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


static uint32_t rows;
static uint32_t columns;

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
    fprintf(stderr, "%sError: %s | matrix.c on line %d%s\n", ANSI_COLOR_RED,
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

    // Print in the reverse order.
    print_heads(mn->top);
    printf("\n\t--(%d)--\n\t%4s", mn->value, "|");
}

Matrix *matrix_init(int x, int y) {
    Matrix *m = (Matrix*) malloc(sizeof(Matrix*));

    if (check_pointer(m)) {
        pferror("Couldn't initialise the Matrix, exiting.", __LINE__);
        exit(1);
    }

    // Set to be used in other functions
    rows = x;
    columns = y;

    // The process of creating the Matrix is create all the lines of the list
    // first and then create a double linked list and for each pair of lists
    // concatenate (link) those sublist together.

    // Walks along the HEADs.
    MatrixNode* walker = NULL;

    // Create every HEAD of the Matrix
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

        if (*m == NULL) {
            // Empty Matrix
            HEAD->top = HEAD->bottom = NULL;
            *m = HEAD;
        } else {
            walker->bottom = HEAD;
            HEAD->top = walker;
        }

        HEAD->left = HEAD->right = NULL;

        walker = HEAD;

#ifdef DEBUG
        const char *message = "Creating HEAD %d. Walker %p |\
 Walker's top is %p";
        int size = snprintf(NULL, 0, message, i, walker, walker->top);
        char buffer[size+1];
        snprintf(buffer, size, message, i, walker, walker->top);
        pfinfo(buffer);
#endif // DEBUG
    }

    // Creating every line for each HEAD.
    for (int i = 0; i < x; i++) {
        MatrixNode* HEAD = NULL;
        if (i == 0) {
            // Points to the first position of the Matrix available

            // FIXME: This looks like Macgyver, need to revisit this later.
            while (walker->top != NULL) {
                walker = walker->top;
            }

            HEAD = walker;
        } else {
            HEAD = walker->bottom;
            walker = walker->bottom;

            HEAD = walker;
        }

        for (int j = 1; j < y; j++) {
            MatrixNode *e = malloc(sizeof(MatrixNode));

            if (check_pointer(e)) {
                pferror("Creating a MatrixNode failed, no memory?", __LINE__);
                exit(1);
            }

            HEAD->right = e;

            e->left = HEAD;
            e->bottom = e->top = NULL;
            // not sure if the below will work well
            e->pos_x = i;
            e->pos_y = j;
            e->value = i+j;


#ifdef DEBUG
            const char *message = "Creating element %d. Row walker %p |\
 Row walker's left is %p";
            int size = snprintf(NULL, 0, message, j, HEAD, HEAD->left);
            char buffer[size+1];
            snprintf(buffer, size, message, j, HEAD, HEAD->left);
            pfinfo(buffer);
#endif // DEBUG

            HEAD = e;
        }
    }


    // Concat every how of the Matrix
    for (int i = 0; i < x; i++) {
        MatrixNode *row = *m;
        MatrixNode *row_next = (*m)->bottom;

        if (row_next == NULL) {
            break;
        }

        // Move one column right
        row = row->right;
        row_next = row_next->right;

        for (int j = 0; j < y; j++) {
            row->bottom = row_next;
            row_next->top = row;

            // It only needs one verification
            if (row->right != NULL) {
                row = row->right;
                row_next = row_next->right;
            }
        }
    }


    return m;
}

MatrixNode *matrix_get_by_coordinate(MatrixNode *m, uint32_t x, uint32_t y) {
    if (check_pointer(m)) {
        pferror("Argument given to matrix_get_by_coordinate is null",
                __LINE__);
        return NULL;
    } else if (x >= rows || y >= columns) {
        pferror("Values given are out of bounds", __LINE__);
        return NULL;
    }

    MatrixNode *desired = m;

    while (desired->pos_x != x) {
        desired = desired->bottom;
    }

    while (desired->pos_y != y) {
        desired = desired->right;
    }

    return desired;
}

MatrixNode *matrix_get_by_value(MatrixNode *m, int v) {
    if (check_pointer(m)) {
        pferror("Argument given to matrix_get_by_value is null", __LINE__);
        return NULL;
    }

    MatrixNode *desired = NULL;

    while (m->bottom != NULL) {
        while (m->right != NULL) {
            if (m->value == v) {
                desired = m;
                goto end;
            }
            m = m->right;
        }

        m = m->bottom;
    }

end:
    return desired;
}

int matrix_print(Matrix m) {
    if (check_pointer(m)) {
        pferror("Argument given to matrix_print is null", __LINE__);
        return 0;
    }

    MatrixNode *HEAD = m;

    newline;

    while (HEAD != NULL) {
        m = HEAD;
        while (m != NULL) {
            printf(" -- [%d (%d,%d)]", m->value, m->pos_x, m->pos_y);

            // Rightmost column of the matrix.
            if (m->right == NULL) {
                printf(" --");
            }

            m = m->right;
        }
        newline;

        HEAD = HEAD->bottom;
    }

    return 1;
}


int matrix_print_element(MatrixNode *m) {
    if (m == NULL) {
        pferror("Argument given to matrix_print_element is null", __LINE__);
        return 0;
    }

    printf("\n[%d (%d,%d)]", m->value, m->pos_x, m->pos_y);

    return 1;
}

void matrix_free(MatrixNode *m) {
    //The free method is described in the Free section of the paper.
    MatrixNode *walker = m;

    // Walker starts at 0x0 if not we have to reset its position.
    while(walker != NULL) {
        m = walker;

        while (m != NULL) {
            MatrixNode *e = m;
            m = m->right;

            free(e);
        }

        walker = walker->bottom;
    }

    free(m);
}
