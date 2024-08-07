#include "pch.h"
#include "source/states/app_state.h"
#include "source/ui/title_page.xaml.h"

static void register_global_resources()
{
    verify(ff::global_resources::add_files(ff::filesystem::executable_path().parent_path()));

    Noesis::RegisterComponent<game::title_page>();
    Noesis::RegisterComponent<game::title_page_view_model>();
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    ff::game::init_params_t<game::app_state> params{};
    params.register_global_resources = ::register_global_resources;

    return ff::game::run(params);
}
