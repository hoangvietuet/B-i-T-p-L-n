
#include "TextureManager.h"
#include "Game.h"
#include "Camera.h"
#include "tinyxml.h"
TextureManager* TextureManager::s_Instance = nullptr;
bool TextureManager::Load(std::string id, std::string filename) {
    SDL_Surface* surface = IMG_Load(filename.c_str());
    if (surface == nullptr) {
        SDL_Log("Failed to load image %s: %s", filename.c_str(), SDL_GetError());
        return false;
    }

    SDL_Log("✅ Loaded image: %s", filename.c_str());


    SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 0, 0, 0)); // Đặt màu đen thành trong suốt

    SDL_Texture* texture = SDL_CreateTextureFromSurface(Game::GetInstance()->GetRenderer(), surface);
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        SDL_Log("❌ Failed to create texture from %s", filename.c_str());
        return false;
    }

    m_TextureMap[id] = texture;
    return true;
}
void TextureManager::Draw(std::string id, int x, int y, int width, int height, float scaleX, float scaleY, float scrollRatio, SDL_RendererFlip flip) {
    if (m_TextureMap.find(id) == m_TextureMap.end()) return;
    if (m_TextureMap[id] == nullptr) return;

    SDL_Rect srcRect = {0, 0, width, height};
    SDL_Rect dstRect;

    if (id == "bg" || scrollRatio == 0) {
        dstRect = {x, y, width*scaleX, height*scaleY};
    } else {
        Vector2D cam = Camera::GetInstance()->GetPosition()*scrollRatio;
        dstRect = {x - cam.X, y - cam.Y, width*scaleX, height*scaleY};
    }

    if (SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip) != 0) {
        SDL_Log("❌ SDL_RenderCopyEx error: %s", SDL_GetError());
    } else {
        SDL_Log("✅ Drawing texture: %s at (%d, %d)", id.c_str(), x, y);
    }
}
void TextureManager::DrawFrame(std::string id, int x, int y, int width, int height, int row, int frame, SDL_RendererFlip flip) {
    if (m_TextureMap.find(id) == m_TextureMap.end()) {
        //SDL_Log("Texture ID %s not found!", id.c_str());
        return;
    }

    SDL_Rect srcRect = {width * frame, height * (row - 1), width, height};
     Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {x-cam.X, y-cam.Y, width, height};

    SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[id], &srcRect, &dstRect, 0, nullptr, flip);
}

void TextureManager::DrawTile(std::string tilesetID, int tileSize, int x, int y, int row, int col, SDL_RendererFlip flip) {

    SDL_Rect srcRect = {
        col * tileSize,
        row * tileSize,
        tileSize,
        tileSize
    };
     Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect dstRect = {
        x-cam.X,
        y-cam.Y,
        tileSize,
        tileSize
    };



    SDL_RenderCopyEx(Game::GetInstance()->GetRenderer(), m_TextureMap[tilesetID], &srcRect, &dstRect, 0, nullptr, flip);
}
void TextureManager::Drop(std::string id) {
    if (m_TextureMap.find(id) != m_TextureMap.end()) {
        SDL_DestroyTexture(m_TextureMap[id]);
        m_TextureMap.erase(id);
    }
}

void TextureManager::Clean() {
    for (auto& texture : m_TextureMap) {
        SDL_DestroyTexture(texture.second);
    }
    m_TextureMap.clear();
    SDL_Log("Texture map cleaned!");
}

bool TextureManager::ParseTextures(std::string source)
{
    TiXmlDocument xml;
    if (!xml.LoadFile(source)) {
        std::cout << "Failed to load: " << source << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    if (root == nullptr) {
        std::cout << "Root element is missing in the XML file." << std::endl;
        return false;
    }

    for (TiXmlElement* e = root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement()) {
        if (e->ValueStr() == "Texture") {
            const char* id = e->Attribute("id");
            const char* src = e->Attribute("source");

            if (id && src) {
                Load(id, src);
            } else {
                std::cout << "Missing attributes in a <Texture> element." << std::endl;
            }
        }
    }
    return true;
}
bool TextureManager::HasTexture(std::string id) {
    auto it = m_TextureMap.find(id);
    return it != m_TextureMap.end() && it->second != nullptr;
}
