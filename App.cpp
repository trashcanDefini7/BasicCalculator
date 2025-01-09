#include "App.hpp"

wxIMPLEMENT_APP(App);

App::App()
{
}

bool App::OnInit()
{
    m_Frame = new Main();
    m_Frame->Show();
    return true;
}
