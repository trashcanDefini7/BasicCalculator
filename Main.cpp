#include "Main.hpp"

#include <iomanip>
#include <sstream>

Main::Main()
    : wxFrame(nullptr, wxID_ANY, "Calculator", wxDefaultPosition, { 300, 400 }, wxDEFAULT_FRAME_STYLE ^ wxRESIZE_BORDER)
{
    SetBackgroundColour(wxColour(0xF3F3F4));
    Init();
}

void Main::Init()
{
	int lastId = 10000;

	wxBoxSizer* mainSizer;
	mainSizer = new wxBoxSizer(wxVERTICAL);

	m_Output = new wxTextCtrl(this, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(300, 50), 0);
	m_Output->SetBackgroundColour(wxColour(0x202020));
	m_Output->Enable(false);

	wxFont font(wxFontInfo(18));
	m_Output->SetFont(font);
	m_Output->SetWindowStyle(wxTE_RIGHT);

	mainSizer->Add(m_Output, 0, wxALL, 5);

	wxGridSizer* buttonsSizer;
	buttonsSizer = new wxGridSizer(4, 5, 0, 0);

	auto FormNumberButton = [&](int index)
		{
			m_NumberBtns[index] = new wxButton(this, lastId++, _(std::to_string(index)), wxDefaultPosition, wxSize(48, 48), 0);
			buttonsSizer->Add(m_NumberBtns[index], 0, wxALL, 5);
		};

	auto FormButton = [&]<class T>(wxButton** button, const char* symbol, T eventHandler)
		{
			*button = new wxButton(this, lastId++, _(symbol), wxDefaultPosition, wxSize(48, 48), 0);

			(*button)->Bind(wxEVT_COMMAND_BUTTON_CLICKED, eventHandler, this);
			buttonsSizer->Add(*button, 0, wxALL, 5);
		};
	
	// The order is reserved

	FormButton(&m_EqualsBtn, "=", &Main::OnEqualsClicked);

	for (int i = 7; i <= 9; i++)
		FormNumberButton(i);

	FormButton(&m_DivideBtn, "/", &Main::OnDivideClicked);
	FormButton(&m_SqrtBtn, "Sqrt", &Main::OnSqrtClicked);

	for (int i = 4; i <= 6; i++)
		FormNumberButton(i);

	FormButton(&m_MultiplyBtn, "*", &Main::OnMultiplyClicked);
	FormButton(&m_DelBtn, "<-", &Main::OnDelClicked);

	for (int i = 1; i <= 3; i++)
		FormNumberButton(i);

	FormButton(&m_MinusBtn, "-", &Main::OnMinusClicked);
	FormButton(&m_ClearBtn, "C", &Main::OnClearClicked);
	FormButton(&m_NumberBtns[0], "0", &Main::OnNumberClicked);
	FormButton(&m_SignBtn, "+/-", &Main::OnSignClicked);
	FormButton(&m_PointBtn, ".", &Main::OnPointClicked);
	FormButton(&m_PlusBtn, "+", &Main::OnPlusClicked);

	mainSizer->Add(buttonsSizer, 1, wxEXPAND, 5);

	SetSizer(mainSizer);
	Layout();

	Centre(wxBOTH);
	
	// Note that we start from 1 because we already attached the event handler to the "0" button
	for (int i = 1; i < 10; i++)
		m_NumberBtns[i]->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &Main::OnNumberClicked, this);
}

void Main::OnEqualsClicked(wxCommandEvent& evt)
{
	if (m_Memory.empty())
	{
		// Now we either have entered something for the first time
		// on the current operation or we haven't entered anything

		if (!m_Output->IsEmpty())
		{
			// Here we entered a number so let's save it
			m_Memory = m_Output->GetLineText(0);
		}
	}
	else
	{
		// The previous number is not empty
		// so now we either perform some operation
		// if we our output section is not empty
		// or we do nothing

		if (!m_Output->IsEmpty())
		{
			double lhs = std::strtod(m_Memory.c_str(), nullptr);
			double rhs = std::strtod(m_Output->GetLineText(0).c_str(), nullptr);
			double result;

			switch (m_BinaryOperation)
			{
			case BinaryOperation::Addition: result = lhs + rhs; break;
			case BinaryOperation::Subtraction: result = lhs - rhs; break;
			case BinaryOperation::Multiplication: result = lhs * rhs; break;
			case BinaryOperation::Division: result = lhs / rhs; break;
			}

			wxString text;
			FormatNumber(result, text);

			m_Output->SetLabelText(text);
			m_Memory = text;

			m_HasResult = true;
		}
	}

	evt.Skip();
}

void Main::OnDivideClicked(wxCommandEvent& evt)
{
	SaveToMemory(BinaryOperation::Division);
	evt.Skip();
}

void Main::OnSqrtClicked(wxCommandEvent& evt)
{
	if (!m_Output->IsEmpty())
	{
		double input = std::strtod(m_Output->GetLineText(0).c_str(), NULL);

		wxString text;
		FormatNumber(sqrt(input), text);

		m_Output->SetLabelText(text);
		m_HasResult = true;
	}

	evt.Skip();
}

void Main::OnMultiplyClicked(wxCommandEvent& evt)
{
	SaveToMemory(BinaryOperation::Multiplication);
	evt.Skip();
}

void Main::OnDelClicked(wxCommandEvent& evt)
{
	wxString text = m_Output->GetLineText(0);

	if (text.Last() == '.')
		m_HasDot = false;

	m_Output->SetLabelText(text.RemoveLast());
	evt.Skip();
}

void Main::OnMinusClicked(wxCommandEvent& evt)
{
	SaveToMemory(BinaryOperation::Subtraction);
	evt.Skip();
}

void Main::OnClearClicked(wxCommandEvent& evt)
{
	if (m_Output->IsEmpty())
	{
		if (!m_Memory.empty())
			m_Memory.clear();
	}
	else
		ResetOutput();
}

void Main::OnSignClicked(wxCommandEvent& evt)
{
	if (m_Output->GetLineLength(0) != 0)
	{
		wxString text = m_Output->GetLineText(0);

		if (text[0] == '-')
			m_Output->SetLabelText(text.SubString(1, text.Length()));
		else
			m_Output->SetLabelText("-" + text);
	}

	evt.Skip();
}

void Main::OnPointClicked(wxCommandEvent& evt)
{
	if (!m_HasDot)
	{
		m_Output->AppendText(".");
		m_HasDot = true;
	}

	evt.Skip();
}

void Main::OnPlusClicked(wxCommandEvent& evt)
{
	SaveToMemory(BinaryOperation::Addition);
	evt.Skip();
}

void Main::SaveToMemory(BinaryOperation operation)
{
	m_BinaryOperation = operation;
	m_Memory = m_Output->GetLineText(0);
	ResetOutput();
}

void Main::ResetOutput()
{
	m_HasDot = false;
	m_HasResult = false;
	m_Output->Clear();
}

void Main::OnNumberClicked(wxCommandEvent& evt)
{
	if (m_HasResult)
	{
		m_Output->Clear();
		m_HasResult = false;
	}

	int length = m_Output->GetLineLength(0);

	if (length < MAX_DIGITS)
	{
		wxTextCtrl* textCtrl = static_cast<wxTextCtrl*>(evt.GetEventObject());
		wxString text = textCtrl->GetLabelText();

		wxString output = m_Output->GetLineText(0);

		if (length != 1 || output[0] != '0')
			m_Output->AppendText(text);
	}
	
	evt.Skip();
}

void FormatNumber(double number, wxString& output)
{
	std::stringstream ss;
	ss << std::setprecision(MAX_DIGITS) << number;
	output = ss.str();
}
