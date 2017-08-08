#include "ShaderAsset.h"

ShaderFactory::ShaderFactory(const std::string & shader_path, sf::Shader::Type shader_type) :
	m_asset(nullptr)
{
	switch (shader_type)
	{
	case sf::Shader::Vertex:
		m_vertex_shader_path = shader_path;
		break;
	case sf::Shader::Geometry:
		break;
	case sf::Shader::Fragment:
		m_fragment_shader_path = shader_path;
		break;
	default:
		break;
	}
}

ShaderFactory::ShaderFactory(const std::string & vertex_shader_path, const std::string & fragment_shader_path) :
	m_vertex_shader_path(vertex_shader_path), m_fragment_shader_path(fragment_shader_path), m_asset(nullptr)
{
}

ShaderFactory::~ShaderFactory()
{
	if (m_asset != nullptr) {
		Log::write("shader deleted: \"" + m_vertex_shader_path + " " + m_fragment_shader_path + "\"");
	}
}

Asset * ShaderFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<ShaderAsset> asset = std::make_shared<ShaderAsset>();
		bool success = false;
		std::string message;

		bool has_vertex_shader = m_vertex_shader_path != "";
		bool hasFragmentShader = m_fragment_shader_path != "";

		if (!has_vertex_shader && hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_fragment_shader_path, sf::Shader::Fragment);
			message = "unable to load fragment shader: \"" + m_fragment_shader_path + "\"";
		}
		else if (has_vertex_shader && !hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_vertex_shader_path, sf::Shader::Vertex);
			message = "unable to load vertex shader: \"" + m_vertex_shader_path + "\"";
		}
		else if (has_vertex_shader && hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_vertex_shader_path, m_fragment_shader_path);
			message = "unable to load shaders: \"" + m_vertex_shader_path + "\" and \"" + m_fragment_shader_path + "\"";
		}
		else {
			message = "unable to load unknown shader";
		}

		if (success) {
			m_asset = std::move(asset);
			Log::write("shader loaded: \"" + m_vertex_shader_path + " " + m_fragment_shader_path + "\"");
		}
		else {
			throw std::runtime_error(message);
		}
	}

	return m_asset.get();
}

void ShaderFactory::clear()
{
	if (m_asset != nullptr) {
		Log::write("shader deleted: \"" + m_vertex_shader_path + " " + m_fragment_shader_path + "\"");
	}
	m_asset.reset();
}
