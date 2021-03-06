#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	template< typename T >
	const T BinaryReader::Read() {
		const auto old_pos = m_pos;
		const auto new_pos = m_pos + sizeof(T);
		
		ThrowIfFailed((m_pos <= new_pos), 
					  "%ls: overflow: no value found.", 
					  GetPath().c_str());
		ThrowIfFailed((new_pos <= m_end), 
					  "%ls: end of file: no value found.", 
					  GetPath().c_str());

		m_pos = new_pos;
		return BytesTo< T >(old_pos, m_big_endian);
	}

	template< typename T >
	inline const T BigEndianBinaryReader::Read() {
		return *ReadArray< T >(1);
	}

	template< typename T >
	const T* BigEndianBinaryReader::ReadArray(size_t count) {
		const auto old_pos = m_pos;
		const auto new_pos = m_pos + sizeof(T) * count;
		
		ThrowIfFailed((m_pos <= new_pos), 
					  "%ls: overflow: no %llu values found.", 
					  GetPath().c_str(), 
					  static_cast< U64 >(count));
		ThrowIfFailed((new_pos <= m_end), 
					  "%ls: end of file: no %llu values found.", 
					  GetPath().c_str(), 
					  static_cast< U64 >(count));

		m_pos = new_pos;
		return reinterpret_cast< const T* >(old_pos);
	}
}