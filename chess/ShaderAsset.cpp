#include "ShaderAsset.h"

ShaderFactory::ShaderFactory(const std::string & shaderPath, sf::Shader::Type shaderType) :
	m_asset(nullptr)
{
	switch (shaderType)
	{
	case sf::Shader::Vertex:
		m_vertexShaderPath = shaderPath;
		break;
	case sf::Shader::Geometry:
		break;
	case sf::Shader::Fragment:
		m_fragmentShaderPath = shaderPath;
		break;
	default:
		break;
	}
}

ShaderFactory::ShaderFactory(const std::string & vertexShaderPath, const std::string & fragmentShaderPath) :
	m_vertexShaderPath(vertexShaderPath), m_fragmentShaderPath(fragmentShaderPath), m_asset(nullptr)
{
}

ShaderFactory::~ShaderFactory()
{
	if (m_asset != nullptr) {
		Log::write("shader deleted: \"" + m_vertexShaderPath + " " + m_fragmentShaderPath + "\"");
	}
}

Asset * ShaderFactory::load()
{
	if (m_asset == nullptr) {
		std::shared_ptr<ShaderAsset> asset = std::make_shared<ShaderAsset>();
		bool success = false;
		std::string message;

		bool hasVertexShader = m_vertexShaderPath != "";
		bool hasFragmentShader = m_fragmentShaderPath != "";

		if (!hasVertexShader && hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_fragmentShaderPath, sf::Shader::Fragment);
			message = "unable to load fragment shader: \"" + m_fragmentShaderPath + "\"";
		}
		else if (hasVertexShader && !hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_vertexShaderPath, sf::Shader::Vertex);
			message = "unable to load vertex shader: \"" + m_vertexShaderPath + "\"";
		}
		else if (hasVertexShader && hasFragmentShader) {
			success = asset->m_shader.loadFromFile(m_vertexShaderPath, m_fragmentShaderPath);
			message = "unable to load shaders: \"" + m_vertexShaderPath + "\" and \"" + m_fragmentShaderPath + "\"";
		}
		else {
			message = "unable to load unknown shader";
		}

		if (success) {
			m_asset = std::move(asset);
			Log::write("shader loaded: \"" + m_vertexShaderPath + " " + m_fragmentShaderPath + "\"");
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
		Log::write("shader deleted: \"" + m_vertexShaderPath + " " + m_fragmentShaderPath + "\"");
	}
	m_asset.reset();
}
