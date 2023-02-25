#include <RcppArmadillo.h>
#include <omp.h>
using namespace Rcpp;
using namespace arma;

// [[Rcpp::depends(RcppArmadillo)]]

// [[Rcpp::export]]
mat VG(const mat &T, const mat &X, const double &sigma) {
  int m = X.n_rows;
  int b = T.n_rows;
  int d = X.n_cols;
  mat V = zeros<mat>(m, m);
  
#pragma omp parallel for
  for (int i = 0; i < m; i++) {
    for (int j = i; j < m; j++) {
      for (int k = 0; k < b; k++) {
        for (int h = 0; h < d; h++) {
          V(i, j) += exp(-pow(T(k, h) - X(i, h), 2) / pow(sigma, 2)) *
            exp(-pow(T(k, h) - X(j, h), 2) / pow(sigma, 2));
        }
      }
      V(j, i) = V(i, j);
    }
  }
  return V;
}
