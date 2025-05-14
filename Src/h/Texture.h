#pragma once
#include "glad.h"
#include "Shader.h"

class CTexture
{
public:
	CTexture(const char* pTexturePath);
	CTexture();
	~CTexture();
	
	
	void Link(const CShader* pShader, unsigned int nBindNumber, const char* pLocationName) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader

	void Bind(unsigned int nBindNumber) const; //nBindNumber is the number of the texture starting at 0. pLocationName is the variable name of the texture inside the shader
	GLuint GetTextureID() const;
	void SetTextureData(unsigned int nWidth, unsigned int nHeight, unsigned int nCountChannels, std::vector<GLfloat>* data, bool bCreateMipmaps, bool bLinearInterpolation = true);
	void LoadTexture(const char* pTexturePath);
	void LoadMaterials();
	void BindMaterial(unsigned int nBindNumberColorSpec, unsigned int nBindNumberNormalHeight);
	void LinkMaterial(CShader* pShader, unsigned int nBindNumberColorSpec, unsigned int nBindNumberNormalHeight, const char* pLocationNameColorSpec, const char* pLocationNameNormalHeight);
	void changeSize(unsigned int width, unsigned int height);
	void getTextureSize(unsigned int& width, unsigned int& height) const;

private:
	void LoadSingleMaterialToTextureArray(const char* pMaterialFolderName, unsigned int nTextureNumber, unsigned int nMaterialNumber);
	GLuint m_nID = 0; //texture id
	unsigned int m_width = 0;
	unsigned int m_height = 0;
	GLuint m_nMaterialColorSpecularID = 0;
	GLuint m_nMaterialNormalHeightID = 0;
};

