
#include "MapParser.h"
#include "SDL.h"
#include <iostream>

MapParser* MapParser::s_Instance = nullptr;

bool MapParser::Load() {
    if (!Parse("level1", "assets/Maps/mapp.tmx")) {
        std::cerr << "Failed to parse map: assets/Maps/mapp.tmx" << std::endl;
        return false;
    }
    return true;
}

Tileset MapParser::ParseTileset(TiXmlElement* xmlTileset) {
    Tileset tileset;
    tileset.Name = xmlTileset->Attribute("name");
    xmlTileset->Attribute("firstgid", &tileset.FirstID);
    xmlTileset->Attribute("tilecount", &tileset.TileCount);
    tileset.LastID = (tileset.FirstID + tileset.TileCount) - 1;
    xmlTileset->Attribute("columns", &tileset.ColCount);
    tileset.RowCount = tileset.TileCount / tileset.ColCount;
    xmlTileset->Attribute("tilewidth", &tileset.TileSize);

    TiXmlElement* image = xmlTileset->FirstChildElement("image");
    if (!image) {
        std::cerr << "Missing image element in tileset: " << tileset.Name << std::endl;
        return tileset;
    }

    std::string sourcePath = image->Attribute("source");
    if (sourcePath.find("assets/Maps/") == std::string::npos) {
        tileset.Source = "assets/Maps/" + sourcePath;
    } else {
        tileset.Source = sourcePath;
    }

    std::cout << "Tileset source path: " << tileset.Source << std::endl;
    return tileset;
}

bool MapParser::Parse(std::string id, std::string source) {
    TiXmlDocument xml;
    if (!xml.LoadFile(source)) {
        std::cerr << "Failed to load: " << source << " Error: " << xml.ErrorDesc() << std::endl;
        return false;
    }

    TiXmlElement* root = xml.RootElement();
    int rowcount, colcount, tilesize = 0;
    root->Attribute("width", &colcount);
    root->Attribute("height", &rowcount);
    root->Attribute("tilewidth", &tilesize);

    // Parse tilesets
    TilesetList tilesets;
    for (TiXmlElement* e = root->FirstChildElement("tileset"); e != nullptr; e = e->NextSiblingElement("tileset")) {
        Tileset tileset = ParseTileset(e);
        tilesets.push_back(tileset);
    }

    // Parse layers
    GameMap* gamemap = new GameMap();
    for (TiXmlElement* e = root->FirstChildElement("layer"); e != nullptr; e = e->NextSiblingElement("layer")) {
        TileLayer* tilelayer = ParseTileLayer(e, tilesets, tilesize, rowcount, colcount);
        if (tilelayer) {
            gamemap->m_MapLayers.push_back(tilelayer);
        }
    }

    m_MapDict[id] = gamemap;
    return true;
}

TileLayer* MapParser::ParseTileLayer(TiXmlElement* xmlLayer, TilesetList tilesets, int tilesize, int rowcount, int colcount) {
    TiXmlElement* data = xmlLayer->FirstChildElement("data");
    if (!data) {
        std::cerr << "Error: Missing <data> element in layer." << std::endl;
        return nullptr;
    }

    std::string matrix(data->GetText());
    std::istringstream iss(matrix);
    std::string id;

    TileMap tilemap(rowcount, std::vector<int>(colcount, 0));
    for (int row = 0; row < rowcount; row++) {
        for (int col = 0; col < colcount; col++) {
            getline(iss, id, ',');
            std::stringstream convertor(id);
            convertor >> tilemap[row][col];
            //std::cout << "ID: " << id << std::endl;

            if (!iss.good()) {
                break;
            }
        }
    }

    return new TileLayer(tilesize, rowcount, colcount, tilemap, tilesets);
}

void MapParser::Clean() {
    for (auto& pair : m_MapDict) {
        delete pair.second;
    }
    m_MapDict.clear();
}

