
#ifndef TABLE_H
#define TABLE_H

#include "Variant.h"

#pragma warning(disable:4786)
#include <vector>
#include <string>
using namespace std;

class CTable {
	public:
		CTable();
		CTable( const CTable& table );

		CTable operator=( const CTable& table );

		void SetFieldNames( vector<string> names );
		void SetData( vector< vector< CVariant > > newdata );

		vector<string> GetFieldNames();
		int GetColumnCount();
		int GetRowCount();

		void MoveTo( int index );
		void MoveNext();
		bool IsAtEOF();
		bool IsEmpty();

		CVariant operator[]( char* index );
		CVariant operator[]( int index );
	private:
		vector< vector< CVariant > > data;
		vector<string> columns;
		int record;
};

#endif
