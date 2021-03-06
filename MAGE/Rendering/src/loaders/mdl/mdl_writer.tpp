#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "loaders\mdl\mdl_tokens.hpp"
#include "loaders\msh\msh_loader.hpp"
#include "string\string_utils.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage::rendering::loader {

	template< typename VertexT, typename IndexT >
	MDLWriter< VertexT, IndexT >
		::MDLWriter(const ModelOutput< VertexT, IndexT >& model_output)
		: Writer(), 
		m_model_output(model_output) {}

	template< typename VertexT, typename IndexT >
	MDLWriter< VertexT, IndexT >::MDLWriter(MDLWriter&& writer) noexcept = default;

	template< typename VertexT, typename IndexT >
	MDLWriter< VertexT, IndexT >::~MDLWriter() = default;

	template< typename VertexT, typename IndexT >
	void MDLWriter< VertexT, IndexT >::Write() {
		// Export mesh.
		ExportMesh();
		
		// Export materials.
		WriteMaterials();
		
		// Export model.
		WriteModelParts();
	}

	template< typename VertexT, typename IndexT >
	void MDLWriter< VertexT, IndexT >::ExportMesh() {
		auto msh_path = GetPath();
		msh_path.replace_extension(L".msh");

		ExportMSHMeshToFile(msh_path, m_model_output.m_vertex_buffer,
			                          m_model_output.m_index_buffer);
	}

	template< typename VertexT, typename IndexT >
	void MDLWriter< VertexT, IndexT >::WriteMaterials() {
		auto mtl_path = GetPath();
		mtl_path.replace_extension(L".mtl");
		
		if (!std::filesystem::is_regular_file(mtl_path)) {
			return;
		}

		const auto mtl_fname = WStringToString(std::wstring(mtl_path.filename()));

		char buffer[MAX_PATH];
		const auto not_null_buffer = NotNull< const_zstring >(buffer);

		sprintf_s(buffer, std::size(buffer),
			      "%s %s",
			      g_mdl_token_material_library, 
				  mtl_fname.c_str());

		WriteStringLine(not_null_buffer);
	}

	template< typename VertexT, typename IndexT >
	void MDLWriter< VertexT, IndexT >::WriteModelParts() {
		char buffer[MAX_PATH];
		const auto not_null_buffer = NotNull< const_zstring >(buffer);

		for (const auto& model_part : m_model_output.m_model_parts) {

			sprintf_s(buffer, std::size(buffer),
				      "%s %s %s %f %f %f %f %f %f %f %f %f %s %u %u",
				      g_mdl_token_submodel, 
				      model_part.m_child.c_str(), 
				      model_part.m_parent.c_str(),
				      model_part.m_transform.GetTranslationX(),
				      model_part.m_transform.GetTranslationY(),
				      model_part.m_transform.GetTranslationZ(),
				      model_part.m_transform.GetRotationX(),
				      model_part.m_transform.GetRotationY(),
				      model_part.m_transform.GetRotationZ(),
				      model_part.m_transform.GetScaleX(),
				      model_part.m_transform.GetScaleY(),
				      model_part.m_transform.GetScaleZ(),
				      model_part.m_material.c_str(),
				      model_part.m_start_index, 
				      model_part.m_nb_indices);
			
			WriteStringLine(not_null_buffer);
		}
	}
}