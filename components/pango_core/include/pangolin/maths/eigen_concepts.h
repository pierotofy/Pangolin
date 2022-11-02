// Copyright (c) farm-ng, inc. All rights reserved.

#pragma once

#include <Eigen/Core>
#include <pangolin/utils/concept_utils.h>


namespace pangolin {


// These concept let us match Eigen's CRTP pattern and capture the true Derived
// type safely

template <typename Derived>
concept EigenType = DerivedFrom<Derived, Eigen::EigenBase<Derived>>;

template <typename Derived>
concept EigenDenseType = DerivedFrom<Derived, Eigen::DenseBase<Derived>>;

template <typename Derived>
concept EigenMatrixType =
    DerivedFrom<Derived, Eigen::MatrixBase<Derived>>;

template <typename Derived>
concept EigenArrayType = DerivedFrom<Derived, Eigen::ArrayBase<Derived>>;

template <typename T1, typename T2>
concept EigenSameDim = requires(T1, T2) {
  EigenDenseType<T1>&& EigenDenseType<T2> &&
      (T1::RowsAtCompileTime == Eigen::Dynamic ||
       T1::RowsAtCompileTime == T2::RowsAtCompileTime) &&
      (T1::ColsAtCompileTime == Eigen::Dynamic ||
       T1::ColsAtCompileTime == T2::ColsAtCompileTime);
};

template <int Rows, int Cols, typename T>
concept EigenWithDim = requires(T) {
  EigenDenseType<T>&& T::RowsAtCompileTime == Rows&& T::ColsAtCompileTime ==
      Cols;
};

template <int Rows, int Cols, typename T>
concept EigenWithDimOrDynamic = requires(T) {
  EigenDenseType<T> &&
      (T::RowsAtCompileTime == Eigen::Dynamic ||
       T::RowsAtCompileTime == Rows) &&
      (T::ColsAtCompileTime == Eigen::Dynamic || T::ColsAtCompileTime == Cols);
};

}  // namespace pangolin