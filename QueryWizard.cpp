
#include "QueryWizard.h"
#include "SelectDialog.h"
#include "WhereDialog.h"
#include "OrderDialog.h"
#include "Connection.h"
#include "resource.h"

CQueryWizard app;

void CQueryWizard::ExecuteQuery( CQueryBuilder& builder ) {
	//MessageBox( 0, builder.GetQuery().c_str(), "query", 0 );

	CConnection conn;
	if ( !conn.OpenForReading() ) {
		MessageBox( 0, "Unable to connect to the database!", "", 0 );
		return;
	}

	try {
		BeginWaitCursor();
		CTable rs = conn.Execute( builder.GetQuery() );

		char tempPath[4001];
		char tempFile[4001];
		::GetTempPath( 4000, tempPath );

		int fileNum = 0;
		FILE* f = NULL;
		while ( !f ) {
			sprintf( tempFile, "%s\\studentinfo%d.xls", tempPath, fileNum++ );
			f = fopen( tempFile, "w" );
			if ( !f && fileNum == 0 ) {
				MessageBox( 0, "Unable to create file to store query!", "StudentInfo QueryWizard", 0 );
				return;
			}
		}

		vector<string> header = builder.GetSelect();
		for ( int j=0; j<header.size(); j++ ) {
			if ( j ) {
				fprintf( f, "\t%s", header[j].c_str() );
			} else {
				fprintf( f, "%s", header[j].c_str() );
			}
		}
		fprintf( f, "\n" );
		while ( !rs.IsAtEOF() ) {
			for ( int i=0; i<rs.GetColumnCount(); i++ ) {
				if ( i ) {
					fprintf( f, "\t%s", ((string)rs[i]).c_str() );
				} else {
					fprintf( f, "%s", ((string)rs[i]).c_str() );
				}
			}
			fprintf( f, "\n" );
			rs.MoveNext();
		}

		fclose(f);

		ShellExecute( NULL, "open", tempFile, NULL, NULL, SW_SHOWNORMAL );
		EndWaitCursor();
	}
	catch ( ... ) {
		//char temp[10000];
		//if ( e.GetErrorMessage( temp, 10000 ) ) {
			MessageBox( 0, "Unable to execute the query!", "", 0 );
		//} else {
		//	MessageBox( 0, ( (string)"Unable to execute the query: " + (string)temp ).c_str(), "", 0 );
		//}
	}
}

BOOL CQueryWizard::InitInstance() {
	CQueryBuilder builder;

	CSelectDialog selectDialog;
	CWhereDialog whereAllDialog( true );
	CWhereDialog whereAnyDialog( false );
	COrderDialog orderDialog;

	CQueryWizardDialog* dialogs[4];
	dialogs[0] = &selectDialog;
	dialogs[1] = &whereAllDialog;
	dialogs[2] = &whereAnyDialog;
	dialogs[3] = &orderDialog;

	int current_dialog = 0;
	while ( true ) {
		switch ( dialogs[ current_dialog ]->Do( &builder ) ) {
			case QW_NEXT: current_dialog++; break;
			case QW_BACK: current_dialog--; break;
			case QW_EXIT: return FALSE; break;
			//case QW_FINISH: ExecuteQuery( builder ); break;
		}
		if ( current_dialog < 0 ) {
			current_dialog = 0;
		}
		if ( current_dialog > 3 ) {
			current_dialog = 3;
		}
	}

	return FALSE;
}
