#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
class Settings {
public:
    static Settings* GetInstance() {
        if (!s_Instance) s_Instance = new Settings();
        return s_Instance;
    }

    void Init();
    void ShowSettingsMenu(SDL_Renderer* renderer);
    void Clean();

    void SetMusicVolume(int volume);
    int GetMusicVolume() const { return m_MusicVolume; }
    void ToggleMusic(bool enable);
    bool IsMusicEnabled() const { return m_MusicEnabled; }
    bool HasTexture(std::string id); // Thêm hàm kiểm tra texture
private:
    Settings() : m_MusicVolume(50), m_MusicEnabled(true), m_BackgroundMusic(nullptr) {}
    static Settings* s_Instance;

    int m_MusicVolume;
    bool m_MusicEnabled;
    Mix_Music* m_BackgroundMusic;
};
