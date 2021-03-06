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
	const T LineReader::Read() {
		if (!ContainsTokens()) {
			throw Exception("%ls: line %u: no value found.",
							GetPath().c_str(), GetCurrentLineNumber());
		}

		const auto token = GetCurrentToken();
		const auto first = NotNull< const char* >(token.data());
		const auto last  = NotNull< const char* >(token.data() + token.size());

		if (const auto result = StringTo< T >(first, last); 
		    bool(result)) {

			++m_iterator;
			return *result;
		}
		else {
			throw Exception("%ls: line %u: invalid value found: %s.",
							GetPath().c_str(), GetCurrentLineNumber(), 
							m_iterator->str().c_str());
		}
	}

	template<>
	inline const std::string_view LineReader::Read() {
		if (!ContainsTokens()) {
			throw Exception("%ls: line %u: no string value found.",
							GetPath().c_str(), GetCurrentLineNumber());
		}

		const auto result = GetCurrentToken();

		++m_iterator;
		return result;
	}

	template<>
	inline const std::string LineReader::Read() {
		return std::string(Read< std::string_view >());
	}

	template< typename T, size_t N, size_t A >
	inline const Array< T, N, A > LineReader::Read() {
		Array< T, N, A > result;
		for (auto& element : result) {
			element = Read< T >();
		}

		return result;
	}

	template< typename T >
	[[nodiscard]]
	inline bool LineReader::Contains() const noexcept {
		if (!ContainsTokens()) {
			return false;
		}

		const auto token = GetCurrentToken();
		const auto first = NotNull< const char* >(token.data());
		const auto last  = NotNull< const char* >(token.data() + token.size());

		return static_cast< bool >(StringTo< T >(first, last));
	}

	template<>
	[[nodiscard]]
	inline bool LineReader::Contains< std::string_view >() const noexcept {
		return ContainsTokens();
	}

	template<>
	[[nodiscard]]
	inline bool LineReader::Contains< std::string >() const noexcept {
		return Contains< std::string_view >();
	}
}