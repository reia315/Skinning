#pragma once

#include <string>
#include <d3d11.h>

class Texture2D
{
public:

	Texture2D(ID3D11Device* device, ID3D11DeviceContext* deviceContext);

	~Texture2D();

	void load(const std::string& fileName);

	void apply(UINT no = 0);

	void clear();

	unsigned int width() const;

	unsigned int height() const;

	Texture2D(const Texture2D& other) = delete;
	Texture2D operator = (const Texture2D& other) = delete;

private:

	ID3D11Device* m_device{ nullptr };
	ID3D11DeviceContext* m_deviceContext{ nullptr };
	ID3D11Texture2D* m_texture{ nullptr };
	ID3D11ShaderResourceView* m_shaderResouceView{ nullptr };
	ID3D11SamplerState* m_sampler{ nullptr };
	unsigned int m_width;
	unsigned int m_height;
};