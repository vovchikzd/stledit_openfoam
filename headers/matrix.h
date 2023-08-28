#ifndef MATRIX_H
#define MATRIX_H

#include <memory>
#include <utility>
#include <stdexcept>
#include <initializer_list>

// TODO(#1): Iterators
// TODO(#3): Determinant function
// TODO(#5): Transpose method
// TODO(#6): Identity matrix method
// TODO(#7): Invertible matrix function

template <typename T, size_t Rows, size_t Columns,
         template <typename> class Allocator = std::allocator>
class Matrix {
private:

    const size_t cap = Rows * Columns;
    size_t sz = 0;
    Allocator<T> alloc;
    T* values = nullptr;

    using alloc_traits = std::allocator_traits<Allocator<T>>;

    void swap(Matrix<T, Rows, Columns, Allocator>& matrix) {
        std::swap(sz, matrix.sz);
        std::swap(values, matrix.values);
    }

public:

    Matrix(): values(alloc_traits::allocate(alloc, cap)) {}

    Matrix(std::initializer_list<T> list): Matrix() {
        for(auto it = list.begin(), end = list.end(); it != end; ++it) {
            push_back(*it);
        }
    }

    Matrix(const T* begin, const T* end): Matrix() {
        for (;begin != end; ++begin) {
            push_back(*begin);
        }
    }

    template <typename Iter>
    Matrix(Iter begin, Iter end): Matrix() {
        for (;begin != end; ++begin) {
            push_back(*begin);
        }
    }

    Matrix(const Matrix<T, Rows, Columns, Allocator>& matrix): Matrix() {
        for (size_t i = 0; i < matrix.size(); ++i) {
            push_back(matrix[i]);
        }
    }

    Matrix(Matrix<T, Rows, Columns, Allocator>&& matrix): Matrix() {
        sz = matrix.sz;
        values = matrix.values;
        matrix.sz = 0;
        matrix.values = nullptr;
    }

    void push_back(const T& value) {
        alloc_traits::construct(alloc, values + sz, value);
        ++sz;
    }

    void push_back(const T* begin, const T* end) {
        for (; begin != end; ++begin) {
            push_back(*begin);
        }
    }

    template <typename Iter>
    void push_back(Iter begin, Iter end) {
        for (; begin != end; ++begin) {
            push_back(*begin);
        }
    }
    
    size_t size() const {
        return sz;
    }

    bool empty() const {
        return sz == 0;
    }

    size_t capacity() const {
        return cap;
    }

    size_t rows() const {
        return Rows;
    }

    size_t columns() const {
        return Columns;
    }

    Matrix<T, Rows, Columns, Allocator>& operator=(const Matrix<T, Rows, Columns, Allocator>& matrix) {
        if (this != &matrix) {
            auto copy = matrix;
            swap(copy);
        }
        return *this;
    }

    Matrix<T, Rows, Columns, Allocator>& operator=(Matrix<T, Rows, Columns, Allocator>&& matrix) {
        if (this != &matrix) {
            sz = matrix.sz;
            values = matrix.values;
            matrix.sz = 0;
            matrix.values = nullptr;
        }
        return *this;
    }

    Matrix<T, Rows, Columns, Allocator>& operator+=(const Matrix<T, Rows, Columns, Allocator>& matrix) {
        for (size_t i = 0; i < sz; ++i) {
            values[i] += matrix[i];
        }
        return *this;
    }

    Matrix<T, Rows, Columns, Allocator>& operator-=(const Matrix<T, Rows, Columns, Allocator>& matrix) {
        for (size_t i = 0; i < sz; ++i) {
            values[i] -= matrix[i];
        }
        return *this;
    }

    Matrix<T, Rows, Columns, Allocator>& operator*=(const T& value) {
        for (size_t i = 0; i < sz; ++i) {
            values[i] *= value;
        }
        return *this;
    }

    T& operator[](size_t number) {
        return values[number];
    }

    const T& operator[](size_t number) const {
        return values[number];
    }

    T& operator()(size_t first, size_t second) {
        return values[second + (first * Columns)];
    }

    const T& operator()(size_t first, size_t second) const {
        return values[second + (first * Columns)];
    }

    T& at(size_t number) {
        if (number >= sz) throw std::out_of_range("Invalid index");        
        return values[number];
    }

    const T& at(size_t number) const {
        if (number >= sz) throw std::out_of_range("Invalid index");        
        return values[number];
    }

    T& at(size_t first, size_t second) {
        if (first >= Rows || second >= Columns) throw std::out_of_range("Invalid index");        
        return values[second + (first * Columns)];
    }

    const T& at(size_t first, size_t second) const {
        if (first >= Rows || second >= Columns) throw std::out_of_range("Invalid index");        
        return values[second + (first * Columns)];
    }

    ~Matrix() {
        for (size_t place = 0; place < sz; ++place) {
            alloc_traits::destroy(alloc, values + place);
        }
        alloc_traits::deallocate(alloc, values, cap);
    }
};

template <typename T, size_t Rows, size_t Columns, template <typename> class Alloc>
bool operator==(Matrix<T, Rows, Columns, Alloc>& left, Matrix<T, Rows, Columns, Alloc>& right) {
    for (size_t i = 0; i < left.size(); ++i) {
        if (left[i] != right[i]) return false;
    }
    return true;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator*(const Matrix<T, Rows, Columns, Allocator>& matrix,
        const T& value) {
    auto copy = matrix;
    copy *= value;
    return copy;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator*(const T& value,
        const Matrix<T, Rows, Columns, Allocator>& matrix) {
    auto copy = matrix;
    copy *= value;
    return copy;
}

template <typename T, size_t Rows, size_t K, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator*(const Matrix<T, Rows, K, Allocator>& left,
        const Matrix<T, K, Columns, Allocator>& right) {
    Matrix<T, Rows, Columns, Allocator> result;
    for (size_t row = 0; row < Rows; ++row) {
        for (size_t column = 0; column < Columns; ++column) {
            T sum = 0;
            for (size_t k = 0; k < K; ++k) {
                sum += left(row, k) * right(k, column);
            }
            result.push_back(sum);
        }
    }
    return result;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator+(const Matrix<T, Rows, Columns, Allocator>& left,
        const Matrix<T, Rows, Columns, Allocator>& right) {
    auto copy = left;
    copy += right;
    return copy;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator-(const Matrix<T, Rows, Columns, Allocator>& left,
        const Matrix<T, Rows, Columns, Allocator>& right) {
    auto copy = left;
    copy -= right;
    return copy;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator+(Matrix<T, Rows, Columns, Allocator>& matrix) {
    return matrix;
}

template <typename T, size_t Rows, size_t Columns, template <typename> class Allocator>
Matrix<T, Rows, Columns, Allocator> operator-(Matrix<T, Rows, Columns, Allocator>& matrix) {
    auto copy = matrix;
    copy *= -1;
    return copy;
}

template <typename T, size_t Size, template <typename> class Allocator>
T determinant(Matrix<T, Size, Size, Allocator>& matrix) {
    // implement
}

#endif
