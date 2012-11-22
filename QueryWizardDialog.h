
#ifndef QUERYWIZARDDIALOG_H
#define QUERYWIZARDDIALOG_H

#include <afxwin.h>
#include "QueryBuilder.h"

enum QWDialogCommand { QW_NEXT, QW_BACK, QW_EXIT, QW_FINISH };

class CQueryWizardDialog : public CDialog {
	public:
		CQueryWizardDialog( UINT nTemplateName );
		QWDialogCommand Do( CQueryBuilder* newBuilder );

		BOOL OnInitDialog();
		virtual void Initialize( CQueryBuilder* builder );

		void OnOK();
		void OnCancel();

		void OnConnection();
		virtual void OnNext();
		virtual void OnBack();
		virtual void OnExit();
		virtual void OnFinish();

		virtual void SaveData( CQueryBuilder* builder );
	protected:
		CQueryBuilder* builder;
	private:
		QWDialogCommand returnValue;
		DECLARE_MESSAGE_MAP()
};

#endif
