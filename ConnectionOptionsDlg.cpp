
#include <afxdlgs.h>
#include "ConnectionOptionsDlg.h"
#include "DBSchema.h"
#include "Preferences.h"
#include "resource.h"

extern CDBSchema* schemaPtr;

CConnectionOptionsDlg::CConnectionOptionsDlg() : CDialog( IDD_CONNECTIONOPTIONS ) {
}

BOOL CConnectionOptionsDlg::OnInitDialog() {
	if ( !CDialog::OnInitDialog() ) {
		return FALSE;
	}

	SetDlgItemText( IDC_DSN, preferences.GetString("DSNDatabase").c_str() );
	SetDlgItemText( IDC_ACCESS, preferences.GetString("AccessDatabase").c_str() );
	SetDlgItemText( IDC_SQLSERVER, preferences.GetString("SQLServer").c_str() );
	SetDlgItemText( IDC_SQLADDRESS, preferences.GetString("SQLAddress").c_str() );
	SetDlgItemText( IDC_SQLDATABASE, preferences.GetString("SQLDatabase").c_str() );

	CheckDlgButton( IDC_PREFIX, ( preferences.GetBool("UsePrefix") ? 1 : 0 ) );

	if ( preferences.GetString("ConnectionType")=="SQL" ) {
		CheckDlgButton( IDC_SQLRADIO, 1 );
		OnSQLRadio();
	} else {
		if ( preferences.GetString("ConnectionType")=="Access" ) {
			CheckDlgButton( IDC_ACCESSRADIO, 1 );
			OnAccessRadio();
		} else {
			CheckDlgButton( IDC_DSNRADIO, 1 );
			OnDSNRadio();
		}
	}

	return TRUE;
}

void CConnectionOptionsDlg::OnOK() {
	int button = GetCheckedRadioButton( IDC_DSNRADIO, IDC_SQLRADIO );
	CString address, server, database;
	switch ( button ) {
		case IDC_DSNRADIO :
			preferences.SetString("ConnectionType","DSN");
			GetDlgItemText( IDC_DSN, database );
			preferences.SetString("DSNDatabase", (LPCTSTR)database );
			preferences.SetString("DSN",(LPCTSTR)database);
			break;
		case IDC_ACCESSRADIO :
			preferences.SetString("ConnectionType","Access");
			GetDlgItemText( IDC_ACCESS, database );
			preferences.SetString("AccessDatabase", (LPCTSTR)database);
			database = (CString)"Driver={Microsoft Access Driver (*.mdb)};Dbq=" + database + ";";
			preferences.SetString("DSN",(LPCTSTR)database);
			break;
		case IDC_SQLRADIO :
			preferences.SetString("ConnectionType","SQL");
			GetDlgItemText( IDC_SQLSERVER, server );
			GetDlgItemText( IDC_SQLADDRESS, address );
			GetDlgItemText( IDC_SQLDATABASE, database );
			preferences.SetString("SQLServer", (LPCTSTR)server);
			preferences.SetString("SQLAddress", (LPCTSTR)address);
			preferences.SetString("SQLDatabase", (LPCTSTR)database);
			database = "Driver={SQL Server};Server=" + server + ";Address=" + address + ";Network=DBMSSOCN;Database=" + database + ";";
			preferences.SetString("DSN",(LPCTSTR)database);
			break;
	}
	preferences.SetBool( "UsePrefix", ( ((CButton*)GetDlgItem( IDC_PREFIX ))->CButton::GetCheck() ? true : false ) );
	if ( !schemaPtr->LoadFromFile( preferences.GetHomePath() + "studentinfo.xml" ) ) {
		MessageBox(  "The configuration file (studentinfo.xml) is missing or corrupted!", "" );
		exit( -1 );
	}

	CDialog::OnOK();
}

void CConnectionOptionsDlg::OnDSNRadio() {
	GetDlgItem(IDC_DSN)->EnableWindow( TRUE );
	GetDlgItem(IDC_ACCESS)->EnableWindow( FALSE );
	GetDlgItem(IDC_BROWSE)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLSERVER)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLADDRESS)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLDATABASE)->EnableWindow( FALSE );
}

void CConnectionOptionsDlg::OnAccessRadio() {
	GetDlgItem(IDC_DSN)->EnableWindow( FALSE );
	GetDlgItem(IDC_ACCESS)->EnableWindow( TRUE );
	GetDlgItem(IDC_BROWSE)->EnableWindow( TRUE );
	GetDlgItem(IDC_SQLSERVER)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLADDRESS)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLDATABASE)->EnableWindow( FALSE );
}

void CConnectionOptionsDlg::OnSQLRadio() {
	GetDlgItem(IDC_DSN)->EnableWindow( FALSE );
	GetDlgItem(IDC_ACCESS)->EnableWindow( FALSE );
	GetDlgItem(IDC_BROWSE)->EnableWindow( FALSE );
	GetDlgItem(IDC_SQLSERVER)->EnableWindow( TRUE );
	GetDlgItem(IDC_SQLADDRESS)->EnableWindow( TRUE );
	GetDlgItem(IDC_SQLDATABASE)->EnableWindow( TRUE );
}

void CConnectionOptionsDlg::OnBrowseClicked() {
	CFileDialog openDlg(
		TRUE,
		"mdb",
		NULL,
		OFN_HIDEREADONLY,
		"Microsoft Access Files (*.mdb)|*.mdb|All Files (*.*)|*.*||",
		this
	);
	if ( openDlg.DoModal() == IDOK ) {
		SetDlgItemText( IDC_ACCESS, (LPCTSTR)openDlg.GetPathName() );
	}
}

BEGIN_MESSAGE_MAP( CConnectionOptionsDlg, CDialog )
	ON_COMMAND( IDC_DSNRADIO, OnDSNRadio )
	ON_COMMAND( IDC_ACCESSRADIO, OnAccessRadio )
	ON_COMMAND( IDC_SQLRADIO, OnSQLRadio )
	ON_COMMAND( IDC_BROWSE, OnBrowseClicked )
END_MESSAGE_MAP()