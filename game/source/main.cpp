#include "pch.h"
#include "source/states/app_state.h"
#include "source/ui/title_page.xaml.h"

static void register_global_resources()
{
    // TODO: Make helper method to automatically load all assets from EXE's directory
    std::filesystem::path assets_path = ff::filesystem::executable_path().parent_path() / "game.res.pack";
    std::shared_ptr<ff::data_base> assets_data = ff::filesystem::map_binary_file(assets_path);
    assert(assets_data);

    if (assets_data)
    {
        ff::data_reader assets_reader(assets_data);
        ff::global_resources::add(assets_reader);
    }

    Noesis::RegisterComponent<game::title_page>();
    Noesis::RegisterComponent<game::title_page_view_model>();
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    ff::game::init_params_t<game::app_state> params{};
    params.register_global_resources = ::register_global_resources;

    return ff::game::run(params);
}
