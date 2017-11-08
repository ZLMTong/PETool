#pragma once


// CPELoadFileList

class CPELoadFileList : public CListBox
{
	DECLARE_DYNAMIC(CPELoadFileList)

public:
	CPELoadFileList();
	virtual ~CPELoadFileList();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


