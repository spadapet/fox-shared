#include "pch.h"
#include "states/app_state.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    return ff::game::run<game::app_state>();
}
