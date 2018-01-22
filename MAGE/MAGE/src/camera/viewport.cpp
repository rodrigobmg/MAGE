//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "camera\viewport.hpp"
#include "rendering\display_configuration.hpp"
#include "logging\error.hpp"
#include "exception\exception.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// Viewport
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]] const D3D11_VIEWPORT Viewport::GetMaxViewport() noexcept {
		const auto config = DisplayConfiguration::Get();
		
		return GetMaxViewport(config->GetDisplayWidth(), 
			                  config->GetDisplayHeight());
	}

	#pragma endregion

	//-------------------------------------------------------------------------
	// Viewport Transformations
	//-------------------------------------------------------------------------
	#pragma region

	[[nodiscard]] const XMMATRIX XM_CALLCONV 
		GetViewportTransform(ID3D11DeviceContext4 *device_context,
		                     DXGI_MODE_ROTATION rotation_mode) {
		
		Assert(device_context);
		
		U32 nb_of_viewports = 1u;
		D3D11_VIEWPORT viewport;
		Pipeline::RS::GetBoundViewports(device_context, 
			                            &nb_of_viewports, 
			                            &viewport);
		ThrowIfFailed((1u == nb_of_viewports), "No viewport is set.");

		return GetViewportTransform(viewport, rotation_mode);
	}

	[[nodiscard]] const XMMATRIX XM_CALLCONV 
		GetViewportTransform(ID3D11DeviceContext4 *device_context,
			                 D3D11_VIEWPORT *viewport,
		                     DXGI_MODE_ROTATION rotation_mode) {
		
		Assert(device_context);
		Assert(viewport);
		
		U32 nb_of_viewports = 1u;
		Pipeline::RS::GetBoundViewports(device_context, 
			                            &nb_of_viewports, 
			                            viewport);
		ThrowIfFailed((1u == nb_of_viewports), "No viewport is set.");

		return GetViewportTransform(*viewport, rotation_mode);
	}
	
	[[nodiscard]] const XMMATRIX XM_CALLCONV 
		GetViewportTransform(const D3D11_VIEWPORT &viewport,
		                     DXGI_MODE_ROTATION rotation_mode) noexcept {
		
		const auto scale_x = (viewport.Width  > 0.0f) ?
							 2.0f / viewport.Width  : 0.0f;
		const auto scale_y = (viewport.Height > 0.0f) ?
							 2.0f / viewport.Height : 0.0f;
		
		switch (rotation_mode) {
		
		case DXGI_MODE_ROTATION_ROTATE90:
			return XMMATRIX {
				 0.0f,    -scale_y, 0.0f, 0.0f,
				-scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				 1.0f,     1.0f,    0.0f, 1.0f
			};
		
		case DXGI_MODE_ROTATION_ROTATE180:
			return XMMATRIX {
				-scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     scale_y, 0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				 1.0f,    -1.0f,    0.0f, 1.0f
			};
		
		case DXGI_MODE_ROTATION_ROTATE270:
			return XMMATRIX {
				 0.0f,     scale_y, 0.0f, 0.0f,
				 scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				-1.0f,    -1.0f,    0.0f, 1.0f
			};
		
		default:
			return XMMATRIX {
				 scale_x,  0.0f,    0.0f, 0.0f,
				 0.0f,    -scale_y, 0.0f, 0.0f,
				 0.0f,     0.0f,    1.0f, 0.0f,
				-1.0f,     1.0f,    0.0f, 1.0f
			};
		}
	}

	#pragma endregion
}