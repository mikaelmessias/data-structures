#ifndef MATRIX_H
#define MATRIX_H

#include <inttypes.h>

/**
 *
 * Assignment done under the Data Structures class in UTFPR.
 *
 * authors: Rafael Campos Nunes
 *          Mikael Messias
 *
 */

typedef struct node* Matrix;
typedef struct node MatrixNode;

/**
 * @brief Initialises an MxN matrix and returns a pointer to the new
 * initialised matrix.
 * @param x Is the quantity of rows in the matrix.
 * @param y Is the quantity of columns in the matrix.
 */
Matrix *matrix_init(int x, int y);

/**
 * @brief Inserts a MatrixNode with the given value v at an x,y position.
 * @param m The Matrix to be inserted.
 * @param x The coordinate x to place the MatrixNode.
 * @param y The coordinate y to place the MatrixNode.
 * @param v The value to be inserted on the MatrixNode.
 */
int matrix_insert(Matrix *m, int x, int y, int v);

/**
 * @brief Searched for a MatrixNode on the coordinate x,y.
 * @param m The Matrix to be searched on.
 * @param x The coordinate x.
 * @param y The coordinate y.
 */
MatrixNode *matrix_get_by_coordinate(MatrixNode *m, uint32_t x, uint32_t y);

/**
 * @brief Searches for a MatrixNode with the given value v.
 * @param m The Matrix to be searched on.
 * @param v The value to be used on search.
 * @returns A pointer to a MatrixNode or NULL on not found.
 */
MatrixNode *matrix_get_by_value(MatrixNode *m, int v);

/**
 * @brief Prints the matrix structure
 * @param m A pointer to the Matrix to be printed.
 * @returns 1 on success and 0 if it didn't succeed.
 */
int matrix_print(Matrix m);


/**
 * @brief Prints a MatrixNode.
 * @param m A pointer to the MatrixNode to be printed.
 * @returns 1 on success and 0 if it didn't succeed.
 */
int matrix_print_element(MatrixNode *m);

/**
 * @brief Print neighbours of an specific MatrixNode.
 * @param m The reference to be used when printing its neighbours.
 * @returns
 */
int matrix_print_neighbours(MatrixNode *m);

/**
 * @brief Frees the allocated Matrix.
 * @param m The matrix to be freed.
 */
void matrix_free(MatrixNode *m);


#endif // MATRIX_H
