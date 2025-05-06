#include "BloodItem.h"
#include "TextureManager.h"
#include "Camera.h"

BloodItem::BloodItem(float x, float y) {
    m_Transform = Vector2D(x, y);
    m_Velocity = Vector2D(0, 100.0f); // Rơi xuống với tốc độ 100 pixel/giây
    m_Width = 32;  // Kích thước cục máu
    m_Height = 32;
    m_TextureID = "blood_item"; // ID của texture trong textures.tml

    m_Collider = new Collider();
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);
}

BloodItem::~BloodItem() {
    Clean();
}

void BloodItem::Draw() {
    TextureManager::GetInstance()->Draw(m_TextureID, m_Transform.X - Camera::GetInstance()->GetPosition().X,
                                        m_Transform.Y - Camera::GetInstance()->GetPosition().Y, m_Width, m_Height);
}

void BloodItem::Update(float dt) {
    m_Transform.Y += m_Velocity.Y * dt; // Cập nhật vị trí rơi
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);
}

void BloodItem::Clean() {
    delete m_Collider;
    m_Collider = nullptr;
}
