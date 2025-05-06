#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector2D.h"

#define ENEMY_SPEED 5.0f
#define ENEMY_ATTACK_RANGE 50.0f
#define ENEMY_CHASE_RANGE 500.0f
#define ENEMY_ATTACK_COOLDOWN 10.0f

enum EnemyState {
    ENEMY_IDLE,
    ENEMY_PATROL,
    ENEMY_CHASE,
    ENEMY_ATTACK,
    ENEMY_DIE
};

class Enemy : public Character {
public:
    Collider* GetCollider() { return m_Collider; }

    Enemy(Properties* props);
    void Draw() override;
    void Update(float dt) override;
    void Clean() override;
    void TakeDamage(int damage);
    Vector2D GetPosition() const { return m_Transform; }
    void SetState(EnemyState state) { m_State = state; }
    EnemyState GetState() const { return m_State; }
    int GetHP() const;

private:
    void PatrolState(float dt);
    void ChaseState(float dt);
    void AttackState(float dt);

private:
    bool m_IsAttacking;
    float m_AttackCooldown;

    Collider* m_Collider;

    Animation* m_Animation;
    RigidBody* m_RigidBody;

    Vector2D m_Transform;
    Vector2D m_TargetPosition;
    EnemyState m_State;

    int m_HP;
    int m_MoveDirection;
    Vector2D m_SpawnPoint;
    float m_PatrolRange;
};

#endif // ENEMY_H
