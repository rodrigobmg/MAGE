#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\msh\msh_tokens.hpp"
#include "exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering::loader {

	template< typename VertexT, typename IndexT >
	MSHReader< VertexT, IndexT >
		::MSHReader(std::vector< VertexT >& vertices, 
		            std::vector< IndexT >& indices)
		: BigEndianBinaryReader(), 
		m_vertices(vertices), 
		m_indices(indices) {}

	template< typename VertexT, typename IndexT >
	MSHReader< VertexT, IndexT >::MSHReader(
		MSHReader&& reader) noexcept = default;

	template< typename VertexT, typename IndexT >
	MSHReader< VertexT, IndexT >::~MSHReader() = default;

	template< typename VertexT, typename IndexT >
	void MSHReader< VertexT, IndexT >::ReadData() {

		// Read the header.
		{
			const bool result = IsHeaderValid();
			ThrowIfFailed(result, 
						  "%ls: invalid mesh header.", GetFilename().c_str());
		}

		const auto nb_vertices = Read< U32 >();
		const auto nb_indices  = Read< U32 >();
		
		const auto vertices = ReadArray< VertexT >(nb_vertices);
		m_vertices.assign(vertices, vertices + nb_vertices);

		const auto indices   = ReadArray< IndexT >(nb_indices);
		m_indices.assign(indices, indices + nb_indices);
	}

	template< typename VertexT, typename IndexT >
	[[nodiscard]]
	bool MSHReader< VertexT, IndexT >::IsHeaderValid() {
		for (auto magic = g_font_token_magic; *magic != L'\0'; ++magic) {
			if (*magic != Read< U8 >()) {
				return false;
			}
		}

		return true;
	}
}