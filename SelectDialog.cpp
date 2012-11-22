
#include "SelectDialog.h"
#include "resource.h"

CSelectDialog::CSelectDialog() : CQueryWizardDialog(IDD_CHOOSEDIALOG) {
}

void CSelectDialog::Initialize( CQueryBuilder* builder ) {
	CListBox* fieldList = (CListBox*)GetDlgItem( IDC_FIELDLIST );
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	vector< string > fields = builder->GetFieldList();
	for ( int i=0; i<fields.size(); i++ ) {
		fieldList->AddString( fields[i].c_str() );
	}

	//( (CButton*)GetDlgItem( IDC_UPBUTTON ) )->EnableWindow( FALSE );
	//( (CButton*)GetDlgItem( IDC_DOWNBUTTON ) )->EnableWindow( FALSE );
	( (CButton*)GetDlgItem( IDC_BACKBUTTON ) )->EnableWindow( FALSE );

	vector<string> chosen = builder->GetSelect();
	for ( int j=0; j<chosen.size(); j++ ) {
		chosenList->AddString( chosen[j].c_str() );
	}
}

void CSelectDialog::SaveData( CQueryBuilder* builder ) {
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	vector<string> fields;
	for ( int i=0; i<chosenList->GetCount(); i++ ) {
		CString temp;
		chosenList->GetText( i, temp );
		fields.push_back( (LPCTSTR)temp );
	}
	builder->SetSelect( fields );
}

void CSelectDialog::OnAddButtonClicked() {
	CListBox* fieldList = (CListBox*)GetDlgItem( IDC_FIELDLIST );
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=fieldList->GetCount()-1; i>=0; i-- ) {
		if ( fieldList->GetSel( i ) ) {
			CString temp;
			fieldList->GetText( i, temp );
			chosenList->AddString( temp );
			fieldList->DeleteString( i );
		}
	}
}

void CSelectDialog::OnRemoveButtonClicked() {
	CListBox* fieldList = (CListBox*)GetDlgItem( IDC_FIELDLIST );
	CListBox* chosenList = (CListBox*)GetDlgItem( IDC_CHOSENLIST );
	for ( int i=chosenList->GetCount()-1; i>=0; i-- ) {
		if ( chosenList->GetSel( i ) ) {
			CString temp;
			chosenList->GetText( i, temp );
			fieldList->AddString( temp );
			chosenList->DeleteString( i );
		}
	}
}

void CSelectDialog::OnUpButtonClicked() {
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

void CSelectDialog::OnDownButtonClicked() {
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

void CSelectDialog::OnFieldListDblClk() {
	OnAddButtonClicked();
}

void CSelectDialog::OnChosenListDblClk() {
	OnRemoveButtonClicked();
}

BEGIN_MESSAGE_MAP( CSelectDialog, CQueryWizardDialog )
	ON_COMMAND( IDC_RIGHTBUTTON, OnAddButtonClicked    )
	ON_COMMAND( IDC_LEFTBUTTON,  OnRemoveButtonClicked )
	ON_COMMAND( IDC_UPBUTTON,  OnUpButtonClicked )
	ON_COMMAND( IDC_DOWNBUTTON,  OnDownButtonClicked )
	ON_LBN_DBLCLK( IDC_FIELDLIST, OnFieldListDblClk )
	ON_LBN_DBLCLK( IDC_CHOSENLIST, OnChosenListDblClk )
END_MESSAGE_MAP()