
#include "Variant.h"

CVariant::CVariant() {
	type = "null";
	intval = -1;
	boolval = false;
	stringval = "";
}

CVariant::CVariant( const CVariant& val ) {
	type = val.type;
	intval = val.intval;
	doubleval = val.doubleval;
	boolval = val.boolval;
	stringval = val.stringval;
	dateval = val.dateval;
}

CVariant::CVariant( int val ) {
	type = "int";
	intval = val;
}

CVariant::CVariant( double val ) {
	type = "double";
	doubleval = val;
}

CVariant::CVariant( string val ) {
	type = "string";
	stringval = val;
}

CVariant::CVariant( bool val ) {
	type = "bool";
	boolval = val;
}

CVariant::CVariant( const CDate& val ) {
	type = "date";
	dateval = val;
}

CVariant CVariant::operator=( const CVariant& val ) {
	type = val.type;
	intval = val.intval;
	doubleval = val.doubleval;
	boolval = val.boolval;
	stringval = val.stringval;
	dateval = val.dateval;
	return *this;
}

bool CVariant::operator==( const CVariant& val ) {
	if ( type != val.type ) {
		return false;
	}
	if ( type == "null" ) {
		return true;
	}
	if ( type == "string" ) {
		return stringval == val.stringval;
	}
	if ( type == "int" ) {
		return intval == val.intval;
	}
	if ( type == "double" ) {
		return doubleval == val.doubleval;
	}
	if ( type == "bool" ) {
		return boolval == val.boolval;
	}
	if ( type == "date" ) {
		return dateval == val.dateval;
	}
	return true;
}

bool CVariant::operator!=( const CVariant& val ) {
	return !( (*this)==val );
}

string CVariant::GetType() {
	return type;
}

void CVariant::SetNull() {
	type = "null";
}

CVariant::operator int () {
	if ( type == "int" ) {
		return intval;
	} else {
		if ( type == "double" ) {
			return (int)doubleval;
		} else {
			return -1;
		}
	}
}

CVariant::operator string () {
	if ( type == "string" ) {
		return stringval;
	}
	if ( type == "int" ) {
		if ( (int)(*this) < 0 ) {
			return "";
		}
		char temp[100];
		sprintf( temp, "%d", (int)(*this) );
		return (string)temp;
	}
	if ( type == "double" ) {
		char temp[100];
		sprintf( temp, "%f", (double)(*this) );
		return (string)temp;
	}
	if ( type == "bool" ) {
		if ( (bool)(*this) ) {
			return "true";
		} else {
			return "false";
		}
	}
	if ( type == "null" ) {
		return "";
	}
	if ( type == "date" ) {
		return dateval;
	}
	return "";
}

CVariant::operator double () {
	return doubleval;
}

CVariant::operator bool () {
	if ( type == "bool" ) {
		return boolval;
	} else {
		if ( type == "string" ) {
			if ( stringval == "T" || stringval == "t" ) {
				return true;
			} else {
				return false;
			}
		} else {
			if ( type == "int" ) {
				if ( intval ) {
					return true;
				} else {
					return false;
				}
			} else {
				return false;
			}
		}
	}
}

CVariant::operator CDate() {
	if ( type == "date" ) {
		return CDate(dateval);
	} else {
		return CDate();
	}
}
