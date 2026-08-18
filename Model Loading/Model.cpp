#include "Model.h"

Model::Model(const char* path)
{
	stbi_set_flip_vertically_on_load(true);

	LoadModel(path);
}
void Model::Draw(Shader& shader)
{
	for (int i = 0;i < meshes.size();i++)
	{
		meshes[i].Draw(shader);
	}
}
void Model::LoadModel(std::string path)
{
	Assimp::Importer importer; //creating an importer object
	//can mention post processing options while importing
	const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Log("ERROR::ASSIMP::" << importer.GetErrorString());
		return;
	}
	directory = path.substr(0, path.find_last_of("/"));
	Log("total meshes count :" << scene->mNumMeshes);
	ProcessNode(scene->mRootNode, scene);
}
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	//process all the meshes in the current node
	for (int i = 0;i < node->mNumMeshes;i++)
	{
		aiMesh* aimesh = scene->mMeshes[node->mMeshes[i]];
		Mesh mesh = ProcessMesh(aimesh, scene);
		meshes.push_back(mesh);
	}

	//then process all the meshes in the children of the current node
	for (int i = 0;i < node->mNumChildren;i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}

}
Mesh Model::ProcessMesh(aiMesh* aiMesh, const aiScene* scene)
{
	std::vector<vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Texture> textures;

	Log("processing mesh : " << aiMesh->mName.C_Str());
	//vertices
	for (int i = 0;i < aiMesh->mNumVertices;i++)
	{
		vertex vertex;

		glm::vec3 pos;
		pos.x = aiMesh->mVertices[i].x;
		pos.y = aiMesh->mVertices[i].y;
		pos.z = aiMesh->mVertices[i].z;
		vertex.position = pos;

		glm::vec3 normal;
		normal.x = aiMesh->mNormals[i].x;
		normal.y = aiMesh->mNormals[i].y;
		normal.z = aiMesh->mNormals[i].z;
		vertex.normal = normal;

		if (aiMesh->mTextureCoords[0])
		{
			glm::vec2 uv;
			uv.x = aiMesh->mTextureCoords[0][i].x;
			uv.y = aiMesh->mTextureCoords[0][i].y;
			vertex.texCoords = uv;
		}
		else
		{
			vertex.texCoords = glm::vec2(0.0f,0.0f);
		}

	}

	//indices
	for (int i = 0;i < aiMesh->mNumFaces;i++)
	{
		aiFace face = aiMesh->mFaces[i];
		for (int j = 0;j < face.mNumIndices;j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//textures
	if (aiMesh->mMaterialIndex >= 0)
	{
		aiMaterial* material = scene->mMaterials[aiMesh->mMaterialIndex];

		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, 
			aiTextureType_DIFFUSE, "diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<Texture> specularMaps = LoadMaterialTextures(material, 
			aiTextureType_SPECULAR, "specular");

		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
	}

	return Mesh(vertices, textures, indices);
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
	std::vector<Texture> textures;
	for (int i = 0;i < mat->GetTextureCount(type);i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		Log("Loading Texture : " << str.C_Str());
		bool skip = false;
		for (int j = 0;j < loadedTextures.size();j++)
		{
			if (std::strcmp(loadedTextures[j].path.data(), str.C_Str()) == 0)
			{
				//texture exists, skip loading , use existing 
				Log(str.C_Str()<<"already loaded , skipping it")
				textures.push_back(loadedTextures[j]);
				skip = true;
				break;
			}
		}

		if (!skip)
		{
			//if texture isnt loaded already ,load it
			Texture texture;
			texture.id = LoadTexture(str.C_Str(), directory);
			texture.type = typeName;
			texture.path = str.C_Str();
			loadedTextures.push_back(texture);
			textures.push_back(texture);
		}
		
	}
	
	return textures;
}

unsigned int LoadTexture(const char* path,const std::string& directory)
{
	std::string fileName = std::string(path);
	fileName = directory + "/" + fileName;

	unsigned int textureID;
	glGenTextures(1, &textureID);

	glBindTexture(GL_TEXTURE_2D, textureID);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // to prevent image with wdith!=height from getting corrupted
	int width, height, nrChannels;
	unsigned char* data = stbi_load(fileName.c_str(), &width, &height, &nrChannels, 0);
	Log("loaded texture : " << fileName << " , channels : " << nrChannels);
	if (data)
	{
		switch (nrChannels)
		{
		case 1:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, width, height, 0, GL_RED, GL_UNSIGNED_BYTE, data);
			break;
		case 3: // jpg format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case 4:// png format
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			Log("invalid image channels")
				break;
		}

		glBindTexture(GL_TEXTURE_2D, textureID);
		glGenerateMipmap(GL_TEXTURE_2D);

	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return textureID;
}

