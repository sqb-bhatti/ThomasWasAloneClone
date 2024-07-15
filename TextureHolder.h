

//
// Created by Saqib Bhatti on 15/7/24.
//

#ifndef TWL_TEXTUREHOLDER_H
#define TWL_TEXTUREHOLDER_H


#include <SFML/Graphics.hpp>
#include <map>  // include directive for map from the STL.


using namespace sf;
using namespace std;


class TextureHolder {
private:
    // A map container that holds related pairs of String and Texture
    map<string, Texture> m_Textures;

    static TextureHolder* m_s_Instance;

public:
    TextureHolder();

    // return a reference which is efficient because it avoids making a copy of what could be a large graphic.
    static Texture& GetTexture(string const& filename);
};


#endif //TWL_TEXTUREHOLDER_H
