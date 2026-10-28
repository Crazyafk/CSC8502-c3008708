/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "TutorialRenderer.h"
#include "GLTFLoader.h"
#include "../NCLCoreClasses/KeyboardMouseController.h"

#include <cassert>

namespace NCL::Rendering {
	#include "./Shaders/glslInterop.h"
//	#include "./Shaders/LightStruct.h"
}

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

OpenGLTutorialEntry* OpenGLTutorialEntry::s_listStartPtr = nullptr;

TutorialRenderer::TutorialRenderer() : OGLRenderer(*Window::GetWindow())
{
	GLTFLoader::SetTextureConstructionFunction(
		[&](std::string& input) ->  SharedTexture {
			return std::shared_ptr<Texture>(OGLTexture::TextureFromFile(input));
		}
	);

	GLTFLoader::SetMeshConstructionFunction(
		[&]()-> std::shared_ptr<Mesh> {return std::shared_ptr<Mesh>(new OGLMesh()); }
	);

	triMesh		= CreateTriangle();
	quadMesh	= CreateQuad();
	sphereMesh	= LoadSingleMesh("uvSphere.gltf");

	defaultCamera	= new PerspectiveCamera();

	controller = new KeyboardMouseController(*Window::GetWindow()->GetKeyboard(), *Window::GetWindow()->GetMouse());
	
	controller->MapAxis(0, "Sidestep");
	controller->MapAxis(1, "UpDown");
	controller->MapAxis(2, "Forward");

	controller->MapAxis(3, "XLook");
	controller->MapAxis(4, "YLook");
	
	defaultCamera->SetController(*controller);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
}

TutorialRenderer::~TutorialRenderer() {
	delete defaultCamera;
}

void TutorialRenderer::Update(float dt) {
	defaultCamera->UpdateCamera(dt);
	frameTime		= dt;
	totalRunTime	+= dt;
}

UniqueOGLMesh TutorialRenderer::CreateTriangle() {
	UniqueOGLMesh triMesh = std::make_unique<OGLMesh>();
	triMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(0,1,0) });
	triMesh->SetVertexColours({ Vector4(1,0,0,1), Vector4(0,1,0,1), Vector4(0,0,1,1) });
	triMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(0.5, 1) });
	triMesh->SetVertexIndices({ 0,1,2 });

	triMesh->SetDebugName("Triangle");
	triMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	triMesh->UploadToGPU(this);

	return triMesh;
}

UniqueOGLMesh TutorialRenderer::CreateQuad() {
	UniqueOGLMesh quadMesh = std::make_unique<OGLMesh>();
	quadMesh->SetVertexPositions({ Vector3(-1,-1,0), Vector3(1,-1,0), Vector3(1,1,0), Vector3(-1,1,0) });
	quadMesh->SetVertexTextureCoords({ Vector2(0,0), Vector2(1,0), Vector2(1, 1), Vector2(0, 1) });

	quadMesh->SetVertexNormals(
		std::vector<Vector3>(4, Vector3(0,0,-1))
	);

	quadMesh->SetVertexTangents(
		std::vector<Vector4>(4, Vector4(1, 0, 0, 1))
	);

	quadMesh->SetVertexIndices({ 0,1,3,2 });
	quadMesh->SetDebugName("Fullscreen Quad");
	quadMesh->SetPrimitiveType(NCL::GeometryPrimitive::TriangleStrip);
	quadMesh->UploadToGPU(this);

	return quadMesh;
}

SharedOGLMesh TutorialRenderer::LoadSingleMesh(const std::string& filename) {
	//gltf.Load(filename);

	//assert(!gltf.outMeshes.empty());
	////gltf.outMeshes[0]->UploadToGPU(this);
	////OGLMesh* m = (OGLMesh*)gltf.outMeshes[0];

	//SharedOGLMesh mesh = std::dynamic_pointer_cast<OGLMesh>(gltf.outMeshes[0]);
	//mesh->UploadToGPU(this);

	//gltf.outMeshes.pop_back();
	//return mesh;
	return nullptr;
}

void TutorialRenderer::CalculateNormals(Mesh& m) const {
	int count = m.GetPrimitiveCount();

	std::vector<Vector3> newNormals;
	newNormals.resize(m.GetVertexCount());

	for (int i = 0; i < count; ++i) {
		Vector3 a, b, c;
		unsigned int ai, bi, ci;
		m.GetTriangle(i, a, b, c);
		m.GetVertexIndicesForTri(i, ai, bi, ci);

		Vector3 n = Vector::Cross(b - a, c - a);
		n = Vector::Normalise(n);

		newNormals[ai] += n;
		newNormals[bi] += n;
		newNormals[ci] += n;
	}
	for (int i = 0; i < newNormals.size(); ++i) {
		newNormals[i] = Vector::Normalise(newNormals[i]);
	}

	m.SetVertexNormals(newNormals);
}

void TutorialRenderer::CalculateNormalsTangents(Mesh& m) const {
	size_t primCount = m.GetPrimitiveCount();
	size_t vertCount = m.GetVertexCount();

	std::vector<Vector4> newTangents;
	newTangents.resize(vertCount);

	std::vector<Vector3> newNormals;
	newNormals.resize(vertCount);

	const std::vector<Vector3>& positions	= m.GetPositionData();
	const std::vector<Vector2>& UVs			= m.GetTextureCoordData();

	if (positions.size() != UVs.size()) {
		return;
	}

	for (int i = 0; i < primCount; ++i) {
		unsigned int ai, bi, ci;
		m.GetVertexIndicesForTri(i, ai, bi, ci);

		Vector3 ba = positions[bi] - positions[ai];
		Vector3 ca = positions[ci] - positions[ai];

		Vector2 tba = UVs[bi] - UVs[ai];
		Vector2 tca = UVs[ci] - UVs[ai];

		Matrix2 texMatrix;
		texMatrix.SetColumn(0, tba);
		texMatrix.SetColumn(1, tca);

		texMatrix = Matrix::Inverse(texMatrix);

		Vector3 tangent		= ba * texMatrix.array[0][0] + ca * texMatrix.array[0][1];
		Vector3 binormal	= ba * texMatrix.array[1][0] + ca * texMatrix.array[1][1];

		Vector3 normal	= Vector::Cross(ba, ca);
		Vector3 biCross = Vector::Cross(tangent, normal);

		float handedness = 1.0f;
		if (Vector::Dot(biCross, binormal) < 0.0f) {
			handedness = -1.0f;
		}

		Vector4 t(tangent.x, tangent.y, tangent.z, handedness);

		newTangents[ai] += t;
		newTangents[bi] += t;
		newTangents[ci] += t;

		newNormals[ai] += normal;
		newNormals[bi] += normal;
		newNormals[ci] += normal;
	}

	for (int i = 0; i < vertCount; ++i) {
		newNormals[i]		= Vector::Normalise(newNormals[i]);

		float handedness	= newTangents[i].w > 0.0f ? 1.0f : -1.0f;
		newTangents[i].w	= 0.0f;
		newTangents[i]		= Vector::Normalise(newTangents[i]);
		newTangents[i].w	= handedness;
	}

	m.SetVertexTangents(newTangents);
	m.SetVertexNormals(newNormals);
}


GLuint	TutorialRenderer::CreateTexture(uint32_t width, uint32_t height, GLint internalFormat, bool allocMips) {
	GLuint tex = 0;

	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	GLsizei levels = 1;

	if (allocMips) {

	}

	glTexStorage2D(GL_TEXTURE_2D, levels, internalFormat, width, height);
	glBindTexture(GL_TEXTURE_2D, 0);

	return tex;
}

void TutorialRenderer::SetUniform(const std::string& name, const Matrix4& mat) {
	assert(m_boundShader);
	GLuint id		= m_boundShader->GetProgramID();
	GLint  location	= glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniformMatrix4fv(location, 1, false, (float*)&mat);
}

void TutorialRenderer::SetUniform(const std::string& name, const Vector2& v) {
	assert(m_boundShader);
	GLuint id = m_boundShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform2f(location, v.x, v.y);
}

void TutorialRenderer::SetCameraUniforms(const Camera& c, GLuint location) {
	float aspect = Window::GetWindow()->GetScreenAspect();
	Matrix4 viewMatrix	= c.BuildViewMatrix();
	Matrix4 projMatrix	= c.BuildProjectionMatrix(aspect);
	Vector3 position	= c.GetPosition();

	assert(m_boundShader);
	GLuint id = m_boundShader->GetProgramID();

	glUniformMatrix4fv(location		, 1, false, (float*)&viewMatrix);
	glUniformMatrix4fv(location + 1	, 1, false, (float*)&projMatrix);
	glUniform3fv(location + 2, 1, (float*)&position);
}

void TutorialRenderer::BindBufferAsUBO(const OGLBuffer& b, GLuint slotID) {
	glBindBufferBase(GL_UNIFORM_BUFFER, slotID, b.gpuID);
}

void TutorialRenderer::BindBufferAsSSBO(const OGLBuffer& b, GLuint slotID) {
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, slotID, b.gpuID);
}

//void TutorialRenderer::SetLightUniforms(const Light& l, GLuint location) {
//	assert(m_boundShader);
//	GLuint id = m_boundShader->GetProgramID();
//
//	glUniform3fv(location	, 1, (float*)&l.position);
//	glUniform1f(location + 1, l.radius);
//	glUniform3fv(location + 2, 1, (float*)&l.intensity);
//}

void TutorialRenderer::BindTextureToPipeline(GLuint texID, const std::string& name, GLuint texUnit, GLuint texType)  {
	assert(m_boundShader);
	GLuint id = m_boundShader->GetProgramID();
	GLint  location = glGetUniformLocation(id, name.c_str());
	if (location == -1) {
		return;
	}
	glUniform1i(location, texUnit);

	glActiveTexture(GL_TEXTURE0 + texUnit);
	glBindTexture(texType, texID);
}


TutorialRenderer* TutorialRenderer::CreateTutorial(const std::string& name) {
	OpenGLTutorialEntry* e = OpenGLTutorialEntry::s_listStartPtr;
	while (e) {
		if (e->m_name == name) {
			std::cout << "Running tutorial " << e->m_name << "\n";
			return e->m_creatorFunc();
		}
		e = e->m_nodeChain;
	}
	std::cout << "Cannot find tutorial " << name << "!\n";
	return nullptr;
}

void TutorialRenderer::WindowEventHandler(WindowEvent e, uint32_t w, uint32_t h) {
	if (e == WindowEvent::Minimize || e == WindowEvent::Maximize) {
	}
	if (e == WindowEvent::Resize || e == WindowEvent::Maximize) {
		OnWindowResize(w, h);
	}
}