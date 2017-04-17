#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "string\writer.hpp"
#include "collection\collection.hpp"
#include "scripting\variable.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	class VSWriter final : public Writer {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		explicit VSWriter(const vector< Variable * > &variable_buffer)
			: Writer(), m_variable_buffer(variable_buffer) {}
		VSWriter(const VSWriter &writer) = delete;
		VSWriter(VSWriter &&writer) = delete;
		virtual ~VSWriter() = default;

		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------	

		VSWriter &operator=(const VSWriter &writer) = delete;
		VSWriter &operator=(VSWriter &&writer) = delete;

	private:

		//---------------------------------------------------------------------
		// Member Methods
		//---------------------------------------------------------------------

		virtual HRESULT Write() override;

		//---------------------------------------------------------------------
		// Member Variables
		//---------------------------------------------------------------------

		const vector< Variable * > &m_variable_buffer;
	};
}