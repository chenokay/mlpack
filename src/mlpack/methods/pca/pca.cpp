/**
 * @file pca.cpp
 * @author Ajinkya Kale
 *
 * Implementation of PCA class to perform Principal Components Analysis on the
 * specified data set.
 */
#include "pca.hpp"
#include <mlpack/core.hpp>
#include <iostream>

using namespace std;
namespace mlpack {
namespace pca {

PCA::PCA(const bool centerData, const bool scaleData) :
      centerData_(centerData),
      scaleData_(scaleData)
{
}

/**
 * Apply Principal Component Analysis to the provided data set.
 *
 * @param data - Data matrix
 * @param transformedData - Data with PCA applied
 * @param eigVal - contains eigen values in a column vector
 * @param coeff - PCA Loadings/Coeffs/EigenVectors
 */
void PCA::Apply(const arma::mat& data, arma::mat& transformedData,
           arma::vec& eigVal, arma::mat& coeffs)
{
  arma::mat transData = trans(data);

  if(centerData_)
  {
    arma::rowvec means = arma::mean(transData, 0);
    transData = transData - arma::ones<arma::colvec>(transData.n_rows) * means;
  }

  if(scaleData_)
  {
    transData = transData / (arma::ones<arma::colvec>(transData.n_rows) * stddev(transData, 0, 0));
  }

  arma::mat covMat = cov(transData);
  arma::eig_sym(eigVal, coeffs, covMat);

  int n_eigVal = eigVal.n_elem;
  for(int i = 0; i < floor(n_eigVal / 2); i++)
    eigVal.swap_rows(i, (n_eigVal - 1) - i);

  coeffs = arma::fliplr(coeffs);
  transformedData = trans(coeffs) * data;
  arma::colvec transformedDataMean = arma::mean(transformedData, 1);
  transformedData = transformedData - (transformedDataMean * arma::ones<arma::rowvec>(transformedData.n_cols));
}

/**
 * Apply Principal Component Analysis to the provided data set.
 *
 * @param data - Data matrix
 * @param transformedData - Data with PCA applied
 * @param eigVal - contains eigen values in a column vector
 */
void PCA::Apply(const arma::mat& data, arma::mat& transformedData,
           arma::vec& eigVal)
{
  arma::mat coeffs;
  Apply(data, transformedData,
              eigVal, coeffs);
}

/**
 * Apply Dimensionality Reduction using Principal Component Analysis
 * to the provided data set.
 *
 * @param data - M x N Data matrix
 * @param newDimension - matrix consisting of N column vectors,
 * where each vector is the projection of the corresponding data vector
 * from data matrix onto the basis vectors contained in the columns of
 * coeff/eigen vector matrix with only newDimension number of columns chosen.
 */
void PCA::Apply(arma::mat& data, const int newDimension)
{
  arma::mat coeffs;
  arma::vec eigVal;

  Apply(data, data, eigVal, coeffs);

  if(newDimension < coeffs.n_rows && newDimension > 0)
    data.shed_rows(newDimension, data.n_rows - 1);
}

PCA::~PCA()
{
}

}; // namespace mlpack
}; // namespace pca
