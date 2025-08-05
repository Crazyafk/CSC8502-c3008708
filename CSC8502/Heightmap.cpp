/******************************************************************************
This file is part of the Newcastle OpenGL Tutorial Series
Author:Rich Davison
Contact:richgdavison@gmail.com
License: MIT (see LICENSE file at the top of the source tree)
*//////////////////////////////////////////////////////////////////////////////
#include "Heightmap.h"
#include "Maths.h"

using namespace NCL;
using namespace Rendering;
using namespace CSC8502;

void Heightmap::CreateHeightmap(const std::string& filename, Mesh& mesh, Vector3 worldScale, Vector2 uvScale)  {
	//char* texData	= nullptr;
	//int texWidth	= 0;
	//int texHeight	= 0;
	//int texChannels = 0;
	//int flags = 0;
	//TextureLoader::LoadTexture(filename, texData, texWidth, texHeight, texChannels, flags);

	//int pixelStride = texChannels;

	//int vCount = texWidth * texHeight;
	//int iCount = (texWidth - 1) * (texHeight - 1) * 6;

	//vector<Vector3> vertices(vCount, Vector3(0, 0, 0));
	//vector<Vector2> uvCoords(vCount, Vector3(0, 0, 0));

	//vector<unsigned int> indices(iCount, 0);

	//float xAdvance = 2.0f / texWidth;
	//float zAdvance = 2.0f / texHeight;

	//for (int z = 0; z < texHeight; ++z) {
	//	for (int x = 0; x < texWidth; ++x) {
	//		int offset = (z * texWidth) + x;
	//		unsigned char value = texData[offset * pixelStride];

	//		vertices[offset] = (Vector3(-1.0f + (x * xAdvance), value / 255.0f, -1.0f + (z * zAdvance))) * worldScale;
	//		uvCoords[offset] = Vector2(x, z) / uvScale;
	//	}
	//}

	//int i = 0;

	//for (int z = 0; z < texHeight - 1; ++z) {
	//	for (int x = 0; x < texWidth - 1; ++x) {
	//		int a = (z * (texWidth)) + x;
	//		int b = (z * (texWidth)) + (x + 1);
	//		int c = ((z + 1) * (texWidth)) + (x + 1);
	//		int d = ((z + 1) * (texWidth)) + x;

	//		indices[i++] = a;
	//		indices[i++] = c;
	//		indices[i++] = b;

	//		indices[i++] = c;
	//		indices[i++] = a;
	//		indices[i++] = d;
	//	}		
	//}

	//mesh.SetVertexPositions(vertices);
	//mesh.SetVertexTextureCoords(uvCoords);
	//mesh.SetVertexIndices(indices);
}
