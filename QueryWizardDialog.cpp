
#include "QueryWizardDialog.h"
#include "QueryWizard.h"
#include "ConnectionOptionsDlg.h"
#include "resource.h"

CQueryWizardDialog::CQueryWizardDialog( UINT nTemplateName ) :CDialog(nTemplateName) {
}

QWDialogCommand CQueryWizardDialog::Do( CQueryBuilder* newBuilder ) {
	builder = newBuilder;
	CDialog::DoModal();
	return returnValue;
}

BOOL CQueryWizardDialog::OnInitDialog() {
	BOOL retval = CDialog::OnInitDialog();
	Initialize( builder );
	return retval;
}

void CQueryWizardDialog::Initialize( CQueryBuilder* builder ) {
}

void CQueryWizardDialog::OnOK() {
	OnNext();
	CDialog::OnOK();
}

void CQueryWizardDialog::OnCancel() {
	OnExit();
	CDialog::OnOK();
}

void CQueryWizardDialog::OnConnection() {
	CConnectionOptionsDlg dlg;
	dlg.DoModal();
}

void CQueryWizardDialog::OnNext() {
	returnValue = QW_NEXT;
	SaveData( builder );
	CDialog::OnOK();
}

void CQueryWizardDialog::OnBack() {
	returnValue = QW_BACK;
	SaveData( builder );
	CDialog::OnOK();
}

void CQueryWizardDialog::OnExit() {
	returnValue = QW_EXIT;
	SaveData( builder );
	CDialog::OnOK();
}

void CQueryWizardDialog::OnFinish() {
	//returnValue = QW_FINISH;
	SaveData( builder );
	((CQueryWizard*)::AfxGetApp())->ExecuteQuery( *builder );
	//CDialog::OnOK();
}

void CQueryWizardDialog::SaveData( CQueryBuilder* builder ) {
}

BEGIN_MESSAGE_MAP( CQueryWizardDialog, CDialog )
	ON_COMMAND( IDC_CONNECTION, OnConnection )
	ON_COMMAND( IDC_FINISHBUTTON, OnFinish )
	ON_COMMAND( IDC_BACKBUTTON, OnBack )
END_MESSAGE_MAP()
