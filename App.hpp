#pragma once

#include <wx/wx.h>

#include "Main.hpp"

class App : public wxApp
{
public:
    App();

    bool OnInit() override;

private:
    Main* m_Frame = nullptr;

};

