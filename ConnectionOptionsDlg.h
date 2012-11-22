
#include <afxwin.h>

class CConnectionOptionsDlg : public CDialog {
	public:
		CConnectionOptionsDlg();
		BOOL OnInitDialog();
		void OnOK();

		void OnDSNRadio();
		void OnAccessRadio();
		void OnSQLRadio();

		void OnBrowseClicked();
	private:
		DECLARE_MESSAGE_MAP()
};
