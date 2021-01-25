#include "stdafx.h"
#include "TextureHolder.h"
//inclde the assert feature
#include <assert.h>

TextureHolder* TextureHolder::m_s_Instance = nullptr;

TextureHolder::TextureHolder()
{
	assert(m_s_Instance == nullptr);
	m_s_Instance = this;
}

sf::Texture& TextureHolder::GetTexture(std::string const& filename)
{
	//get a reference to m_Textures using m_s_Instance
	auto& m = m_s_Instance->m_Textures;
	//auto is the equivalent of map<string, Texture>

	//create an iterator to hold a key-value pair (kvp)
	//and search for the required kvp
	//using the passed in file name
	auto keyValuePair = m.find(filename);
	//auto is equivilant of map<string, Texture>::iterator

	//did we find a match?
	if (keyValuePair != m.end())
	{
		/*yes
		return the texture
		the second part of kvp, the texture*/
		return keyValuePair->second;
	}
	else
	{
		//file name not found
		//create a new key value pair using the filename
		auto& texture = m[filename];

		//load the texture from file in the usual way
		texture.loadFromFile(filename);

		//return the texture to the calling code
		return texture;
	}


}