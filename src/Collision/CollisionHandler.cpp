
#include "CollisionHandler.h"
#include "Game.h"
#include <iostream>

CollisionHandler* CollisionHandler::s_Instance = nullptr;

CollisionHandler::CollisionHandler() {
    if (Game::GetInstance()->GetMap() != nullptr) {
        m_CollisionLayer = (TileLayer*)Game::GetInstance()->GetMap()->GetMapLayers().front();
        m_CollisionTilemap = m_CollisionLayer->GetTilemap();

        if (!m_CollisionTilemap.empty()) {
            std::cout << "Bản đồ va chạm đã tải thành công!" << std::endl;
        } else {
            std::cout << "Lỗi: Bản đồ va chạm trống!" << std::endl;
        }
    } else {
        m_CollisionLayer = nullptr;
        std::cout << "Lỗi: Không tìm thấy lớp va chạm!" << std::endl;
    }
}
bool CollisionHandler::MapCollision(SDL_Rect a) {
    if (m_CollisionLayer == nullptr || m_CollisionTilemap.empty()) {
        std::cout << "Cảnh báo: Dữ liệu bản đồ va chạm không hợp lệ!" << std::endl;
        return false;
    }

    int tileSize = 32;
    int RowCount = m_CollisionTilemap.size();
    int ColCount = m_CollisionTilemap[0].size();

    int left_tile = a.x / tileSize;
    int right_tile = (a.x + a.w) / tileSize;
    int top_tile = a.y / tileSize;
    int bottom_tile = (a.y + a.h) / tileSize;

    left_tile = std::max(0, left_tile);
    right_tile = std::min(ColCount - 1, right_tile);
    top_tile = std::max(0, top_tile);
    bottom_tile = std::min(RowCount - 1, bottom_tile);

    for (int i = left_tile; i <= right_tile; i++) {
        for (int j = top_tile; j <= bottom_tile; j++) {
            if (m_CollisionTilemap[j][i] > 0) {
                return true;
            }
        }
    }
    return false;
}

bool CollisionHandler::GroundCollision(SDL_Rect a) {
    SDL_Rect groundCheck = { a.x + 4, a.y + a.h - 2, a.w - 8, 2 };
    return MapCollision(groundCheck);



}
bool CollisionHandler::CheckCollision(SDL_Rect a, SDL_Rect b) {
    bool x_overlaps = (a.x < b.x + b.w) && (a.x + a.w > b.x);
    bool y_overlaps = (a.y < b.y + b.h) && (a.y + a.h > b.y);
    return (x_overlaps && y_overlaps);
}
