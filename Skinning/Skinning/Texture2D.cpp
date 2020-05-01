#include "Texture2D.h"

#include <stdexcept>
#include "lodepng.h"

Texture2D::Texture2D(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
	: m_device{ device }
	, m_deviceContext{ deviceContext }
{
}

Texture2D::~Texture2D()
{
	clear();
}

void Texture2D::load(const std::string & fileName)
{
	std::vector<unsigned char> image;
	auto error = lodepng::decode(image, m_width, m_height, fileName);
	if (error != 0)
	{
		throw std::runtime_error("can not open" + fileName);
	}
	clear();

	D3D11_TEXTURE2D_DESC texture2dDesc{};
	texture2dDesc.Width = m_width;
	texture2dDesc.Height = m_height;
	texture2dDesc.MipLevels = 1;
	texture2dDesc.ArraySize = 1;
	texture2dDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texture2dDesc.SampleDesc.Count = 1;
	texture2dDesc.SampleDesc.Quality = 0;
	texture2dDesc.Usage = D3D11_USAGE_DEFAULT;
	texture2dDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texture2dDesc.CPUAccessFlags = 0;
	texture2dDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA subrescorceData{};
	subrescorceData.pSysMem = image.data();
	subrescorceData.SysMemPitch = m_width * 4;
	subrescorceData.SysMemSlicePitch = m_width * m_height * 4;
	m_device->CreateTexture2D(&texture2dDesc, &subrescorceData, &m_texture);

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResouceViewDesc{};
	shaderResouceViewDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	shaderResouceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResouceViewDesc.Texture2D.MipLevels = 1;
	m_device->CreateShaderResourceView(m_texture, &shaderResouceViewDesc, &m_shaderResouceView);

	D3D11_SAMPLER_DESC samplerDesc{};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
	m_device->CreateSamplerState(&samplerDesc, &m_sampler);
}

void Texture2D::apply(UINT no)
{
	m_deviceContext->PSSetShaderResources(no, 1, &m_shaderResouceView);
	m_deviceContext->PSSetSamplers(no, 1, &m_sampler);
}

void Texture2D::clear()
{
	if (m_texture != nullptr)
	{
		m_texture->Release();
		m_texture = nullptr;
	}
	if (m_shaderResouceView != nullptr)
	{
		m_shaderResouceView->Release();
		m_shaderResouceView = nullptr;
	}
	if (m_sampler != nullptr)
	{
		m_sampler->Release();
		m_sampler = nullptr;
	}
}

unsigned int Texture2D::width() const
{
	return m_width;
}

unsigned int Texture2D::height() const
{
	return m_height;
}

