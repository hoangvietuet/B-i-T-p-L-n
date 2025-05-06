#ifndef WARRIOR_H
#define WARRIOR_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector2D.h"

#define JUMP_TIME 20.0f
#define JUMP_FORCE 20.0f
#define RUN_FORCE 4.0f
#define ATTACK_TIME 20.0f

enum WarriorState {
    WARRIOR_IDLE,
    WARRIOR_WALK,
    WARRIOR_ATTACK,
    WARRIOR_DIE
};

class Warrior : public Character {
public:
    Collider* GetCollider() { return m_Collider; }

    Warrior(Properties* props);
    void Draw() override;
    void Update(float dt) override;
    void Clean() override;
    void TakeDamage(int damage);
    Vector2D GetPosition() const { return m_Transform; }
    void SetState(WarriorState state) { m_State = state; }
    WarriorState GetState() const { return m_State; }
    void SetIsAttacking(bool attacking) { m_IsAttacking = attacking; }
    bool GetIsAttacking() const { return m_IsAttacking; }
    int GetX() const { return static_cast<int>(m_Transform.X); }
    int GetY() const { return static_cast<int>(m_Transform.Y); }
    int GetHP() const { return m_HP; } // Thêm để lấy HP

private:
    void AnimationState();

private:
    bool m_IsRunning;
    bool m_IsJumping;
    bool m_IsFalling;
    bool m_IsGrounded;
    bool m_IsAttacking;
    bool m_IsCrouching;

    float m_JumpTime;
    float m_JumpForce;
    float m_AttackTime;

    Collider* m_Collider;

    Animation* m_Animation;
    RigidBody* m_RigidBody;

    Vector2D m_LastSafePosition;
    SDL_Rect rect;
    int m_HP;
    Vector2D m_Transform;
    WarriorState m_State;
};

#endif // WARRIOR_H
