#include <render/additional_material_property.hpp>
#include <render/program_uniforms.hpp>

void render::additional_material_property_f::bind_property(const render::program_uniforms& uniforms) const {
	gl::glUniform1f(uniforms.find_location(property_name_hash), value);
}
