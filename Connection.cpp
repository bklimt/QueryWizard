
#include "Connection.h"
#include "Preferences.h"
#include <afxdisp.h>

CConnection::CConnection() {
	CoInitialize( NULL );
	open = false;
}

CConnection::CConnection(string dsn, string uid, string pass) {
	CoInitialize( NULL );
	open = false;
	Open(dsn,uid,pass);
}

CConnection::~CConnection() {
	Close();
	CoUninitialize();
}

bool CConnection::Open(string dsn, string uid, string pass) {
	if (open) Close();
	try {
		int result = conn.CreateInstance( __uuidof( Connection ) );
		if (result != S_OK) {
			char temp[100];
			sprintf( temp, "Error: 0x%x in WinError.h", result );
			MessageBox( 0, temp, "", 0 );
			return false;
		}
		conn->Open( dsn.c_str(), uid.c_str(), pass.c_str(), adConnectUnspecified );
		open = true;
	} catch ( ... ) {
		return false;
	}
	return true;
}

string CConnection::GetDSN() {
	return preferences.GetString("DSN");
}

bool CConnection::OpenForReading() {
	return Open( GetDSN(), "StudentInfoReader", "bryanklimtreader" );
}

bool CConnection::OpenForWriting() {
	return Open( GetDSN(), "StudentInfoWriter", "bryanklimtwriter" );
}

bool CConnection::OpenForAuthReading() {
	return Open( GetDSN(), "StudentInfoReader", "bryanklimtreader" );
}

bool CConnection::OpenForAuthWriting() {
	return Open( GetDSN(), "StudentInfoWriter", "bryanklimtwriter" );
}

void CConnection::Close() {
	if (open) {
		try {
			conn->Close();
		}
		catch (...) {
		}
		conn.Release();
		open = false;
	}
}

CTable CConnection::Execute( string sql ) {
	CTable ans;
	_RecordsetPtr rs;

	_variant_t ra( 0L );
	//MessageBox( 0, sql.c_str(), "", 0 );
	rs = conn->Execute( sql.c_str(), &ra, adOptionUnspecified );

	vector< string > fields;
	vector< vector< CVariant > > data;

	if ( rs->State != adStateClosed ) {
		if ( !rs->GetadoEOF() ) {
			for ( int i=0; i<rs->GetFields()->GetCount(); i++ ) {
				string fieldName = (LPCTSTR)rs->GetFields()->GetItem((long)i)->Name;
				fields.push_back( fieldName );
			}
		}

		while ( !rs->GetadoEOF() ) {
			vector< CVariant > row;
			for ( int i=0; i<rs->Fields->GetCount(); i++ ) {
				_variant_t val = rs->GetCollect( (long)i );
				CVariant   temp;
				switch (val.vt) {
					case VT_NULL : temp.SetNull(); break;
					case VT_I4   :
					case VT_I2   : temp = (long)val; break;
					case VT_R4   : temp = (double)val; break;
					case VT_R8   : temp = (double)val; break;
					case VT_BOOL : temp = (bool)val; break;

					case VT_BSTR : {
						string s = (string)((_bstr_t)val);
						int pos = s.find( '\\' );
						while ( pos >= 0 ) {
							s = s.substr( 0, pos ) + "\r\n" + s.substr( pos+2, s.length()-(pos+2) );
							pos = s.find( '\\' );
						}
						temp = s;
					} break;

					case VT_DATE : {
						COleDateTime dt = val;
						temp = CDate( dt.GetMonth(), dt.GetDay(), dt.GetYear() );
					} break;

					default		 : temp.SetNull(); break;
				}
				row.push_back(temp);
			}
			data.push_back(row);
			rs->MoveNext();
		}
		rs->Close();
		ans.SetFieldNames( fields );
		ans.SetData( data );
		ans.MoveTo( 0 );
	}
	rs.Release();
	return ans;
}

void CConnection::BeginTransaction() {
	if ( open ) {
		conn->BeginTrans();
	}
}

void CConnection::CommitTransaction() {
	if ( open ) {
		conn->CommitTrans();
	}
}

void CConnection::CancelTransaction() {
	if ( open ) {
		conn->Cancel();
	}
}
