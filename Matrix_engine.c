#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>

#include "matrix.h"

static int g_seed = 0;

static ssize_t g_width = 0;
static ssize_t g_height = 0;
static ssize_t g_elements = 0;

static ssize_t g_nthreads = 1;

////////////////////////////////
///     UTILITY FUNCTIONS    ///
////////////////////////////////

/**
 * Returns pseudorandom number determined by the seed.
 */
int fast_rand(void) {

	g_seed = (214013 * g_seed + 2531011);
	return (g_seed >> 16) & 0x7FFF;
}

/**
 * Sets the seed used when generating pseudorandom numbers.
 */
void set_seed(int seed) {

	g_seed = seed;
}

/**
 * Sets the number of threads available.
 */
void set_nthreads(ssize_t count) {

	g_nthreads = count;
}

/**
 * Sets the dimensions of the matrix.
 */
void set_dimensions(ssize_t order) {

	g_width = order;
	g_height = order;

	g_elements = g_width * g_height;
}

/**
 * Displays given matrix.
 */
void display(const float* matrix) {

	for (ssize_t y = 0; y < g_height; y++) {
		for (ssize_t x = 0; x < g_width; x++) {
			if (x > 0) printf(" ");
			printf("%.2f", matrix[y * g_width + x]);
		}

		printf("\n");
	}
}

/**
 * Displays given matrix row.
 */
void display_row(const float* matrix, ssize_t row) {

	for (ssize_t x = 0; x < g_width; x++) {
		if (x > 0) printf(" ");
		printf("%.2f", matrix[row * g_width + x]);
	}

	printf("\n");
}

/**
 * Displays given matrix column.
 */
void display_column(const float* matrix, ssize_t column) {

	for (ssize_t i = 0; i < g_height; i++) {
		printf("%.2f\n", matrix[i * g_width + column]);
	}
}

/**
 * Displays the value stored at the given element index.
 */
void display_element(const float* matrix, ssize_t row, ssize_t column) {

	printf("%.2f\n", matrix[row * g_width + column]);
}

////////////////////////////////
///   MATRIX INITALISATIONS  ///
////////////////////////////////

/**
 * Returns new matrix with all elements set to zero.
 */
float* new_matrix(void) {

	return calloc(g_elements, sizeof(float));
}

/**
 * Returns new identity matrix.
 */
float* identity_matrix(void) {

	float* result = new_matrix();

	/*
		1 0
		0 1
	*/
	for(int y = 0; y < g_height;y++){
		for(int x = 0; x<g_width;x++){
			if(y == x){
				// insert 1 into slot where x = y to for indentity matrix.
				result[y * g_width + x] = 1;
			}
		}
	}

	return result;
}

/**
 * Returns new matrix with elements generated at random using given seed.
 */
float* random_matrix(int seed) {

	float* result = new_matrix();

	set_seed(seed);

	for (ssize_t i = 0; i < g_elements; i++) {
		result[i] = fast_rand();
	}

	return result;
}

/**
 * Returns new matrix with all elements set to given value.
 */
float* uniform_matrix(float value) {

	float* result = new_matrix();

	/*
		     1 1
		1 => 1 1
	*/
	for(int i = 0; i < g_elements; i++){
		result[i] = value;
	}
	return result;
}

/*
 * Returns new matrix with elements in sequence from given start and step
 */
float* sequence_matrix(float start, float step) {

	float* result = new_matrix();

	/*
		       1 2
		1 1 => 3 4
	*/

	for(int i = 0; i < g_elements; i++){
		result[i] = start;
		start += step;
	}

	return result;
}

////////////////////////////////
///     MATRIX OPERATIONS    ///
////////////////////////////////

/**
 * Returns new matrix with elements cloned from given matrix.
 */
float* cloned(const float* matrix) {

	float* result = new_matrix();

	// clone the array
	for(int i = 0; i < g_elements; i++){
		result[i] = matrix[i];
	}

	return result;
}

/**
 * Returns new matrix with elements in ascending order.
 */
float* sorted(const float* matrix) {

	float* result = new_matrix();
	float temp;

	/*
		3 4    1 2
		2 1 => 3 4
	*/

	// clone matrix to result
	for(int i = 0; i < g_elements; i++) result[i] = matrix[i];

	for(int i = 0; i < g_elements;i++){
		for(int j = 0; j < g_elements-i-1;j++){
			if(result[j] > result[j+1]){
				temp = result[j];
				result[j] = result[j+1];
				result[j+1] = temp;
			}
		}
	}


	return result;
}
/**
 * Returns new matrix with elements rotated 90 degrees clockwise.
 */
float* rotated(const float* matrix) {

	float* result = new_matrix();
	/*
	TODO
		1 2    3 1
		3 4 => 4 2
	*/

	// Rotate the matrix in 90 degrees
	for(int x = 0; x < g_width; x++){
		for(int y = 0; y < g_height; y++){
			 result[x*g_height+y] = matrix[(g_height-1-y)*g_width+x];
		}
	}


	return result;
}
/**
 * Returns new matrix with elements ordered in reverse.
 */
float* reversed(const float* matrix) {

	float* result = new_matrix();

	/*
		1 2    4 3
		3 4 => 2 1
	*/

	// reverse the array
	for(int i = 0; i < g_elements;i++){
		result[i] = matrix[g_elements-i-1];
	}

	return result;
}

/**
 * Returns new transposed matrix.
 */
float* transposed(const float* matrix) {

	float* result = new_matrix();

	/*
		1 2    1 3
		3 4 => 2 4
	*/

	// Transpose the matrix
	for(int y = 0; y < g_height;y++){
		for(int x = 0; x < g_width; x++){
			result[y*g_width+x] = matrix[x*g_height+y];
		}
	}
	return result;
}

/**
 * Returns new matrix with scalar added to each element.
 */
float* scalar_add(const float* matrix, float scalar) {

	float* result = new_matrix();

	/*
		1 0        2 1
		0 1 + 1 => 1 2

		1 2        5 6
		3 4 + 4 => 7 8
	*/

	// Add x to each elements of result
	for(int y = 0; y < g_height; y++){
		for(int x = 0; x < g_width; x++){
			result[y*g_width+x] = matrix[y*g_width+x] + scalar;
		}
	}

	return result;
}

/*
 * Returns new matrix with scalar multiplied to each element.
 */
float* scalar_mul(const float* matrix, float scalar) {

	float* result = new_matrix();

	/*
		1 0        2 0
		0 1 x 2 => 0 2

		1 2        2 4
		3 4 x 2 => 6 8
	*/

	// Multiply x to each elements of result
	for(int y = 0; y < g_height; y++){
		for(int x = 0; x < g_width; x++){
			result[y*g_width+x] = matrix[y*g_width+x]*scalar;
		}
	}


	return result;
}

/*
 * Returns new matrix that is the result of
 * adding the two given matrices together.
 */
float* matrix_add(const float* matrix_a, const float* matrix_b) {

	float* result = new_matrix();

	/*
		1 0   0 1    1 1
		0 1 + 1 0 => 1 1

		1 2   4 4    5 6
		3 4 + 4 4 => 7 8
	*/

	// Add 2 matrix together
	for(int y = 0; y < g_height; y++){
		for(int x = 0; x < g_width; x++){
			result[y*g_width+x] = matrix_a[y*g_width+x] + matrix_b[y*g_width+x];
		}
	}

	return result;
}

/**
 * Returns new matrix that is the result of
 * multiplying the two matrices together.
 */
float* matrix_mul(const float* matrix_a, const float* matrix_b) {

	float* result = new_matrix();
	/*
		1 2   1 0    1 2
		3 4 x 0 1 => 3 4

		1 2   5 6    19 22
		3 4 x 7 8 => 43 50
	*/

	// Multiply 2 matrix together
	for (int i = 0; i < g_width; i++) {
		for (int j = 0; j < g_height; j++) {
			for (int k = 0; k < g_width; k++)
				result[i * g_height + j] =  result[i * g_height + j] + matrix_a[i * g_height + k] * matrix_b[k * g_height + j];
			 }
}

	return result;
}

/**
 * Returns new matrix that is the result of
 * powering the given matrix to the exponent.
 */
float* matrix_pow(const float* matrix, int exponent) {

	float* result = new_matrix();

	/*
		TODO
		1 2        1 0
		3 4 ^ 0 => 0 1

		1 2        1 2
		3 4 ^ 1 => 3 4

		1 2        199 290
		3 4 ^ 4 => 435 634
	*/

	if(exponent == 0){
		for(int y = 0; y < g_height;y++){
			for(int x = 0; x < g_width;x++){
				if(y == x){
					// insert 1 into slot where x = y to for indentity matrix.
					result[y * g_width + x] = 1;
				}
			}
		}
	}

	else{
		//Cloned value to matrix_b
		for(int i = 0; i < g_elements; i++) result[i] = matrix[i];
		// multiply by n times
		float* temp;
		for(int num = 0; num < exponent - 1; num++){
			temp = matrix_mul(result,matrix);
			free(result);
			result = cloned(temp);
			free(temp);
	}
	}

	return result;
}

/* //////////////////////////////
  //  CONVOLUTION OF MATRIX   //
 //////////////////////////////
 */

 /* ---------------------------------------
    |   Method for extending the matrix   |
		|   - bottom & top,Row & Column       |
		|   - side and Vertex                 |
		---------------------------------------
*/

float* extend_row(const float* matrix, int p, float*side){
		side[0] = matrix[p-g_width-1];
		side[1] = matrix[p-g_width];
		side[2] = matrix[p-g_width];
		side[3] = matrix[p-1];
		side[4] = matrix[p];
		side[5] = matrix[p];
		side[6] = matrix[p + g_width-1];
		side[7] = matrix[p + g_width];
		side[8] = matrix[p + g_width];
	return side;
}
float* extend_col(const float* matrix, int pt, float* side){
		side[0] = matrix[pt-g_width];
		side[1] = matrix[pt-g_width];
		side[2] = matrix[pt-g_width+1];
		side[3] = matrix[pt];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt + g_width];
		side[7] = matrix[pt + g_width];
		side[8] = matrix[pt + g_width+1];
	return side;
}
float* extend_top(const float* matrix, int pt, float* side){
		side[0] = matrix[pt-1];
		side[1] = matrix[pt];
		side[2] = matrix[pt+1];
		side[3] = matrix[pt-1];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt + g_width-1];
		side[7] = matrix[pt + g_width];
		side[8] = matrix[pt + g_width+1];
	return side;
}
float* extend_bottom(const float* matrix, int pt, float* side){
		side[0] = matrix[pt-g_width-1];
		side[1] = matrix[pt-g_width];
		side[2] = matrix[pt-g_width+1];
		side[3] = matrix[pt-1];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt-1];
		side[7] = matrix[pt];
		side[8] = matrix[pt+1];
	return side;
}
float* extend_middle(const float* matrix, int pt, float* side){
		side[0] = matrix[pt-g_width-1];
		side[1] = matrix[pt-g_width];
		side[2] = matrix[pt-g_width+1];
		side[3] = matrix[pt-1];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt+g_width-1];
		side[7] = matrix[pt+g_width];
		side[8] = matrix[pt+g_width+1];
	return side;
}
float* extend_vertex(const float* matrix, int pt, float* side){
	if(pt == 0){
		side[0] = matrix[pt];
		side[1] = matrix[pt];
		side[2] = matrix[pt+1];
		side[3] = matrix[pt];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt + g_width];
		side[7] = matrix[pt + g_width];
		side[8] = matrix[pt + g_width+1];
	}
	if(pt == g_width - 1) {
		side[0] = matrix[pt - 1];
		side[1] = matrix[pt];
		side[2] = matrix[pt];
		side[3] = matrix[pt - 1];
		side[4] = matrix[pt];
		side[5] = matrix[pt];
		side[6] = matrix[pt+g_width-1];
		side[7] = matrix[pt+g_width];
		side[8] = matrix[pt+g_width];
	}
	if(pt == g_elements - g_width) {
		side[0] = matrix[pt-g_width];
		side[1] = matrix[pt-g_width];
		side[2] = matrix[pt-g_width+1];
		side[3] = matrix[pt];
		side[4] = matrix[pt];
		side[5] = matrix[pt+1];
		side[6] = matrix[pt];
		side[7] = matrix[pt];
		side[8] = matrix[pt+1];
	}
	if(pt == g_elements - 1) {
		side[0] = matrix[pt-g_width-1];
		side[1] = matrix[pt-g_width];
		side[2] = matrix[pt-g_width];
		side[3] = matrix[pt-1];
		side[4] = matrix[pt];
		side[5] = matrix[pt];
		side[6] = matrix[pt-1];
		side[7] = matrix[pt];
		side[8] = matrix[pt];
	}
	return side;
}
/*
 * Returns new matrix that is the result of
 * convolving given matrix with a 3x3 kernel matrix.
 */
float* matrix_conv(const float* matrix, const float* kernel) {

	float* result = new_matrix();
	/*
		Convolution is the process in which the values of a matrix are
		computed according to the weighted sum of each value and it's
		neighbours, where the weights are given by the kernel matrix.
	*/

	// Convolution Start
	float* conv = calloc(9,sizeof(float));

	if (g_width == 1){
		for(size_t i = 0; i < 9; i++){
			result[0]+=kernel[i]*matrix[0];
		}
		return result;
	}
	if (g_width == 2){
		for(size_t i = 0; i < g_elements; i++){
			conv = extend_vertex(matrix, i, conv);
			for(size_t j = 0; j < 9; j++){
				result[i]+=kernel[j]*conv[j];
			}
		}
		return result;
	}
	if (g_width >= 3) {
		for(size_t i = 0; i < g_elements; i++) {
			if(i == 0 || i == g_width - 1 || i == g_elements - g_width || i == g_elements - 1) {
				conv = extend_vertex(matrix, i, conv);
			} else if (i%g_width == 0) {
				conv = extend_col(matrix, i, conv);
			} else if (i < g_width) {
				conv = extend_top(matrix, i, conv);
			} else if (i > g_elements-g_width) {
				conv = extend_bottom(matrix, i, conv);
			} else if (i%g_width == g_width-1) {
				conv = extend_row(matrix, i, conv);
			} else {
				conv = extend_middle(matrix, i, conv);
			}
			for(size_t j = 0; j < 9; j++){
				result[i]+=kernel[j]*conv[j];
			}
		}
	}
	// Free Memory before finishing programme
	free(conv);
	return result;
}

////////////////////////////////
///       COMPUTATIONS       ///
////////////////////////////////

/**
 * Returns the sum of all elements.
 */
float get_sum(const float* matrix) {

	/*
		2 1
		1 2 => 6

		1 1
		1 1 => 4
	*/
	float sum = 0;
	// Add value of each elements of the matrix
	for(int y = 0; y < g_height; y++){
		for(int x = 0; x < g_width; x++){
			sum += matrix[y*g_width+x];
		}
	}

	return sum;
}

/**
 * Returns the trace of the matrix.
 */
float get_trace(const float* matrix) {

	/*
		1 0
		0 1 => 2

		2 1
		1 2 => 4
	*/

	float trace = 0;

	for(int y = 0; y < g_height; y++){
		for(int x = 0; x <g_width; x++){
			if(y == x){
				trace += matrix[y*g_width+x];
			}
		}
	}

	return trace;
}

/**
 * Returns the smallest value in the matrix.
 */
float get_minimum(const float* matrix) {

	/*
		1 2
		3 4 => 1

		4 3
		2 1 => 1
	*/

	// Set the smallest as the first of array
	float smallest = matrix[0];

	// If an element is smaller than smallest, return element as smallest
	for(int i = 0; i < g_elements; i++){
		if(matrix[i]<smallest){
			smallest = matrix[i];
		}
	}

	return smallest;
}

/**
 * Returns the largest value in the matrix.
 */
float get_maximum(const float* matrix) {

	/*
		1 2
		3 4 => 4

		4 3
		2 1 => 4
	*/
	// Set the largest as the first of array
	float largest = matrix[0];
	// If an element is larger than largest, return element as smallest
	for(int i = 0; i < g_elements; i++){
		if(matrix[i]>largest){
			largest = matrix[i];
		}
	}

	return largest;
}

// Method for calculating determinant for the matrix
float determinant(const float* matrix,ssize_t n){

  if(n == 1){ // 1D matrix
 		return matrix[0];
 	}
  else if (n == 2){ // 2D matrix
 		float det = matrix[0]*matrix[3]-matrix[1]*matrix[2];
    return det;
  }

 	float** m = malloc(n*sizeof(float*));
 	float det = 0.00;

  // FOR 3D or above Matrix
  for (int i1 = 0;i1 < n;i1++){
 		//create a reflect of i column of matrix
 		m[i1] = malloc(pow((g_elements-1),2)*sizeof(float));
 		// Populate the matrices of i column
 		for (int j = 1;j<n;j++){
 			for(int i2 = 0; i2 < n; i2++){
 				if (i1 > i2){
 					*(m[i1]+(j-1)*(n-1)+i2) = *(matrix+j*n+i2);
 					continue;
 				}
 				if (i2 > i1){
 					*(m[i1]+(j-1)*(n-1)+i2-1) = *(matrix+j*n+i2);
 				}
 			}
 		}

 		// Calculate the determinant
 		det = det + pow(-1,i1)*matrix[i1]* determinant(m[i1],n-1);
 	}

	// Release memory
	for(int i = 0; i < n; i++){
		free(m[i]);
	}

	free(m);
 	return det;
}

/**
 * Returns the determinant of the matrix.
 */
float get_determinant(const float* matrix) {

	/*
		1 0
		0 1 => 1

		1 2
		3 4 => -2

		8 0 2
		0 4 0
		2 0 8 => 240
	*/

	float det = 0.00;

	// Call Function 'determinant' and return result
	det = determinant(matrix,g_height);

	return det;
}

/**
 * Returns the frequency of the given value in the matrix.
 */
	ssize_t get_frequency(const float* matrix, float value) {

	/*
		1 1
		1 1 :: 1 => 4

		1 0
		0 1 :: 2 => 0
	*/

	// Number of Repeated number
	int count = 0;

	// Loop through the array then increment count.
	for(int i = 0; i < g_elements; i++){
		if(matrix[i] == value){
			count++;
		}
	}

	return count;
}
