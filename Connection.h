
/*

  Remember to move the MakeString functions here.

*/

#ifndef CONNECTION_H
#define CONNECTION_H

#include "Table.h"

#pragma warning(disable:4146)
#pragma warning(disable:4786)
#include <comdef.h>
#import "C:\program files\common files\system\ado\msado15.dll" no_namespace rename( "EOF", "adoEOF" )

#include <vector>
using namespace std;

class CConnection {
	public:
		CConnection();
		CConnection(string dsn, string uid, string pass);
		~CConnection();

		bool Open(string dsn, string uid, string pass);

		string CConnection::GetDSN();
		bool CConnection::OpenForReading();
		bool CConnection::OpenForWriting();
		bool CConnection::OpenForAuthReading();
		bool CConnection::OpenForAuthWriting();

		void Close();
		CTable Execute(string sql);

		void BeginTransaction();
		void CommitTransaction();
		void CancelTransaction();
	private:
		bool open;
		_ConnectionPtr conn;
};

#endif