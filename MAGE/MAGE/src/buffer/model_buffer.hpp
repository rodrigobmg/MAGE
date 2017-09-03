#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "memory\allocation.hpp"
#include "material\spectrum.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	//-------------------------------------------------------------------------
	// ModelBuffer
	//-------------------------------------------------------------------------

	/**
	 A struct of model buffers used by pixel shaders.
	 */
	_declspec(align(16)) struct ModelBuffer final : public AlignedData< ModelBuffer > {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a model buffer.
		 */
		ModelBuffer()
			: m_object_to_view{}, 
			m_normal_to_view{}, 
			m_texture_transform{},
			m_Kd{}, m_dissolve(0.0f),
			m_Ks{}, m_material_coefficients{} {}

		/**
		 Constructs a model buffer from the given model buffer.

		 @param[in]		buffer
						A reference to the model buffer to copy.
		 */
		ModelBuffer(const ModelBuffer &buffer) = default;
		
		/**
		 Constructs a model buffer by moving the given model buffer.

		 @param[in]		buffer
						A reference to the model buffer to move.
		 */
		ModelBuffer(ModelBuffer &&buffer) = default;

		/**
		 Destructs this model buffer.
		 */
		~ModelBuffer() = default;
		
		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------
		
		/**
		 Copies the given model buffer to this model buffer.

		 @param[in]		buffer
						A reference to the model buffer to copy.
		 @return		A reference to the copy of the given model buffer
						(i.e. this model buffer).
		 */
		ModelBuffer &operator=(const ModelBuffer &buffer) = default;

		/**
		 Moves the given model buffer to this model buffer.

		 @param[in]		buffer
						A reference to the model buffer to move.
		 @return		A reference to the moved model buffer
						(i.e. this model buffer).
		 */
		ModelBuffer &operator=(ModelBuffer &&buffer) = default;

		//---------------------------------------------------------------------
		// Assignment Operators: Transforms
		//---------------------------------------------------------------------

		// HLSL expects column-major packed matrices by default.
		// DirectXMath expects row-major packed matrices.

		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view matrix of this model buffer for use in HLSL.
		 */
		XMMATRIX m_object_to_view;
		
		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view inverse tranpose matrix (normal-to-view matrix) 
		 of this model buffer for use in HLSL.
		 */
		XMMATRIX m_normal_to_view;

		/**
		 The (object dependent) (column-major packed, row-major matrix) 
		 texture transform matrix of this model buffer for use in HLSL.
		 */
		XMMATRIX m_texture_transform;

		//---------------------------------------------------------------------
		// Assignment Operators: Material
		//---------------------------------------------------------------------

		/**
		 The diffuse reflectivity of this model buffer.
		 */
		RGBSpectrum m_Kd;

		/**
		 The dissolve factor (i.e. opacity) of this model buffer.
		 */
		float m_dissolve;

		/**
		 The specular reflectivity of this model buffer.
		 */
		RGBSpectrum m_Ks;

		/**
		 The BRDF dependent material coefficients
		 of this model buffer.
		 */
		float m_material_coefficients[2];
	};

	static_assert(sizeof(ModelBuffer) == 240, "CPU/GPU struct mismatch");

	//-------------------------------------------------------------------------
	// ModelTransformBuffer
	//-------------------------------------------------------------------------

	/**
	 A struct of model transform buffers used by pixel shaders.
	 */
	_declspec(align(16)) struct ModelTransformBuffer final : public AlignedData< ModelTransformBuffer > {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a model transform buffer.
		 */
		ModelTransformBuffer()
			: m_object_to_view{},
			m_normal_to_view{},
			m_texture_transform{} {}

		/**
		 Constructs a model transform buffer from the given model transform buffer.

		 @param[in]		buffer
						A reference to the model transform buffer to copy.
		 */
		ModelTransformBuffer(const ModelTransformBuffer &buffer) = default;
		
		/**
		 Constructs a model transform buffer by moving the given model transform buffer.

		 @param[in]		buffer
						A reference to the model transform buffer to move.
		 */
		ModelTransformBuffer(ModelTransformBuffer &&buffer) = default;

		/**
		 Destructs this model transform buffer.
		 */
		~ModelTransformBuffer() = default;
		
		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------
		
		/**
		 Copies the given model transform buffer to this model transform buffer.

		 @param[in]		buffer
						A reference to the model transform buffer to copy.
		 @return		A reference to the copy of the given model transform buffer
						(i.e. this model transform buffer).
		 */
		ModelTransformBuffer &operator=(const ModelTransformBuffer &buffer) = default;

		/**
		 Moves the given model transform buffer to this model transform buffer.

		 @param[in]		buffer
						A reference to the model transform buffer to move.
		 @return		A reference to the moved model transform buffer
						(i.e. this model transform buffer).
		 */
		ModelTransformBuffer &operator=(ModelTransformBuffer &&buffer) = default;

		//---------------------------------------------------------------------
		// Assignment Operators: Transforms
		//---------------------------------------------------------------------

		// HLSL expects column-major packed matrices by default.
		// DirectXMath expects row-major packed matrices.

		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view matrix of this model buffer for use in HLSL.
		 */
		XMMATRIX m_object_to_view;
		
		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view inverse tranpose matrix (normal-to-view matrix) 
		 of this model buffer for use in HLSL.
		 */
		XMMATRIX m_normal_to_view;

		/**
		 The (object dependent) (column-major packed, row-major matrix) 
		 texture transform matrix of this model buffer for use in HLSL.
		 */
		XMMATRIX m_texture_transform;
	};

	static_assert(sizeof(ModelTransformBuffer) == 192, "CPU/GPU struct mismatch");

	//-------------------------------------------------------------------------
	// DeferredModelBuffer
	//-------------------------------------------------------------------------

	/**
	 A struct of deferred model buffers used by pixel shaders.
	 */
	_declspec(align(16)) struct DeferredModelBuffer final : public AlignedData< DeferredModelBuffer > {

	public:

		//---------------------------------------------------------------------
		// Constructors and Destructors
		//---------------------------------------------------------------------

		/**
		 Constructs a deferred model buffer.
		 */
		DeferredModelBuffer()
			: m_object_to_view{}, 
			m_normal_to_view{}, 
			m_texture_transform{},
			m_Kd{}, m_mat2_norm(0.0f),
			m_Ks{}, m_mat1_norm(0.0f) {}

		/**
		 Constructs a deferred model buffer from the given deferred model buffer.

		 @param[in]		buffer
						A reference to the deferred model buffer to copy.
		 */
		DeferredModelBuffer(const DeferredModelBuffer &buffer) = default;
		
		/**
		 Constructs a deferred model buffer by moving the given deferred model buffer.

		 @param[in]		buffer
						A reference to the deferred model buffer to move.
		 */
		DeferredModelBuffer(DeferredModelBuffer &&buffer) = default;

		/**
		 Destructs this deferred model buffer.
		 */
		~DeferredModelBuffer() = default;
		
		//---------------------------------------------------------------------
		// Assignment Operators
		//---------------------------------------------------------------------
		
		/**
		 Copies the given deferred model buffer to this deferred model buffer.

		 @param[in]		buffer
						A reference to the deferred model buffer to copy.
		 @return		A reference to the copy of the given deferred model buffer
						(i.e. this deferred model buffer).
		 */
		DeferredModelBuffer &operator=(const DeferredModelBuffer &buffer) = default;

		/**
		 Moves the given deferred model buffer to this deferred model buffer.

		 @param[in]		buffer
						A reference to the deferred model buffer to move.
		 @return		A reference to the moved deferred model buffer
						(i.e. this deferred model buffer).
		 */
		DeferredModelBuffer &operator=(DeferredModelBuffer &&buffer) = default;

		//---------------------------------------------------------------------
		// Assignment Operators: Transforms
		//---------------------------------------------------------------------

		// HLSL expects column-major packed matrices by default.
		// DirectXMath expects row-major packed matrices.

		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view matrix of this deferred model buffer for use in HLSL.
		 */
		XMMATRIX m_object_to_view;
		
		/**
		 The (camera dependent, object dependent) (column-major packed, row-major matrix) 
		 object-to-view inverse tranpose matrix (normal-to-view matrix) 
		 of this deferred model buffer for use in HLSL.
		 */
		XMMATRIX m_normal_to_view;

		/**
		 The (object dependent) (column-major packed, row-major matrix) 
		 texture transform matrix of this deferred model buffer for use in HLSL.
		 */
		XMMATRIX m_texture_transform;

		//---------------------------------------------------------------------
		// Assignment Operators: Material
		//---------------------------------------------------------------------

		/**
		 The diffuse reflectivity of this deferred model buffer.
		 */
		RGBSpectrum m_Kd;

		/**
		The 2nd BRDF dependent normalized material coefficient
		of this deferred model buffer.
		 */
		float m_mat2_norm;

		/**
		 The specular reflectivity of this deferred model buffer.
		 */
		RGBSpectrum m_Ks;

		/**
		 The 1st BRDF dependent normalized material coefficient
		 of this deferred model buffer.
		 */
		float m_mat1_norm;
	};

	static_assert(sizeof(DeferredModelBuffer) == 224, "CPU/GPU struct mismatch");
}