#pragma once

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "string\line_reader.hpp"
#include "model\model_output.hpp"
#include "mesh\mesh_descriptor.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Engine Declarations and Definitions
//-----------------------------------------------------------------------------
namespace mage {

	template < typename Vertex >
	class OBJReader : public LineReader {

	public:

		OBJReader(ModelOutput< Vertex > &model_output, const MeshDescriptor &mesh_desc)
			: LineReader(), m_model_output(model_output), m_mesh_desc(mesh_desc) {}
		virtual ~OBJReader() = default;

	protected:

		virtual HRESULT Preprocess() override;
		virtual HRESULT ReadLine(char *line) override;
		virtual HRESULT Postprocess() override;

		void ReadOBJMaterialLibrary();
		void ReadOBJMaterialUse();
		void ReadOBJGroup();
		void ReadOBJObject();
		void ReadOBJVertex();
		void ReadOBJVertexTexture();
		void ReadOBJVertexNormal();
		void ReadOBJTriangleFace();

		Point3 ReadOBJVertexCoordinates();
		Normal3 ReadOBJVertexNormalCoordinates();
		UV ReadOBJVertexTextureCoordinates();
		XMUINT3 ReadOBJVertexIndices();
		
		Vertex ConstructVertex(const XMUINT3 &vertex_indices);

	private:

		/**
		 A struct of @c XMUINT3 comparators for OBJ vertex indices.
		 */
		struct OBJComparatorXMUINT3 {

		public:

			/**
			 Compares the two given @c XMUINT3 vectors against each other.

			 @param[in]		a
							A reference to the first vector.
			 @param[in]		b
							A reference to the second vector.
			 @return		@c true if the @a a is smaller than @a b.
							@c false otherwise.
			 */
			bool operator()(const XMUINT3& a, const XMUINT3& b) const {
				return (a.x == b.x) ? ((a.y == b.y) ? (a.z < b.z) : (a.y < b.y)) : (a.x < b.x);
			}
		};

		struct OBJBuffer {

		public:

			vector< Point3 >  vertex_coordinates;
			vector< UV >      vertex_texture_coordinates;
			vector< Normal3 > vertex_normal_coordinates;
			map< XMUINT3, uint32_t, OBJComparatorXMUINT3 > mapping;
		};

		OBJReader(const OBJReader &reader) = delete;
		OBJReader &operator=(const OBJReader &reader) = delete;

		OBJBuffer m_buffer;
		ModelOutput< Vertex > &m_model_output;
		const MeshDescriptor &m_mesh_desc;
	};
}

//-----------------------------------------------------------------------------
// Engine Includes
//-----------------------------------------------------------------------------
#pragma region

#include "mesh\obj\obj_reader.tpp"

#pragma endregion