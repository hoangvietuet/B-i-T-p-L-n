#include "ENEMY.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Camera.h"
#include "Vector2D.h"
#include "Game.h"
#include "CollisionHandler.h"
#include "RigidBody.h"
#include <iostream>

Enemy::Enemy(Properties* props) : Character(props) {
    m_IsAttacking = false;
    m_AttackCooldown = ENEMY_ATTACK_COOLDOWN;

    m_Collider = new Collider();
    m_RigidBody = new RigidBody();
    m_RigidBody->SetFravity(5.0f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 10, 120);
    m_Width = 128;
    m_Height = 128;

    m_State = ENEMY_PATROL;
    m_SpawnPoint = m_Transform;

    m_TargetPosition = Vector2D(0, 0);
    m_MoveDirection = -1;
    m_PatrolRange = 50.0f;
    m_HP = 10000;


    std::cout << "Enemy initialized at: X=" << m_Transform.X << ", Y=" << m_Transform.Y << std::endl;
}

void Enemy::Draw() {
    int offsetY = m_Height - 70;
    int offsetX = m_Width / 2 - 20;
    float drawX = m_Transform.X - offsetX;
    float drawY = m_Transform.Y - offsetY;
    m_Animation->Draw(drawX, drawY, m_Width, m_Height, m_Flip);

    // Debug tọa độ vẽ
    std::cout << "Drawing enemy at: X=" << drawX << ", Y=" << drawY << ", Width=" << m_Width << ", Height=" << m_Height << ", Flip=" << m_Flip << std::endl;

   /* // Vẽ hộp va chạm của Enemy
    SDL_Renderer* renderer = Game::GetInstance()->GetRenderer();
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_Rect box = m_Collider->Get();
    box.x -= Camera::GetInstance()->GetPosition().X;
    box.y -= Camera::GetInstance()->GetPosition().Y;
    SDL_RenderDrawRect(renderer, &box); */
}

void Enemy::Update(float dt) {
    int offsetY = m_Height - 70;
    int offsetX = m_Width / 2 - 20;
    if (m_State == ENEMY_DIE) {
        m_Animation->Update();
        return;
    }

    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    int playerCenterX = playerPos.X + 64;
    int enemyCenterX = m_Transform.X + 64;
    int distance = abs(playerCenterX - enemyCenterX);

    if (m_AttackCooldown > 0) {
        m_AttackCooldown -= dt;
    }

    std::cout << "Enemy state: " << m_State << ", Distance: " << distance << ", PlayerX=" << playerPos.X << ", EnemyX=" << m_Transform.X << ", Cooldown=" << m_AttackCooldown << ", dt=" << dt << std::endl;

    if (m_State != ENEMY_ATTACK) {
        if (distance > ENEMY_CHASE_RANGE && m_State != ENEMY_DIE) {
            m_State = ENEMY_IDLE;
            m_RigidBody->UnSetForce();
        } else if (distance <= ENEMY_ATTACK_RANGE && m_AttackCooldown <= 0 && m_State != ENEMY_DIE) {
            m_State = ENEMY_ATTACK;

            if (playerCenterX < enemyCenterX) {
                m_Animation->SetProps("enemy_attack_left", 1, 5, 150);
                m_Flip = SDL_FLIP_HORIZONTAL;
            } else {
                m_Animation->SetProps("enemy_attack", 1, 5, 150);
                m_Flip = SDL_FLIP_NONE;
            }
            m_RigidBody->UnSetForce();
        } else if (distance <= ENEMY_CHASE_RANGE && m_State != ENEMY_DIE) {
            m_State = ENEMY_CHASE;
        } else if (m_State != ENEMY_DIE) {
            m_State = ENEMY_IDLE;
            m_RigidBody->UnSetForce();
        }
    }


    switch (m_State) {
    case ENEMY_PATROL:
        PatrolState(dt);
        break;
    case ENEMY_CHASE:
        ChaseState(dt);
        break;
    case ENEMY_ATTACK:
        AttackState(dt);
        break;
    case ENEMY_IDLE:
        m_RigidBody->UnSetForce();
        break;
    }

    m_RigidBody->Update(dt);

    Vector2D nextPos = m_Transform + m_RigidBody->Position();
    SDL_Rect futureCollider = {
        static_cast<int>(nextPos.X - m_Width / 2),
        static_cast<int>(nextPos.Y - m_Height / 2),
        m_Width,
        m_Height
    };
    bool collision = CollisionHandler::GetInstance()->MapCollision(futureCollider);
    std::cout << "Collision check at X=" << futureCollider.x << ", Y=" << futureCollider.y << ": " << (collision ? "True" : "False") << std::endl;

    if (!collision) {
        m_Transform = nextPos;
        std::cout << "Enemy position updated: X=" << m_Transform.X << ", Y=" << m_Transform.Y << std::endl;
    } else {
        m_RigidBody->UnSetForce();
        std::cout << "Movement blocked by collision" << std::endl;
    }


    m_Collider->Set(m_Transform.X - offsetX, m_Transform.Y - offsetY, m_Width, m_Height);


    std::string currentTexture = (m_State == ENEMY_ATTACK) ? (playerCenterX < enemyCenterX ? "enemy_attack_left" : "enemy_attack") :
                                (m_State == ENEMY_CHASE) ? "enemy_walk" :
                                (m_State == ENEMY_PATROL) ? "enemy_walk" : "enemy_idle";
    std::cout << "Setting animation: " << currentTexture << std::endl;

    if (m_State == ENEMY_ATTACK) {

        if (playerCenterX < enemyCenterX) {
            m_Animation->SetProps("enemy_attack_left", 1, 5, 100);
            m_Flip = SDL_FLIP_HORIZONTAL;
        } else {
            m_Animation->SetProps("enemy_attack", 1, 5, 100);
            m_Flip = SDL_FLIP_NONE;
        }
    } else if (m_State == ENEMY_CHASE) {

    } else if (m_State == ENEMY_PATROL) {
        m_Animation->SetProps("enemy_walk", 1, 12, 150);
    } else {
        m_Animation->SetProps("enemy_idle", 1, 10, 150);
    }

    m_Animation->Update();
}

void Enemy::PatrolState(float dt) {
    float left = m_SpawnPoint.X - m_PatrolRange;
    float right = m_SpawnPoint.X + m_PatrolRange;

    if (m_Transform.X <= left) m_MoveDirection = 1;
    if (m_Transform.X >= right) m_MoveDirection = -1;

    m_RigidBody->AppLyForceX(m_MoveDirection * ENEMY_SPEED);
    m_Flip = (m_MoveDirection > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void Enemy::ChaseState(float dt) {
    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    int playerCenterX = playerPos.X + 64;
    int enemyCenterX = m_Transform.X + 64;
    float speed = ENEMY_SPEED * 1.5f;

    if (playerCenterX > enemyCenterX) {
        m_RigidBody->AppLyForceX(speed);
        m_Flip = SDL_FLIP_NONE;
        m_Animation->SetProps("enemy_walk", 1, 12, 120);
        std::cout << "Moving right: Speed=" << speed << std::endl;
    } else if (playerCenterX < enemyCenterX) {
        m_RigidBody->AppLyForceX(-speed);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_Animation->SetProps("enemy_walk_left", 1, 12, 120);
        std::cout << "Moving left: Speed=" << -speed << std::endl;
    }

    m_Transform.X += m_RigidBody->Position().X * dt;

    std::cout << "Chasing: PlayerCenterX=" << playerCenterX << ", EnemyCenterX=" << enemyCenterX
              << ", Speed=" << speed << ", Flip=" << m_Flip << std::endl;
}

void Enemy::AttackState(float dt) {
    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    int playerCenterX = playerPos.X + 64;
    int enemyCenterX = m_Transform.X + 64;
    int distance = abs(playerCenterX - enemyCenterX);
    m_RigidBody->UnSetForce();

    if (m_AttackCooldown > 0) {
        m_AttackCooldown -= dt;
    }

    if (distance <= ENEMY_ATTACK_RANGE && m_AttackCooldown <= 0) {
        m_IsAttacking = true;
        if (playerCenterX < enemyCenterX) {
            m_Animation->SetProps("enemy_attack_left", 1, 5, 100);
            m_Flip = SDL_FLIP_HORIZONTAL;
        } else {
            m_Animation->SetProps("enemy_attack", 1, 5, 100);
            m_Flip = SDL_FLIP_NONE;
        }
        m_AttackCooldown = ENEMY_ATTACK_COOLDOWN;
        Game::GetInstance()->DealDamageToPlayer(10);
        std::cout << "Enemy attacking: Damage dealt, Animation=" << (playerCenterX < enemyCenterX ? "enemy_attack_left" : "enemy_attack") << ", Flip=" << m_Flip << std::endl;
    } else if (distance > ENEMY_ATTACK_RANGE) {
        m_State = ENEMY_CHASE;
    }

    m_Animation->Update();
}

void Enemy::TakeDamage(int damage) {
    m_HP -= damage;
    if (m_HP <= 0) {
        m_HP = 0;
        m_State = ENEMY_DIE;
        m_Animation->SetProps("enemy_die", 1, 6, 150);
        std::cout << "Enemy died!" << std::endl;
    }
}
void Enemy::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}

int Enemy::GetHP() const {
    return m_HP;
}
