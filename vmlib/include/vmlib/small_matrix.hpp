#ifndef SMALL_MATRIX_HPP_7266454B_05D7_441E_8775_07E79DDBBE03
#define SMALL_MATRIX_HPP_7266454B_05D7_441E_8775_07E79DDBBE03


#include "small_vector.hpp"

/** SmallMat: NxM matrix
 *
 * See small_vector.hpp for discussion. Similar SmallVec, SmallMat is
 * intentionally kept simple and somewhat bare bones. It is also 
 * standard_layout and trivially_copyable, ensuring that we can easily
 * pass it to relevant C-based APIs such as OpenGL (which mostly just
 * expect a bunch of consecutive floats in memory).
 *
 * The matrix is stored in row-major order. Take care when passing it to 
 * OpenGL!
 *
 * The overloaded operator [] allows access to individual elements. Example:
 *    SmallMat<4,4,float> m = ...;
 *    float m12 = m[1,2];
 *    m[0,3] = 3.f;
 *
 * (Multi-dimensionsal subscripts in operator[] is a C++23 feature!)
 *
 * A 4x4 matrix would be arranged as:
 *
 *   ⎛ 0,0  0,1  0,2  0,3 ⎞
 *   ⎜ 1,0  1,1  1,2  1,3 ⎟
 *   ⎜ 2,0  2,1  2,2  2,3 ⎟
 *   ⎝ 3,0  3,1  3,2  3,3 ⎠
 */
template< std::size_t tM, std::size_t tN, typename tType = float >
struct SmallMat
{
	tType v[tM*tN];

	constexpr
	tType& operator[] (std::size_t aI, std::size_t aJ) noexcept
	{
		assert( aI < tM && aJ < tN );
		return v[aI*tN + aJ];
	}

	constexpr
	tType operator[] (std::size_t aI, std::size_t aJ) const noexcept
	{
		assert( aI < tM && aJ < tN );
		return v[aI*tN + aJ];
	};

	// Return pointer to the data
	// Use this e.g., for glUniformMatrix4fv( location, 1, matrix.data() );
	tType const* data() const noexcept
	{
		return v;
	}
};


// Operators
template< std::size_t tM, std::size_t tN, std::size_t tP, typename tType > constexpr
SmallMat<tM,tP,tType> operator*( SmallMat<tM,tN,tType> const& aLhs, SmallMat<tN,tP,tType> const& aRhs ) noexcept
{	
	SmallMat<tM, tP,tType> multiplied{}; //takes size of outcome

	for (std::size_t i = 0; i < tM; ++i){
		for (std::size_t j = 0; j < tP; ++j){
			for (std::size_t k = 0; k < tN; ++k){
				multiplied[i,j] += aLhs[i,k] * aRhs[k,j];
			}
		}
	}
	return multiplied; //TODO: implemented
}

template< std::size_t tM, std::size_t tN, typename tType > constexpr
SmallVec<tM,tType> operator*( SmallMat<tM,tN,tType> const& aLeft, SmallVec<tN,tType> const& aRight ) noexcept
{	
	SmallVec<tM,tType> multiplied{}; //takes size of outcome

	for (std::size_t i = 0; i < tM; ++i){
		for (std::size_t j = 0; j < tN; ++j){
			multiplied[i] += aLeft[i,j] * aRight[j];
			
		}
	}
	return multiplied; //TODO: implemented
}

// Functions
template< std::size_t tM, std::size_t tN, typename tType >
constexpr
SmallMat<tN,tM,tType> transpose( SmallMat<tM,tN,tType> const& aMat ) noexcept
{	
	//swap measurements
	SmallMat<tN, tM,tType> transposed{};

	for (std::size_t i = 0; i < tM; ++i){
		for (std::size_t j = 0; j < tN; ++j){
			transposed[j,i] = aMat[i,j];
		}
	}
	return transposed; //TODO: implemented
}

// Pseudo-constructor for identity matrices
// Usage example: auto identity33 = make_identity<Mat33f>();
// Only valid for square matrices.
template< class tMatrix > constexpr
tMatrix make_identity() noexcept
{
	// This uses an immediate invoked lambda with templates to get the matrix
	// size. It also enforces that we call this only on square matrices.
	tMatrix ret{};
	[] <std::size_t tN, typename tType> (SmallMat<tN,tN,tType>& aMat)
	{
		for( std::size_t i = 0; i < tN; ++i )
			aMat[i,i] = 1.f;
	}( ret );
	return ret;
}

// For Task 2.1 only:
template< std::size_t tN, typename tType, std::size_t tM > inline 
void gauss( SmallMat<tN,tN,tType> aMat, SmallMat<tN,tM,tType>& aResult ) noexcept
{
	// TODO-Task 2.1 only: implemented
	//gaussian elimination, pivoting thing.

	//Step 1, find row with largest abs value and swap with R1.

	for (std::size_t i= 0; i <tN; i++){
		
		std::size_t pivotRow = i;

		for (std::size_t row = i + 1; row < tN; row++){
			if (std::abs(aMat[row,i]) > std::abs(aMat[pivotRow,i])){
				//if smaller, update largest row finder, NEEDS TO CHECK FOR NEGATIVES`
				pivotRow = row;
			}

		}

		//now we know which row swaps with R1, we can start swapping
		if (pivotRow != i){
			for (std::size_t column = 0; column < tN; column++){
				auto temp = aMat[i, column];
				aMat[i, column] = aMat[pivotRow, column];
				aMat[pivotRow, column] = temp;
			}

			//again.
			for (std::size_t column = 0; column < tM; column++){
				auto temp = aResult[i, column];
				aResult[i, column] = aResult[pivotRow, column];
				aResult[pivotRow, column] = temp;
			}
			
		}

		//Get rid of things below pivot.


		for (std::size_t row = i + 1; row < tN; row++){
			//get factor
			auto factor = aMat[row,i]/ aMat[i,i];

			for (std::size_t column = i + 1; column < tN; column++){
				aMat[row,column] -= factor * aMat[i, column];
			}

			aMat[row,i] = tType{}; //zero

			for (std::size_t column = 0; column < tM; column++){
				aResult[row, column] -= factor * aResult[i, column];
			}
			
		}
	}
	//That SHOULD have now made a copy with swapped rows to aResult
	//step 2, start to work up.

	for (std::size_t i = tN; i-- > 0;){
		//pivot needs to be 1.

		auto pivot = aMat[i,i];

		for (std::size_t column = i+1; column < tN; column++){
			aMat[i,column] = aMat[i,column]/ pivot;
		}

		aMat[i,i]= tType{1};

		for (std::size_t column = 0; column < tM; column++){
			aResult[i,column] = aResult[i,column] /pivot;
		}

		//eliminations
		for (std::size_t row = 0; row < i; row++){
			auto factor = aMat[row, i];

			for (std::size_t column = i + 1; column < tN; column++){
				aMat[row, column] -= factor * aMat[i, column];
			}

			aMat[row, i] = tType{};

			for (std::size_t column = 0; column < tM; column++){
				aResult[row, column] -= factor * aResult[i, column];
			}
		}
	}

}

template< std::size_t tN, typename tType > // Must be square!
SmallMat<tN,tN,tType> invert( SmallMat<tN,tN,tType> const& aMat) noexcept{
	//inverting for matrices that arent only 4x4
	auto result = make_identity<SmallMat<tN,tN,tType>>();
	gauss(aMat, result);
	return result;
}

template<>
SmallMat<4,4,float> invert( SmallMat<4,4,float> const& ) noexcept;


#endif // SMALL_MATRIX_HPP_7266454B_05D7_441E_8775_07E79DDBBE03
