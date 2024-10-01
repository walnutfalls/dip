#pragma once

#include <cstdint>
#include <renderer/opengl_mesh.hpp>
#include <stdexcept>


namespace renderer
{
	struct opengl_model
	{
		static constexpr std::uint16_t MaxMeshCount = 256;
		
		opengl_mesh meshes[MaxMeshCount];
		std::uint16_t mesh_count{ 0 };

		void add_mesh(opengl_mesh mesh)
		{
			if (mesh_count == MaxMeshCount)
			{
				throw std::runtime_error("Mesh limit exceeded");
			}

			meshes[mesh_count++] = mesh;
		}
	};
}
