
#include "OrderDialog.h"
#include "resource.h"

COrderDialog::COrderDialog() : CQueryWizardDialog(IDD_CHOOSEDIALOG) {
}

void COrderDialog::Initialize( CQueryBuilder* builder ) {
	CListBox* fieldList = (CListBox*)GetDlgItem( IDC_FIELDLIST );
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	vector<string> fields = builder->GetSelect();
	for ( int i=0; i<fields.size(); i++ ) {
		fieldList->AddString( ( fields[i]+" ( Ascending  )" ).c_str() );
		fieldList->AddString( ( fields[i]+" ( Descending )" ).c_str() );
	}
	vector<string> chosen = builder->GetOrder();
	for ( int j=0; j<chosen.size(); j++ ) {
		chosenList->AddString( chosen[j].c_str() );
	}
	( (CButton*)GetDlgItem( IDOK ) )->EnableWindow( FALSE );
	( (CButton*)GetDlgItem( IDC_FINISHBUTTON ) )->SetButtonStyle( BS_DEFPUSHBUTTON );
	( (CStatic*)GetDlgItem( IDC_CHOOSEDESC ) )->SetWindowText("What fields would you like to sort your report by?");
}

void COrderDialog::SaveData( CQueryBuilder* builder ) {
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	vector<string> fields;
	for ( int i=0; i<chosenList->GetCount(); i++ ) {
		CString temp;
		chosenList->GetText( i, temp );
		fields.push_back( (LPCTSTR)temp );
	}
	builder->SetOrder( fields );
}

void COrderDialog::OnAddButtonClicked() {
	CListBox* fieldList = (CListBox*)GetDlgItem( IDC_FIELDLIST );
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=0; i<fieldList->GetCount(); i++ ) {
		if ( fieldList->GetSel( i ) ) {
			CString temp;
			fieldList->GetText( i, temp );
			chosenList->AddString( temp );
		}
	}
}

void COrderDialog::OnRemoveButtonClicked() {
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=chosenList->GetCount()-1; i>=0; i-- ) {
		if ( chosenList->GetSel( i ) ) {
			chosenList->DeleteString( i );
		}
	}
}

void COrderDialog::OnUpButtonClicked() {
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=1; i<chosenList->GetCount(); i++ ) {
		if ( chosenList->GetSel( i ) ) {
			char temp[4000];
			if ( !chosenList->GetSel( i-1 ) ) {
				chosenList->GetText( i, temp );
				chosenList->InsertString( i-1, temp );
				chosenList->SetSel( i-1, TRUE );
				chosenList->DeleteString( i+1 );
			}
		}
	}
}

void COrderDialog::OnDownButtonClicked() {
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=chosenList->GetCount()-2; i>=0; i-- ) {
		if ( chosenList->GetSel( i ) ) {
			if ( !chosenList->GetSel( i+1 ) ) {
				char temp[4000];
				chosenList->GetText( i+1, temp );
				chosenList->InsertString( i, temp );
				chosenList->SetSel( i+2, TRUE );
				chosenList->DeleteString( i+2 );
			}
		}
	}
}

void COrderDialog::OnFieldListDblClk() {
	OnAddButtonClicked();
}

void COrderDialog::OnChosenListDblClk() {
	OnRemoveButtonClicked();
}

BEGIN_MESSAGE_MAP( COrderDialog, CQueryWizardDialog )
	ON_COMMAND( IDC_RIGHTBUTTON, OnAddButtonClicked    )
	ON_COMMAND( IDC_LEFTBUTTON,  OnRemoveButtonClicked )
	ON_COMMAND( IDC_UPBUTTON,  OnUpButtonClicked )
	ON_COMMAND( IDC_DOWNBUTTON,  OnDownButtonClicked )
	ON_LBN_DBLCLK( IDC_FIELDLIST, OnFieldListDblClk )
	ON_LBN_DBLCLK( IDC_CHOSENLIST, OnChosenListDblClk )
END_MESSAGE_MAP()
