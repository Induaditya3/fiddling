// continue from determinant of 4x4 matrix
#include <float.h>
#include <stdio.h>
#include <math.h>

// identity matrix 4x4
double ID4x4[4][4] = {
    {1, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 0, 1, 0},
    {0, 0, 0, 1}
};

// matrix multiplication
void mult(int rows, int cols, double m[rows][cols],
           int rows2, int cols2, double m2[rows2][cols2],
           double result[rows][cols2]){
  
  if (cols != rows2) return;

  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols2; j++){      
      for (int p = 0; p < cols; p++){
        result[i][j] += m[i][p]*m2[p][j]; 
      }
    }
  }
}
// comparing two double types
// returns 1 if equal and 0 otherwise
int dbl_equal(double a, double b){
  double epsilon = 0.00001;
  if (fabs(a-b) < epsilon) return 1;
  return 0;
}

// comparing two matrices of double type
// returns 1 if equal and 0 otherwise
int matrix_equal(int rows, int cols, double m[rows][cols], int rows2, int cols2, double m2[rows2][cols2]){
  // if dimension is not equal, it is nonsensical to comapare
  if (rows != rows2 || cols != cols2) return 0;

  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++)
      if (dbl_equal(m[i][j], m2[i][j]) == 0) return 0;
  }

  return 1;
}

// transpose of a matrix
void transpose(int rows, int cols, double m[rows][cols], double res[cols][rows]){
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++){
      res[j][i] = m[i][j];
    }
  }
}

// transpose of matrix in place
// valid only for 4x4
void transpose_in(double m[4][4]){
  double temp;
  for (int i = 0; i < 4; i++){
    for (int j = i+1; j < 4; j++){
      temp = m[i][j];
      m[i][j] = m[j][i];
      m[j][i] = temp;
    }
  }
}

// determinant of 2x2 matrix
static inline double det2x2(double m[2][2]) {
  return m[0][0]*m[1][1]-m[0][1]*m[1][0];
}

// determinant of 3x3 matrix
static inline double det3x3(double m[3][3]){
  // used to sarrus rule to do the expansion
  return (m[0][0]*m[1][1]*m[2][2] + m[0][1]*m[1][2]*m[2][0] + m[0][2]*m[1][0]*m[2][1]) - (m[0][2]*m[1][1]*m[2][0] + m[0][0]*m[1][2]*m[2][1] + m[0][1]*m[1][0]*m[2][2]);
}

//  3x3 matrix formed by deleting delx row and dely column from 4x4 matrix
static inline void submatrix4x4(double m[4][4], int delx, int dely, double res[3][3]) {
  for (int i = 0, x = 0; i < 3; i++, x++){
    for (int j = 0, y = 0; j < 3; j++, y++){
      if (x == delx) x++;
      if (y == dely) y++;
      res[i][j] = m[x][y];
    }
  }
}

//  2x2 matrix formed by deleting delx row and dely column from 3x3 matrix
static inline void submatrix3x3(double m[3][3], int delx, int dely, double res[2][2]) {
  for (int i = 0, x = 0; i < 2; i++, x++){
    for (int j = 0, y = 0; j < 2; j++, y++){
      if (x == delx) x++;
      if (y == dely) y++;
      res[i][j] = m[x][y];
    }
  }
}

// cofactor of 3x3 matrix
double cofactor3x3(double m[3][3], int delx, int dely) {
  double res[2][2];
  submatrix3x3(m, delx, dely, res);
  if ((delx+dely)%2)
    return -det2x2(res);
  return det2x2(res);
}

// cofactor of 4x4 matrix
double cofactor4x4(double m[4][4], int delx, int dely) {
  double res[3][3];
  submatrix4x4(m, delx, dely, res);
  if ((delx+dely)%2)
    return -det3x3(res);
  return det3x3(res);
}
// determinant of 4x4 matrix
// using cofactor method
double det4x4(double m[4][4]){
  double det = 0;
  for (int i = 0; i < 4; i++) {
    det += m[0][i]*cofactor4x4(m, 0, i);
  }  // FIX ME : start here
}

// print the matrix for debugging
void print_matrix(int rows, int cols, double m[rows][cols]){
  for (int i = 0; i < rows; i++){
    for (int j = 0; j < cols; j++) {
      printf("%4.0f", m[i][j]);
    }
    printf("\n");
  }
}
int main(){
  double A[3][3] = {
      {1,2,6},
      {-5,8,-4},
      {2, 6, 4}
  };

  printf("Cofactor of A at 0, 0 is %f\n", cofactor3x3(A, 0, 0));
  printf("Cofactor of A at 0,1 is %f\n",cofactor3x3(A, 0, 1));
  printf("Cofactor of A at 0, 2 is %f\n", cofactor3x3(A, 0, 2));
}
