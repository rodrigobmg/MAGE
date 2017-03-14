#pragma once

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	struct SpriteFontDescriptor final {

	public:

		SpriteFontDescriptor(bool force_srgb = false)
			: m_force_srgb(force_srgb) {}
		SpriteFontDescriptor(const SpriteFontDescriptor &desc) = default;
		~SpriteFontDescriptor() = default;

		SpriteFontDescriptor &operator=(const SpriteFontDescriptor &desc) = default;

		bool ForceSRGB() const {
			return m_force_srgb;
		}

	private:

		bool m_force_srgb;
	};
}