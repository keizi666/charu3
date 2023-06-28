#pragma once
#include <afxwin.h>
#include <vector>
#include "Init.h"

class CTextEdit : public CDialog
{
public:
    CTextEdit(CWnd* pParent, bool newData, int kind);
    void Paste(int nCtrlID, CString strString);

private:
    enum { IDD = IDD_TEXT_EDIT };
    DECLARE_MESSAGE_MAP()
    virtual void DoDataExchange(CDataExchange* pDX);
    virtual BOOL OnInitDialog();
    virtual void OnCancel();
    afx_msg void OnSelchangeMacroCombo();
    afx_msg void OnPasteFile();
    void PasteMacro(int nCtrlID, CString strString);

    CWnd* m_pParent;
    std::vector<MACRO_STRUCT>* m_vctMacro;
    CComboBox m_ctrlMacro;
};
