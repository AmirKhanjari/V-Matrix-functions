#include <RcppArmadillo.h>
#include <omp.h>

// [[Rcpp::depends(RcppArmadillo)]]

using namespace Rcpp;
using namespace arma;

// [[Rcpp::plugins(openmp)]]
// [[Rcpp::export]]
mat Vmatrix_parallel(const mat &X, const mat &XT) {
  int n = X.n_rows;
  int nn = XT.n_rows;
  int m = X.n_cols;
  mat V(n, n, fill::zeros);
  
#pragma omp parallel for
  for (int i = 0; i < n; i++) {
    for (int j = i; j < n; j++) {
      mat d(nn, m);
      vec e(nn);
      
      for (int t = 0; t < nn; t++) {
        for (int k = 0; k < m; k++) {
          d(t, k) = XT(t, k) - std::max(X(i, k), X(j, k));
        }
      }
      
#pragma omp simd
      for (int t = 0; t < nn; t++) {
        e(t) = sum(d.row(t) >= 0);
      }
      
      V(i, j) = V(j, i) = sum(e);
    }
  }
  
  return V;
}
