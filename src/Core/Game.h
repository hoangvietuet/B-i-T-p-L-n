#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"
#include "GameMap.h"
#include "MapParser.h"
#include "Warrior.h"
#include "ENEMY.H"
#include "BloodItem.h"
#include <vector>
#ifndef ENEMY_H_FILE
#endif
#include "Settings.h"
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Game {
public:
    static Game* GetInstance() {
        return s_Instance = (s_Instance != nullptr) ? s_Instance : new Game();
    }
    bool Init();
    bool Clean();
    void Quit();
    void Update();
    void Render();
    void Events();
    void ShowSplashScreen(SDL_Renderer* renderer);

    inline GameMap* GetMap() { return m_LevelMap; }
    inline bool IsRunning() { return m_IsRunning; }
    inline bool IsGameOver() { return m_IsGameOver; }
    inline SDL_Renderer* GetRenderer() { return m_Renderer; }
    inline Enemy* GetEnemy() { return m_Enemy; }
    void DealDamageToPlayer(int damage) { if (player) player->TakeDamage(damage); }
    void DealDamageToEnemy(int damage);

private:
    Game() : m_IsRunning(false), m_IsGameOver(false), m_LevelMap(nullptr), m_Window(nullptr), m_Renderer(nullptr),
             player(nullptr), m_Enemy(nullptr), m_IsSettingsOpen(false), m_Font(nullptr), m_GameOverFont(nullptr),
             m_PlayerHPTexture(nullptr), m_EnemyHPTexture(nullptr), m_GameOverTexture(nullptr),
             m_BloodSpawnTimer(0.0f), m_BloodSpawnInterval(5.0f), m_HealingCooldown(0.0f) {}
    void UpdateHPText();
    void SpawnBloodItem();

    bool m_IsRunning;
    bool m_IsGameOver;
    GameMap* m_LevelMap;
    SDL_Window* m_Window;
    SDL_Renderer* m_Renderer;
    static Game* s_Instance;
    Warrior* player;
    Enemy* m_Enemy;
    bool m_IsSettingsOpen;

    TTF_Font* m_Font;
    TTF_Font* m_GameOverFont;
    SDL_Texture* m_PlayerHPTexture;
    SDL_Texture* m_EnemyHPTexture;
    SDL_Texture* m_GameOverTexture;
    SDL_Rect m_PlayerHPRect;
    SDL_Rect m_EnemyHPRect;
    SDL_Rect m_GameOverRect;

    std::vector<BloodItem*> m_BloodItems;
    float m_BloodSpawnTimer;
    float m_BloodSpawnInterval;

    float m_HealingCooldown; // Cooldown để tránh cộng máu liên tục
};

#endif // GAME_H
