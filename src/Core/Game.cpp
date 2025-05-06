#include "Game.h"
#include "TextureManager.h"
#include "Input.h"
#include "Warrior.h"
#include "SDL.h"
#include "Timer.h"
#include "MapParser.h"
#include <iostream>
#include "Camera.h"
#include "CollisionHandler.h"
#include "AnimatedSprite.h"
#include <cstdlib>
#include "Settings.h"
#include <sstream>
#include "ENEMY.h"
const int groundY = 500;
Game* Game::s_Instance = nullptr;

bool Game::Init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0 || IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG) == 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return false;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        return false;
    }

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    m_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags);
    if (!m_Window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!m_Renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    m_Font = TTF_OpenFont("assets/arial.ttf", 24);
    if (!m_Font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return false;
    }

    m_GameOverFont = TTF_OpenFont("assets/arial.ttf", 48);
    if (!m_GameOverFont) {
        SDL_Log("Failed to load game over font: %s", TTF_GetError());
        return false;
    }

    Settings::GetInstance()->Init();
    SDL_Delay(100);
    ShowSplashScreen(m_Renderer);

    if (!MapParser::GetInstance()->Load()) {
        std::cout << "Failed to load map" << std::endl;
        return false;
    }

    TextureManager::GetInstance()->ParseTextures("assets/textures.tml");

    player = new Warrior(new Properties("player", 100, 200, 136, 96));
    m_Enemy = new Enemy(new Properties("enemy_idle", 300, 300, 128, 128));
    m_LevelMap = MapParser::GetInstance()->GetMap("level1");

    if (m_Enemy) {
        std::cout << "Enemy initialized successfully! Pointer: " << m_Enemy << std::endl;
    } else {
        std::cout << "Failed to initialize enemy!" << std::endl;
    }

    Camera::GetInstance()->SetTarget(player->GetOrigin());
    UpdateHPText();

    return m_IsRunning = true;
}

bool Game::Clean() {
    TextureManager::GetInstance()->Clean();
    Settings::GetInstance()->Clean();
    SDL_DestroyTexture(m_PlayerHPTexture);
    SDL_DestroyTexture(m_EnemyHPTexture);
    SDL_DestroyTexture(m_GameOverTexture);
    TTF_CloseFont(m_Font);
    TTF_CloseFont(m_GameOverFont);
    TTF_Quit();
    SDL_DestroyRenderer(m_Renderer);
    SDL_DestroyWindow(m_Window);
    IMG_Quit();
    SDL_Quit();

    if (player) {
        player->Clean();
        delete player;
        player = nullptr;
    }

    if (m_Enemy) {
        m_Enemy->Clean();
        delete m_Enemy;
        m_Enemy = nullptr;
    }

    return true;
}

void Game::Quit() {
    m_IsRunning = false;
}

void Game::UpdateHPText() {
    SDL_DestroyTexture(m_PlayerHPTexture);
    SDL_DestroyTexture(m_EnemyHPTexture);

    std::stringstream ss;
    ss << "Player HP: " << (player ? player->GetHP() : 0);
    SDL_Surface* surface = TTF_RenderText_Solid(m_Font, ss.str().c_str(), {255, 255, 255});
    m_PlayerHPTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    SDL_QueryTexture(m_PlayerHPTexture, nullptr, nullptr, &m_PlayerHPRect.w, &m_PlayerHPRect.h);
    m_PlayerHPRect.x = 10;
    m_PlayerHPRect.y = 10;
    SDL_FreeSurface(surface);

    ss.str("");
    ss << "Enemy HP: " << (m_Enemy ? m_Enemy->GetHP() : 0);
    surface = TTF_RenderText_Solid(m_Font, ss.str().c_str(), {255, 255, 255});
    m_EnemyHPTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
    SDL_QueryTexture(m_EnemyHPTexture, nullptr, nullptr, &m_EnemyHPRect.w, &m_EnemyHPRect.h);
    m_EnemyHPRect.x = SCREEN_WIDTH - m_EnemyHPRect.w - 10;
    m_EnemyHPRect.y = 10;
    SDL_FreeSurface(surface);
}

void Game::Render() {
    std::cout << "Render function called..." << std::endl;
    SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_Renderer);

    if (!m_IsSettingsOpen) {
        TextureManager::GetInstance()->Draw("bg1", 0, 0, 1920, 1080, 1, 1, 0.4);
        m_LevelMap->Render();
        player->Draw();
        m_Enemy->Draw();
        if (m_PlayerHPTexture) SDL_RenderCopy(m_Renderer, m_PlayerHPTexture, nullptr, &m_PlayerHPRect);
        if (m_EnemyHPTexture) SDL_RenderCopy(m_Renderer, m_EnemyHPTexture, nullptr, &m_EnemyHPRect);
        if (m_IsGameOver && m_GameOverTexture) {
            SDL_RenderCopy(m_Renderer, m_GameOverTexture, nullptr, &m_GameOverRect);
        }
    }

    SDL_RenderPresent(m_Renderer);
}

void Game::Update() {
    if (m_IsGameOver) {
        return;
    }

    float dt = Timer::GetInstance()->GetDeltaTime();
    player->Update(dt);
    m_Enemy->Update(dt);
    Camera::GetInstance()->SetTarget(player->GetOrigin());
    Camera::GetInstance()->Update(dt);
    m_LevelMap->Update();
    UpdateHPText();

    if ((player && player->GetHP() <= 0) || (m_Enemy && m_Enemy->GetHP() <= 0)) {
        if (!m_IsGameOver) {
            m_IsGameOver = true;
            std::cout << "Game Over: " << (player->GetHP() <= 0 ? "Player HP=0" : "Enemy HP=0") << std::endl;

            SDL_Surface* surface = TTF_RenderText_Solid(m_GameOverFont, "Victory", {255, 255, 0});
            if (surface) {
                m_GameOverTexture = SDL_CreateTextureFromSurface(m_Renderer, surface);
                SDL_QueryTexture(m_GameOverTexture, nullptr, nullptr, &m_GameOverRect.w, &m_GameOverRect.h);
                m_GameOverRect.x = (SCREEN_WIDTH - m_GameOverRect.w) / 2;
                m_GameOverRect.y = (SCREEN_HEIGHT - m_GameOverRect.h) / 2;
                SDL_FreeSurface(surface);
                std::cout << "Game Over Rect: X=" << m_GameOverRect.x << ", Y=" << m_GameOverRect.y << ", W=" << m_GameOverRect.w << ", H=" << m_GameOverRect.h << std::endl;
            } else {
                std::cout << "Failed to create GAME OVER texture: " << TTF_GetError() << std::endl;
            }
        }
    }
}

void Game::Events() {
    Input::GetInstance()->Listen();
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S)) {
        Settings::GetInstance()->ShowSettingsMenu(m_Renderer);
        m_IsSettingsOpen = false;
    }
}

void Game::ShowSplashScreen(SDL_Renderer* renderer) {
    const int numFrames = 7;
    const int frameDuration = 100;
    SDL_Texture* frames[numFrames];

    for (int i = 0; i < numFrames; ++i) {
        std::string path = "assets/splash_" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("Failed to load frame %d: %s", i, IMG_GetError());
            return;
        }
        frames[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    bool done = false;
    SDL_Event event;
    int currentFrame = 0;
    Uint32 lastTime = SDL_GetTicks();

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                done = true;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastTime >= frameDuration) {
            currentFrame = (currentFrame + 1) % numFrames;
            lastTime = now;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frames[currentFrame], nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);
        if (now - lastTime > 3000) {
            done = true;
        }
    }

    for (int i = 0; i < numFrames; ++i) {
        SDL_DestroyTexture(frames[i]);
    }
}

void Game::DealDamageToEnemy(int damage) {
    if (m_Enemy) {
        std::cout << "Calling Enemy::TakeDamage with damage=" << damage << std::endl;
        m_Enemy->TakeDamage(damage);
        UpdateHPText();
        std::cout << "Enemy hit: Damage=" << damage << ", New HP=" << m_Enemy->GetHP() << std::endl;
    } else {
        std::cout << "No Enemy to damage! m_Enemy is null" << std::endl;
    }
}
