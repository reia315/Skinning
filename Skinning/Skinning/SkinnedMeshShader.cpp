#include "SkinnedMeshShader.h"


#include <d3dcompiler.h>
#include <memory>
#include <filesystem>

SkinnedMeshShader::SkinnedMeshShader(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
	: m_device{ device }
	, m_deviceContext{ deviceContext }
{
	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.ByteWidth = sizeof(BufferData);
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	m_device->CreateBuffer(&bufferDesc, NULL, &m_constantBuffer);
}

SkinnedMeshShader::~SkinnedMeshShader()
{
	clear();
}

void SkinnedMeshShader::load(const std::string & fileName)
{
	std::experimental::filesystem::path shaderFilePath(fileName);

	UINT compileFlag = 0;
#ifdef _DEBUG
	compileFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#endif // _DEBUG

	ID3DBlob* compilePs{ NULL };
	D3DCompileFromFile((LPCWSTR)shaderFilePath.generic_u16string().c_str(), NULL, NULL, "PS", "ps_5_0", compileFlag, 0, &compilePs, NULL);
	m_device->CreatePixelShader(compilePs->GetBufferPointer(), compilePs->GetBufferSize(), NULL, &m_pixelShader);
	compilePs->Release();

	ID3DBlob* compileVs{ NULL };
	D3DCompileFromFile((LPCWSTR)shaderFilePath.generic_u16string().c_str(), NULL, NULL, "VS", "vs_5_0", compileFlag, 0, &compileVs, NULL);
	m_device->CreateVertexShader(compileVs->GetBufferPointer(), compileVs->GetBufferSize(), NULL, &m_vertexShader);

	D3D11_INPUT_ELEMENT_DESC layout[] = {
		{ "POSITION",		0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL",			0, DXGI_FORMAT_R32G32B32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD",		0, DXGI_FORMAT_R32G32_FLOAT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDINDICES",	0, DXGI_FORMAT_R8G8B8A8_SINT,		0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT",	0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};
	m_device->CreateInputLayout(layout, 5, compileVs->GetBufferPointer(), compileVs->GetBufferSize(), &m_vertexLayout);
	compileVs->Release();
}

void SkinnedMeshShader::world(const Matrix4 & mat)
{
	m_data.world = mat;
}

void SkinnedMeshShader::view(const Matrix4 & mat)
{
	m_data.view = mat;
}

void SkinnedMeshShader::projection(const Matrix4 & mat)
{
	m_data.projection = mat;
}

void SkinnedMeshShader::clear()
{
	if (m_constantBuffer != nullptr)
	{
		m_constantBuffer->Release();
		m_constantBuffer = nullptr;
	}

	if (m_vertexShader != nullptr)
	{
		m_vertexShader->Release();
		m_vertexShader = nullptr;
	}

	if (m_pixelShader != nullptr)
	{
		m_pixelShader->Release();
		m_pixelShader = nullptr;
	}

	if (m_vertexLayout != nullptr)
	{
		m_vertexLayout->Release();
		m_vertexLayout = nullptr;
	}
}

void SkinnedMeshShader::begin()
{
	D3D11_MAPPED_SUBRESOURCE mappedSubresource{};
	m_deviceContext->Map(m_constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedSubresource);
	memcpy_s(mappedSubresource.pData, mappedSubresource.RowPitch, (void*)(&m_data), sizeof(m_data));
	m_deviceContext->Unmap(m_constantBuffer, 0);

	m_deviceContext->VSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_deviceContext->PSSetConstantBuffers(0, 1, &m_constantBuffer);
	m_deviceContext->VSSetShader(m_vertexShader, NULL, 0);
	m_deviceContext->PSSetShader(m_pixelShader, NULL, 0);
	m_deviceContext->IASetInputLayout(m_vertexLayout);
}

void SkinnedMeshShader::end()
{
}

void SkinnedMeshShader::material(const Mesh::Material & material)
{
	material.texture->apply();
}

void SkinnedMeshShader::boneMatrices(int size, const Matrix4 matrices[])
{
	std::copy(&matrices[0], &matrices[size], m_data.boneMatrices);
}

