#pragma once
#include <string>

namespace Audio
{
    // Engine lifetime
    bool Init();
    void Shutdown();

    // Must be called once per frame
    void Update();

    // Fire-and-forget SFX (STACKS)
    void Play(
        const std::string& path,
        float volume = 1.0f,
        float pitch  = 1.0f
    );

    // Looping sound (music / ambience)
    // Only one active loop at a time (intentional)
    void PlayLoop(
        const std::string& path,
        float volume = 1.0f,
        float pitch  = 1.0f
    );
    void StopLoop();

    // Play sound starting from a given time (STACKS)
    void PlayFromTime(
        const std::string& path,
        float startSeconds,
        float volume = 1.0f,
        float pitch  = 1.0f
    );
}
