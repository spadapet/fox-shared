#include "pch.h"
#include "source/states/app_state.h"

static const std::string_view NOESIS_NAME = "d704047b-5bd2-4757-9858-6a7d86cdd006";
static const std::string_view NOESIS_KEY = "rl9dmPcVXZhzCS6KAX6FhQC3A7UT/jOt7AJ5uB+5MkgTRvl0";

namespace res
{
    void register_controls();
    void register_graphics();
    void register_xaml();
}

static std::shared_ptr<ff::game::app_state_base> create_app_state()
{
    return std::make_shared<fox::app_state>();
}

static void register_global_resources()
{
    ::res::register_controls();
    ::res::register_graphics();
    ::res::register_xaml();
}

static void register_noesis_components()
{
    // Noesis::RegisterComponent<Noesis::EnumConverter<fox::game_flags>>();
    // Noesis::RegisterComponent(Noesis::TypeOf<fox::debug_page>(), nullptr);
}

static ff::game::init_params get_game_init_params()
{
    ff::game::init_params params{};
    params.create_initial_state = ::create_app_state;
    params.noesis_application_resources_name = "application_resources.xaml";
    params.noesis_default_font = "Segoe UI";
    params.noesis_default_font_size = 8;
    params.noesis_license_key = ::NOESIS_KEY;
    params.noesis_license_name = ::NOESIS_NAME;
    params.register_global_resources = ::register_global_resources;
    params.register_noesis_components = ::register_noesis_components;

    return params;
}

#if UWP_APP
int main(Platform::Array<Platform::String^>^)
#else
int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
#endif
{
    return ff::game::run(::get_game_init_params());
}
