#pragma once

namespace NBHS
{
    class RaceMenuIntegration
    {
    public:
        static RaceMenuIntegration& GetSingleton();
        bool Initialize();
        bool IsAvailable() const noexcept { return available_; }

    private:
        bool available_{ false };
    };
}
