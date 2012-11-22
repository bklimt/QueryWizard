
#include "WhereDialog.h"
#include "resource.h"

#define GetCB( a, b ) CComboBox* a = (CComboBox*)GetDlgItem( b );

#define GetCBSet( num ) \
	GetCB( var ## num, IDC_VAR ## num ) \
	GetCB( comp ## num , IDC_COMP ## num ) \
	GetCB( value ## num , IDC_VALUE ## num )

#define GetAll \
	GetCBSet(1) \
	GetCBSet(2) \
	GetCBSet(3) \
	GetCBSet(4) \
	GetCBSet(5) \
	GetCBSet(6) \
	GetCBSet(7) \
	GetCBSet(8) \
	GetCBSet(9) \
	GetCBSet(10)

CWhereDialog::CWhereDialog( bool isAllDialog ) : CQueryWizardDialog(IDD_TESTDIALOG) {
	isAll = isAllDialog;
}

string GetText( CWnd* control ) {
	CString temp;
	control->GetWindowText( temp );
	return (LPCTSTR)temp;
}

void CWhereDialog::Initialize( CQueryBuilder* builder ) {
	if ( !isAll ) {
		( (CStatic*)GetDlgItem( IDC_TESTDESC ) )->SetWindowText("Only show records of students for whom AT LEAST ONE of the following criteria is true:");
	}

	GetAll

	vector<string> fields = builder->GetFieldList();
	fields.push_back( "" );

	for ( int i=0; i<fields.size(); i++ ) {
		var1->AddString( fields[i].c_str() );
		var2->AddString( fields[i].c_str() );
		var3->AddString( fields[i].c_str() );
		var4->AddString( fields[i].c_str() );
		var5->AddString( fields[i].c_str() );
		var6->AddString( fields[i].c_str() );
		var7->AddString( fields[i].c_str() );
		var8->AddString( fields[i].c_str() );
		var9->AddString( fields[i].c_str() );
		var10->AddString( fields[i].c_str() );
	}

	vector< vector< string > > clauses;
	if ( isAll ) {
		clauses = builder->GetWhereAll();
	} else {
		clauses = builder->GetWhereAny();
	}

#define LOAD_SET( num ) \
	{ \
		var ## num ## ->SelectString( -1, clauses[ num - 1 ][ 0 ].c_str() ); \
		comp ## num ## ->SelectString( -1, clauses[ num - 1 ][ 1 ].c_str() ); \
		value ## num ## ->SetWindowText( clauses[ num - 1 ][ 2 ].c_str() ); \
	}

	LOAD_SET(1)
	LOAD_SET(2)
	LOAD_SET(3)
	LOAD_SET(4)
	LOAD_SET(5)
	LOAD_SET(6)
	LOAD_SET(7)
	LOAD_SET(8)
	LOAD_SET(9)
	LOAD_SET(10)
}

void CWhereDialog::SaveData( CQueryBuilder* builder ) {
	GetAll

	vector< vector< string > > data( 10, vector<string>( 3, "" ) );

#define SAVE_SET(num) \
	{ \
		data[ num - 1 ][ 0 ] = GetText( var ## num ); \
		data[ num - 1 ][ 1 ] = GetText( comp ## num ); \
		data[ num - 1 ][ 2 ] = GetText( value ## num ); \
	}

	SAVE_SET(1)
	SAVE_SET(2)
	SAVE_SET(3)
	SAVE_SET(4)
	SAVE_SET(5)
	SAVE_SET(6)
	SAVE_SET(7)
	SAVE_SET(8)
	SAVE_SET(9)
	SAVE_SET(10)

	if ( isAll ) {
		builder->SetWhereAll( data );
	} else {
		builder->SetWhereAny( data );
	}
}

#define FIELD_HANDLER( num ) \
	void CWhereDialog::OnFieldChosen ## num ## () { \
		CComboBox* var ## num = (CComboBox*)GetDlgItem( IDC_VAR ## num ); \
		CComboBox* value ## num = (CComboBox*)GetDlgItem( IDC_VALUE ## num ); \
		string temp = GetText( value ## num ); \
		if ( builder->HasList( GetText( var ## num ) ) ) { \
			BeginWaitCursor(); \
			vector<string> dataList = builder->GetList( GetText( var ## num ) ); \
			value ## num ## ->ResetContent(); \
			for ( int i=0; i<dataList.size(); i++ ) {\
				value ## num ## ->AddString( dataList[i].c_str() ); \
			} \
			EndWaitCursor(); \
		} else { \
			value ## num ## ->ResetContent(); \
		} \
		value ## num ## ->SetWindowText( temp.c_str() ); \
	}

FIELD_HANDLER(1)
FIELD_HANDLER(2)
FIELD_HANDLER(3)
FIELD_HANDLER(4)
FIELD_HANDLER(5)
FIELD_HANDLER(6)
FIELD_HANDLER(7)
FIELD_HANDLER(8)
FIELD_HANDLER(9)
FIELD_HANDLER(10)

BEGIN_MESSAGE_MAP( CWhereDialog, CQueryWizardDialog )
	ON_CBN_KILLFOCUS( IDC_VAR1, OnFieldChosen1 )
	ON_CBN_KILLFOCUS( IDC_VAR2, OnFieldChosen2 )
	ON_CBN_KILLFOCUS( IDC_VAR3, OnFieldChosen3 )
	ON_CBN_KILLFOCUS( IDC_VAR4, OnFieldChosen4 )
	ON_CBN_KILLFOCUS( IDC_VAR5, OnFieldChosen5 )
	ON_CBN_KILLFOCUS( IDC_VAR6, OnFieldChosen6 )
	ON_CBN_KILLFOCUS( IDC_VAR7, OnFieldChosen7 )
	ON_CBN_KILLFOCUS( IDC_VAR8, OnFieldChosen8 )
	ON_CBN_KILLFOCUS( IDC_VAR9, OnFieldChosen9 )
	ON_CBN_KILLFOCUS( IDC_VAR10, OnFieldChosen10 )
END_MESSAGE_MAP()
