//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "texture\texture.hpp"
#include "texture\texture_loader.hpp"
#include "texture\texture_utils.hpp"
#include "logging\error.hpp"
#include "logging\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	Texture::Texture(const wstring &fname)
		: Texture(fname, GetDevice()) {}

	Texture::Texture(const wstring &fname, ID3D11Device2 *device)
		: Resource< const Texture >(fname), m_texture_srv() {

		Assert(device);

		ImportTextureFromFile(GetFilename(), device, m_texture_srv.ReleaseAndGetAddressOf());
	}

	Texture::Texture(const wstring &guid,
		const D3D11_TEXTURE2D_DESC *desc,
		const D3D11_SUBRESOURCE_DATA *initial_data)
		: Texture(guid, GetDevice(), desc, initial_data) {}

	Texture::Texture(const wstring &guid, ID3D11Device2 *device,
		const D3D11_TEXTURE2D_DESC *desc, const D3D11_SUBRESOURCE_DATA *initial_data)
		: Resource< const Texture >(guid), m_texture_srv() {

		Assert(device);

		ComPtr< ID3D11Texture2D > texture;
		const HRESULT result_resource = device->CreateTexture2D(
			desc, initial_data, texture.ReleaseAndGetAddressOf());
		if (FAILED(result_resource)) {
			throw FormattedException("Texture 2D creation failed: %08X.", result_resource);
		}

		const HRESULT result_srv = device->CreateShaderResourceView(
			texture.Get(), nullptr, m_texture_srv.ReleaseAndGetAddressOf());
		if (FAILED(result_srv)) {
			throw FormattedException("Texture SRV creation failed: %08X.", result_srv);
		}
	}

	bool Texture::HasAlpha() const noexcept {
		return mage::HasAlpha(m_texture_srv.Get());
	}
}