
#include "XMLObject.h"

bool CXMLObject::LoadFromFile( string filename ) {
	bool inTag=false, inString=false, inParameter=false, inValue=false, inTagName=false, inComment=false;
	XMLTag tag;
	vector< XMLTag > tags;
	string parameter;

	FILE* f = fopen( filename.c_str(), "r" );
	if ( !f ) {
		SetError( "unable to open file" );
		return false;
	}

	int curr;
	curr = fgetc( f );
	while ( curr != EOF ) {
		char current = (char)curr;
		if ( inTag ) {
			if ( inTagName ) {
				if ( current == '/' ) {
					inTagName = false;
					tag.empty = true;
				} else {
					if ( current == ' ' ) {
						inTagName = false;
					} else {
						if ( current == '>' ) {
							inTagName = false;
							inTag = false;
							tags.push_back( tag );
						} else {
							tag.name += current;
						}
					}
				}
			} else {
				if ( inParameter ) {
					if ( inValue ) {
						if ( inString ) {
							if ( current == '\"' ) {
								inString = false;
								inValue = false;
								inParameter=false;
							} else {
								tag.parameters[ tag.parameters.size()-1 ].second += current;
							}
						} else {
							if ( current == '/' ) {
								inParameter = false;
								inValue = false;
								tag.empty = true;
							} else {
								if ( current == '\"' ) {
									if ( tag.parameters[ tag.parameters.size()-1 ].second == "" ) {
										inString = true;
									} else {
										SetError( "stray quote" );
										return false;
									}
								} else {
									if ( current == ' ' ) {
										inParameter = false;
										inValue = false;
									} else {
										if ( current == '>' ) {
											inParameter = false;
											inValue = false;
											inTag = false;
										} else {
											tag.parameters[ tag.parameters.size()-1 ].second += current;
										}
									}
								}
							}
						}
					} else {
						if ( current == '/' ) {
							tag.empty = true;
							inParameter = false;
							tag.options.push_back( parameter );
						} else {
							if ( current == '>' ) {
								tag.options.push_back( parameter );
								parameter = "";
								inTag = false;
								inParameter = false;
							} else {
								if ( current == '=' ) {
									tag.parameters.push_back( pair<string,string>( parameter, "" ) );
									inValue = true;
								} else {
									parameter+=current;
								}
							}
						}
					}
				} else {
					if ( current == '/' ) {
						if ( tag.name == "" ) {
							tag.end = true;
						} else {
							tag.empty = true;
						}
					} else {
						if ( current == '>' ) {
							inTag = false;
							tags.push_back( tag );
						} else {
							if ( current != ' ' ) {
								if ( tag.name == "" ) {
									inTagName = true;
									tag.name += current;
								} else {
									inParameter = true;
									parameter = "";
									parameter += current;
								}
							}
						}
					}
				}
			}
		} else {
			if ( current == '<' ) {
				inTag = true;
				tag.name = "";
				tag.parameters = vector< pair< string, string > >();
				tag.options = vector< string >();
				tag.empty = false;
				tag.end = false;
			} else {
				if ( current != ' ' && current != '\n' ) {
					SetError( "Stray Character" );
					return false;
				}
			}
		}
		curr = fgetc( f );
	}
	if ( inTag ) {
		SetError( "unexpected end of file" );
		return false;
	}
	fclose( f );
	int index = 0;
	if ( !CreateObject( tags, index ) ) {
		return false;
	}
	return true;
}

string CXMLObject::GetType() {
	return type;
}

string CXMLObject::GetParameter( string name ) {
	if ( parameters.find( name ) == parameters.end() ) {
		return "";
	} else {
		return parameters[ name ];
	}
}

bool CXMLObject::GetOption( string name ) {
	return ( options.find( name ) != options.end() );
}

int CXMLObject::GetChildCount() {
	return children.size();
}

CXMLObject* CXMLObject::GetChild( int i ) {
	if ( i>=0 && i<children.size() ) {
		return &(children[i]);
	} else {
		return NULL;
	}
}

bool CXMLObject::CreateObject( vector< XMLTag >& tags, int& index ) {
	type = tags[ index ].name;
	for ( int i=0; i<tags[ index ].parameters.size(); i++ ) {
		parameters[ tags[ index ].parameters[ i ].first ] = tags[ index ].parameters[ i ].second;
	}
	for ( int j=0; j<tags[ index ].options.size(); j++ ) {
		options.insert( tags[ index ].options[ j ] );
	}
	if ( tags[ index ].empty ) {
		index++;
		return true;
	}
	index++;
	while ( index<tags.size() ) {
		if ( tags[ index ].name == type && tags[ index ].end ) {
			index++;
			return true;
		}
		CXMLObject child;
		children.push_back( child );
		if ( !children[ children.size()-1 ].CreateObject( tags, index ) ) {
			return false;
		}
	}
	SetError( ( (string)"Unexpected end of XML file at " + type + " tag." ).c_str() );
	return false;
}

void CXMLObject::SetError( string newError ) {
	error = newError;
}
