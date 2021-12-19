#include "Mimax_PCH.h"

#include "mimax/common/Matrix.h"

#include <algorithm>
#include <limits>
#include <string.h>
#include <type_traits>

using namespace std;

namespace mimax {
namespace common {

CMatrix CMatrix::Identity(size_t const size)
{
    CMatrix matrix(size, size);

    for (size_t i = 0; i < size; ++i)
    {
        matrix(i, i) = ScalarOne;
    }

    return matrix;
}

CMatrix CMatrix::Zeroes(size_t const rowsCnt, size_t const colsCnt)
{
    CMatrix matrix(rowsCnt, colsCnt);
    memset(matrix.m_data, 0, sizeof(Scalar) * matrix.size());
    return matrix;
}

CMatrix CMatrix::CreateRowVector(size_t const size)
{
    return CMatrix(1, size);
}

CMatrix CMatrix::CreateRowVector(initializer_list<Scalar> const& initList)
{
    return CMatrix({ initList });
}

CMatrix::CMatrix()
    : m_data(nullptr)
    , m_sizes{ 0, 0 }
    , m_capacity(0)
{
}

CMatrix::CMatrix(CMatrix const& other)
    : CMatrix()
{
    *this = other;
}

CMatrix::CMatrix(CMatrix&& other)
    : CMatrix()
{
    *this = move(other);
}

CMatrix::CMatrix(size_t const rowsCnt, size_t const colsCnt)
    : CMatrix()
{
    Resize(rowsCnt, colsCnt, false);
}

CMatrix::CMatrix(size_t const rowsCnt, size_t const colsCnt, Scalar const initValue)
    : CMatrix()
{
    Resize(rowsCnt, colsCnt, false);
    Fill(initValue);
}

CMatrix::CMatrix(initializer_list<initializer_list<Scalar>> const& initList)
    : CMatrix()
{
    size_t const rowsCnt = initList.size();
    size_t const colsCnt = rowsCnt > 0
        ? initList.begin()->size()
        : 0;

    Resize(rowsCnt, colsCnt, false);
    if (size() > 0)
    {
        CopyFrom(initList);
    }
}

CMatrix& CMatrix::operator=(CMatrix const& other)
{
    Resize(other.GetRowsCount(), other.GetColsCount(), false);
    CopyFrom(other);

    return *this;
}

CMatrix& CMatrix::operator=(CMatrix&& other)
{
    m_data = other.m_data;
    m_capacity = other.m_capacity;
    SetSizes(other.GetRowsCount(), other.GetColsCount());

    other.m_data = nullptr;
    other.m_capacity = 0;
    other.SetSizes(0, 0);

    return *this;
}

void CMatrix::Resize(size_t const rowsCnt, size_t const colsCnt, bool const copyDataIfReallocated)
{
    size_t const newSize = rowsCnt * colsCnt;
    ReserveAtLeast(newSize, copyDataIfReallocated);
    SetSizes(rowsCnt, colsCnt);
}

void CMatrix::Fill(Scalar const value)
{
    fill(begin(), end(), value);
}

void CMatrix::CopyFrom(initializer_list<initializer_list<Scalar>> const& initList)
{
    assert(initList.size() == GetRowsCount());

    auto dataRow = m_data;
    for (auto const& initRow : initList)
    {
        assert(initRow.size() == GetColsCount());

        memcpy(dataRow, initRow.begin(), sizeof(Scalar) * GetColsCount());
        dataRow += GetColsCount();
    }
}

void CMatrix::CopyFrom(CMatrix const& matrix)
{
    assert(GetRowsCount() == matrix.GetRowsCount());
    assert(GetColsCount() == matrix.GetColsCount());

    memcpy(m_data, matrix.m_data, sizeof(Scalar) * size());
}

void CMatrix::SetSizes(size_t const rowsCnt, size_t const colsCnt)
{
    bool const isEmpty = rowsCnt * colsCnt == 0;
    m_sizes[0] = isEmpty ? 0 : rowsCnt;
    m_sizes[1] = isEmpty ? 0 : colsCnt;
}

void CMatrix::ReserveAtLeast(size_t const capacity, bool const copyDataIfReallocated)
{
    if (m_capacity >= capacity) return;

    auto newData = AllocateData(capacity);
    if (copyDataIfReallocated && !empty())
    {
        memcpy(newData, m_data, sizeof(Scalar) * size());
    }
    FreeDataIfExists(m_data);
    m_data = newData;

    m_capacity = capacity;
}

CMatrix::Scalar* CMatrix::AllocateData(size_t const capacity)
{
    assert(capacity > 0);
    return new Scalar[capacity];
}

void CMatrix::FreeDataIfExists(Scalar* data)
{
    if (data)
    {
        delete[] data;
    }
}

CMatrix::~CMatrix()
{
    FreeDataIfExists(m_data);
}

bool CMatrix::operator==(CMatrix const& other) const
{
    if (GetRowsCount() != other.GetRowsCount() || GetColsCount() != other.GetColsCount())
    {
        return false;
    }

    return equal(begin(), end(), other.begin(), other.end(),
        [](Scalar const lhs, Scalar const rhs)
        {
            auto const maxNumber = max({ ScalarOne, (Scalar)fabs(lhs) , (Scalar)fabs(rhs) });
            return fabs(lhs - rhs) <= maxNumber * numeric_limits<Scalar>::epsilon();
        });
}

CMatrix CMatrix::operator+(CMatrix const& other) const
{
    assert(GetRowsCount() == other.GetRowsCount());
    assert(GetColsCount() == other.GetColsCount());

    CMatrix result(GetRowsCount(), GetColsCount());
    transform(begin(), end(), other.begin(), result.begin(),
        [](Scalar const lhs, Scalar const rhs)
        {
            return lhs + rhs;
        });

    return result;
}

CMatrix CMatrix::operator+(Scalar const value) const
{
    CMatrix result(GetRowsCount(), GetColsCount());
    transform(begin(), end(), result.begin(),
        [value](Scalar const matrixValue)
        {
            return matrixValue + value;
        });

    return result;
}

CMatrix CMatrix::operator-(CMatrix const& other) const
{
    assert(GetRowsCount() == other.GetRowsCount());
    assert(GetColsCount() == other.GetColsCount());

    CMatrix result(GetRowsCount(), GetColsCount());
    transform(begin(), end(), other.begin(), result.begin(),
        [](Scalar const lhs, Scalar const rhs)
        {
            return lhs - rhs;
        });

    return result;
}

CMatrix CMatrix::operator-(Scalar const value) const
{
    CMatrix result(GetRowsCount(), GetColsCount());
    transform(begin(), end(), result.begin(),
        [value](Scalar const matrixValue)
        {
            return matrixValue - value;
        });

    return result;
}

CMatrix CMatrix::operator*(CMatrix const& other) const
{
    assert(GetColsCount() == other.GetRowsCount());

    CMatrix result = Zeroes(GetRowsCount(), other.GetColsCount());
    auto resultRowData = result.begin();
    for (int lshRowIndex = 0; lshRowIndex < GetRowsCount(); ++lshRowIndex)
    {
        auto rhsRowData = other.begin();
        for (int lshColIndex = 0; lshColIndex < GetColsCount(); ++lshColIndex)
        {
            Scalar const lhsValue = operator()(lshRowIndex, lshColIndex);
            transform(rhsRowData, rhsRowData + other.GetColsCount(), resultRowData, resultRowData,
                [lhsValue](Scalar const rhsValue, Scalar const resultValue)
                {
                    return resultValue + lhsValue * rhsValue;
                });
            rhsRowData += other.GetColsCount();
        }
        resultRowData += result.GetColsCount();
    }

    return result;
}

CMatrix CMatrix::operator*(Scalar const value) const
{
    CMatrix result(GetRowsCount(), GetColsCount());
    transform(begin(), end(), result.begin(),
        [value](Scalar const matrixValue)
        {
            return matrixValue * value;
        });

    return result;
}

CMatrix CMatrix::AddToEachRow(CMatrix const& rowVectorMatrix) const
{
    auto result = *this;
    return result.AddToEachRowInPlace(rowVectorMatrix);
}

CMatrix& CMatrix::AddToEachRowInPlace(CMatrix const& rowVectorMatrix)
{
    assert(IsRowVector(rowVectorMatrix));
    assert(GetColsCount() == rowVectorMatrix.GetColsCount());

    auto rowDataIter = begin();
    while (rowDataIter != end())
    {
        transform(rowDataIter, rowDataIter + GetColsCount(), rowVectorMatrix.begin(), rowDataIter,
            [](Scalar const lhs, Scalar const rhs)
            {
                return lhs + rhs;
            });
        rowDataIter += GetColsCount();
    }

    return *this;
}

CMatrix CMatrix::Transpose() const
{
    CMatrix transposed = *this;
    transposed.SetSizes(GetColsCount(), GetRowsCount());

    if (transposed.GetColsCount() != 1 && transposed.GetRowsCount() != 1)
    {
        for (size_t rowIndex = 0; rowIndex < GetRowsCount(); ++rowIndex)
        {
            for (size_t colIndex = 0; colIndex < GetColsCount(); ++colIndex)
            {
                transposed(colIndex, rowIndex) = operator()(rowIndex, colIndex);
            }
        }
    }

    return transposed;
}

} // common
} // mimax