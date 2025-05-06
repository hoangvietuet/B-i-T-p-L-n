#include "Settings.h"
#include "TextureManager.h"
#include <SDL_ttf.h>
#include <string>
#include <iostream>
#include "Game.h"
#include "TextureManager.h"
#include "Input.h"
#include "Warrior.h"
#include "SDL.h"
#include "Timer.h"
#include "MapParser.h"
#include "Camera.h"
#include "CollisionHandler.h"
#include "AnimatedSprite.h"
#include "Enemy.h"
#include <cstdlib>
#include "RigidBody.h"
#include <ctime>

Settings* Settings::s_Instance = nullptr;

void Settings::Init() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cout << "Failed to initialize SDL_mixer: " << Mix_GetError() << std::endl;
        return;
    }
    m_BackgroundMusic = Mix_LoadMUS("assets/background_music.mp3");
    if (!m_BackgroundMusic) {
        std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
    } else if (m_MusicEnabled) {
        Mix_VolumeMusic(m_MusicVolume);
        Mix_PlayMusic(m_BackgroundMusic, -1);
    }
}

void Settings::ShowSettingsMenu(SDL_Renderer* renderer) {
    bool inSettings = true;
    SDL_Event event;

    const int buttonWidth = 100;
    const int buttonHeight = 100;
    const int spacing = 20;

    const int centerX = (SCREEN_WIDTH - buttonWidth) / 2; // 350

    const int totalHeight = (buttonHeight * 4) + (spacing * 3); // 460
    const int startY = (SCREEN_HEIGHT - totalHeight) / 2; // 70

    SDL_Rect playRect = {centerX, startY, buttonWidth, buttonHeight};                         // (350, 70)
    SDL_Rect musicRect = {centerX, startY + buttonHeight + spacing, buttonWidth, buttonHeight}; // (350, 190)
    SDL_Rect highScoreRect = {centerX, startY + (buttonHeight + spacing) * 2, buttonWidth, buttonHeight}; // (350, 310)
    SDL_Rect exitRect = {centerX, startY + (buttonHeight + spacing) * 3, buttonWidth, buttonHeight}; // (350, 430)

    std::cout << "Settings Menu is active..." << std::endl;

    while (inSettings) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                inSettings = false;
                Game::GetInstance()->Quit();
            }
            if (event.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);

                if (x >= exitRect.x && x <= exitRect.x + exitRect.w &&
                    y >= exitRect.y && y <= exitRect.y + exitRect.h) {
                    inSettings = false;
                    Game::GetInstance()->Quit();
                    std::cout << "Exit button clicked: Game quitting" << std::endl;
                }
                if (x >= musicRect.x && x <= musicRect.x + musicRect.w &&
                    y >= musicRect.y && y <= musicRect.y + musicRect.h) {
                    ToggleMusic(!m_MusicEnabled);
                    std::cout << "Music toggled: " << (m_MusicEnabled ? "ON" : "OFF") << std::endl;
                }
                if (x >= playRect.x && x <= playRect.x + playRect.w &&
                    y >= playRect.y && y <= playRect.y + playRect.h) {
                    inSettings = false;
                    std::cout << "Play button clicked" << std::endl;
                }
                if (x >= highScoreRect.x && x <= highScoreRect.x + highScoreRect.w &&
                    y >= highScoreRect.y && y <= highScoreRect.y + highScoreRect.h) {
                    std::cout << "High Score: 1000" << std::endl;
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderDrawRect(renderer, &exitRect);
        SDL_RenderDrawRect(renderer, &musicRect);
        SDL_RenderDrawRect(renderer, &playRect);
        SDL_RenderDrawRect(renderer, &highScoreRect);

        if (TextureManager::GetInstance()->HasTexture("exit_button")) {
            TextureManager::GetInstance()->Draw("exit_button", exitRect.x, exitRect.y, exitRect.w, exitRect.h, 1, 1, 0, SDL_FLIP_NONE);
            std::cout << "Drawing exit_button at (" << exitRect.x << ", " << exitRect.y << ") with size " << exitRect.w << "x" << exitRect.h << std::endl;
        } else {
            std::cout << "exit_button texture not found!" << std::endl;
        }
        if (TextureManager::GetInstance()->HasTexture(m_MusicEnabled ? "volume_button" : "play_pause_button")) {
            TextureManager::GetInstance()->Draw(m_MusicEnabled ? "volume_button" : "play_pause_button", musicRect.x, musicRect.y, musicRect.w, musicRect.h, 1, 1, 0, SDL_FLIP_NONE);
            std::cout << "Drawing music_button at (" << musicRect.x << ", " << musicRect.y << ") with size " << musicRect.w << "x" << musicRect.h << std::endl;
        } else {
            std::cout << "music button texture not found!" << std::endl;
        }
        if (TextureManager::GetInstance()->HasTexture("play_pause_button")) {
            TextureManager::GetInstance()->Draw("play_pause_button", playRect.x, playRect.y, playRect.w, playRect.h, 1, 1, 0, SDL_FLIP_NONE);
            std::cout << "Drawing play_button at (" << playRect.x << ", " << playRect.y << ") with size " << playRect.w << "x" << exitRect.h << std::endl;
        } else {
            std::cout << "play button texture not found!" << std::endl;
        }
        if (TextureManager::GetInstance()->HasTexture("home_button")) {
            TextureManager::GetInstance()->Draw("home_button", highScoreRect.x, highScoreRect.y, highScoreRect.w, highScoreRect.h, 1, 1, 0, SDL_FLIP_NONE);
            std::cout << "Drawing high_score_button at (" << highScoreRect.x << ", " << highScoreRect.y << ") with size " << highScoreRect.w << "x" << highScoreRect.h << std::endl;
        } else {
            std::cout << "high score button texture not found!" << std::endl;
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }
}

void Settings::Clean() {
    if (m_BackgroundMusic) {
        Mix_FreeMusic(m_BackgroundMusic);
        m_BackgroundMusic = nullptr;
    }
    Mix_CloseAudio();
}

void Settings::SetMusicVolume(int volume) {
    m_MusicVolume = std::max(0, std::min(128, volume));
    Mix_VolumeMusic(m_MusicVolume);
}

void Settings::ToggleMusic(bool enable) {
    m_MusicEnabled = enable;
    if (m_MusicEnabled && m_BackgroundMusic) {
        Mix_VolumeMusic(m_MusicVolume);
        Mix_PlayMusic(m_BackgroundMusic, -1);
    } else {
        Mix_PauseMusic();
    }
}
