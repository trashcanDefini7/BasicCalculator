#pragma once

#include <wx/wx.h>

enum class BinaryOperation
{
	None,
	Addition,
	Subtraction,
	Multiplication,
	Division,
};

constexpr int MAX_DIGITS = 15;

void FormatNumber(double number, wxString& output);

class Main : public wxFrame
{
public:
    Main();

private:
    void Init();

	void OnNumberClicked(wxCommandEvent& evt);

	void OnEqualsClicked(wxCommandEvent& evt);
	void OnDivideClicked(wxCommandEvent& evt);
	void OnSqrtClicked(wxCommandEvent& evt);
	void OnMultiplyClicked(wxCommandEvent& evt);
	void OnDelClicked(wxCommandEvent& evt);
	void OnMinusClicked(wxCommandEvent& evt);
	void OnClearClicked(wxCommandEvent& evt);
	void OnSignClicked(wxCommandEvent& evt);
	void OnPointClicked(wxCommandEvent& evt);
	void OnPlusClicked(wxCommandEvent& evt);

	void SaveToMemory(BinaryOperation operation);
	void ResetOutput();

private:
	wxTextCtrl* m_Output;
	wxButton* m_EqualsBtn;
	wxButton* m_DivideBtn;
	wxButton* m_SqrtBtn;
	wxButton* m_MultiplyBtn;
	wxButton* m_DelBtn;
	wxButton* m_MinusBtn;
	wxButton* m_ClearBtn;
	wxButton* m_SignBtn;
	wxButton* m_PointBtn;
	wxButton* m_PlusBtn;

	wxButton* m_NumberBtns[10];

	wxString m_Memory = "0";
	BinaryOperation m_BinaryOperation = BinaryOperation::None;

	bool m_HasResult = true;

};
