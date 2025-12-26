#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "Audio.h"
#include <vector>
#include <algorithm>
#include <iostream>

static ma_engine g_Engine;
static bool g_Initialized = false;

/*
    ACTIVE STACKED SOUNDS
    ---------------------
    Every SFX / PlayFromTime call allocates its own ma_sound
    and stays alive until playback finishes.
*/
static std::vector<ma_sound*> g_ActiveSounds;

/*
    SINGLE LOOPING SOUND
    --------------------
    Music / ambience is intentionally NOT stacked.
*/
static ma_sound g_LoopSound;
static bool g_LoopPlaying = false;

// ------------------------------------------------------------
// INTERNAL CLEANUP
// ------------------------------------------------------------
static void CleanupFinishedSounds()
{
    g_ActiveSounds.erase(
        std::remove_if(
            g_ActiveSounds.begin(),
            g_ActiveSounds.end(),
            [](ma_sound* s)
            {
                if (!ma_sound_is_playing(s))
                {
                    ma_sound_uninit(s);
                    delete s;
                    return true;
                }
                return false;
            }),
        g_ActiveSounds.end()
    );
}

// ------------------------------------------------------------
// INIT / SHUTDOWN
// ------------------------------------------------------------
namespace Audio
{
    bool Init()
    {
        if (ma_engine_init(nullptr, &g_Engine) != MA_SUCCESS)
        {
            std::cerr << "[Audio] Failed to initialize miniaudio\n";
            return false;
        }

        g_Initialized = true;
        std::cout << "[Audio] Initialized\n";
        return true;
    }

    void Shutdown()
    {
        if (!g_Initialized)
            return;

        // Stop looping sound
        StopLoop();

        // Cleanup stacked sounds
        for (ma_sound* s : g_ActiveSounds)
        {
            ma_sound_uninit(s);
            delete s;
        }
        g_ActiveSounds.clear();

        ma_engine_uninit(&g_Engine);
        g_Initialized = false;

        std::cout << "[Audio] Shutdown\n";
    }

    void Update()
    {
        if (!g_Initialized)
            return;

        CleanupFinishedSounds();
    }

    // ------------------------------------------------------------
    // FIRE-AND-FORGET SFX (STACKS)
    // ------------------------------------------------------------
    void Play(const std::string& path, float volume, float pitch)
    {
        if (!g_Initialized)
            return;

        ma_sound* sound = new ma_sound;

        if (ma_sound_init_from_file(
            &g_Engine,
            path.c_str(),
            0,              // NOT streaming (best for SFX)
            nullptr,
            nullptr,
            sound
        ) != MA_SUCCESS)
        {
            delete sound;
            return;
        }

        ma_sound_set_volume(sound, volume);
        ma_sound_set_pitch(sound, pitch);
        ma_sound_start(sound);

        g_ActiveSounds.push_back(sound);
    }

    // ------------------------------------------------------------
    // LOOPING SOUND (SINGLE INSTANCE)
    // ------------------------------------------------------------
    void PlayLoop(const std::string& path, float volume, float pitch)
    {
        if (!g_Initialized)
            return;

        if (g_LoopPlaying)
        {
            ma_sound_uninit(&g_LoopSound);
            g_LoopPlaying = false;
        }

        if (ma_sound_init_from_file(
            &g_Engine,
            path.c_str(),
            MA_SOUND_FLAG_STREAM,   // REQUIRED for MP3 / OGG
            nullptr,
            nullptr,
            &g_LoopSound
        ) != MA_SUCCESS)
        {
            std::cerr << "[Audio] Failed to load looping sound\n";
            return;
        }

        ma_sound_set_looping(&g_LoopSound, MA_TRUE);
        ma_sound_set_volume(&g_LoopSound, volume);
        ma_sound_set_pitch(&g_LoopSound, pitch);
        ma_sound_start(&g_LoopSound);

        g_LoopPlaying = true;
    }

    void StopLoop()
    {
        if (!g_LoopPlaying)
            return;

        ma_sound_stop(&g_LoopSound);
        ma_sound_uninit(&g_LoopSound);
        g_LoopPlaying = false;
    }

    // ------------------------------------------------------------
    // PLAY FROM TIME (STACKS)
    // ------------------------------------------------------------
    void PlayFromTime(
        const std::string& path,
        float startSeconds,
        float volume,
        float pitch
    )
    {
        if (!g_Initialized)
            return;

        ma_sound* sound = new ma_sound;

        if (ma_sound_init_from_file(
            &g_Engine,
            path.c_str(),
            MA_SOUND_FLAG_STREAM,   // REQUIRED for seeking
            nullptr,
            nullptr,
            sound
        ) != MA_SUCCESS)
        {
            delete sound;
            return;
        }

        ma_uint64 sampleRate = ma_engine_get_sample_rate(&g_Engine);
        ma_uint64 startFrame =
            static_cast<ma_uint64>(startSeconds * sampleRate);

        // DO NOT clamp using totalFrames for streamed sounds
        ma_sound_seek_to_pcm_frame(sound, startFrame);


        ma_sound_seek_to_pcm_frame(sound, startFrame);
        ma_sound_set_volume(sound, volume);
        ma_sound_set_pitch(sound, pitch);
        ma_sound_start(sound);

        g_ActiveSounds.push_back(sound);
    }
}
