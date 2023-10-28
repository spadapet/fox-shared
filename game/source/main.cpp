#include "pch.h"
#include "source/states/app_state.h"
#include "source/ui/title_page.xaml.h"

namespace res
{
    void register_controls();
    void register_graphics();
    void register_xaml();
}

static void register_global_resources()
{
    ::res::register_controls();
    ::res::register_graphics();
    ::res::register_xaml();

    Noesis::RegisterComponent<game::title_page>();
    Noesis::RegisterComponent<game::title_page_view_model>();
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    ff::game::init_params_t<game::app_state> params{};
    params.register_global_resources = ::register_global_resources;

    return ff::game::run(params);
}
