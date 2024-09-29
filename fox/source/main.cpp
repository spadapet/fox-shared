#include "pch.h"
#include "source/states/app_state.h"
#include "source/ui/title_page.xaml.h"

static void register_global_resources()
{
    verify(ff::global_resources::add_files(ff::filesystem::executable_path().parent_path()));

    Noesis::RegisterComponent<game::title_page>();
    Noesis::RegisterComponent<game::title_page_view_model>();
}

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    return ff::game::run<game::app_state>(::register_global_resources);
}
