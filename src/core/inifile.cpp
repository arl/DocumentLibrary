/* Copyright (C) 
* 2010 - Aurelien Rainone
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
* 
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
* 
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
* 
*/

// inifile.cpp:  Implementation of the inifile class.
// Written by:   Adam Clauss
// Email: cabadam@houston.rr.com
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.HGetValueill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform.
//////////////////////////////////////////////////////////////////////

// C Includes
#include <cstdio>
#include <cstdarg>
#include <cctype>

// Local Includes
#include "inifile.hpp"

#include <boost/lexical_cast.hpp>
using boost::lexical_cast;

#if defined(WIN32)
# define iniEOL endl
#else
# define iniEOL '\r' << endl
#endif

namespace doclib
{

	namespace core
	{

		using namespace std;

		inifile::inifile(const string iniPath)
		{
			path(iniPath);
			mbCaseInsensitive = true;
		}

		bool inifile::readFile()
		{
			ifstream f;
			f.open(mPath.c_str(), ios::in);
			if (f.fail())
				return false;

			bool bRet = readStream(f);	
			f.close();

			return bRet;
		}

		// read ini file from a string
		bool inifile::readIniFromString(const string & iniStr)
		{
			istringstream strstr;
			strstr.str(iniStr);

			return readStream(strstr);
		}

		bool inifile::readStream(istream & str)
		{
			// Normally you would use ifstream, but the SGI CC compiler has
			// a few bugs with ifstream. So ... fstream used.
			string   line;
			string   keyname, valuename, value;
			string::size_type pLeft, pRight;

			while( getline( str, line)) {
				// To be compatible with Win32, check for existence of '\r'.
				// Win32 files have the '\r' and Unix files don't at the end of a line.
				// Note that the '\r' will be written to INI files from
				// Unix so that the created INI file can be read under Win32
				// without change.
				if (line.empty()) continue;
				if ( line[line.length() - 1] == '\r')
					line = line.substr( 0, line.length() - 1);

				if ( line.length()) {
					// Check that the user hasn't openned a binary file by checking the first
					// character of each line!
					if ( !isprint( line[0])) {
						printf( "Failing on char %d\n", line[0]);
						return false;
					}
					if (( pLeft = line.find_first_of(";#[=")) != string::npos) {
						switch ( line[pLeft]) {
							case '[':
								if ((pRight = line.find_last_of("]")) != string::npos &&
										pRight > pLeft) {
									keyname = line.substr( pLeft + 1, pRight - pLeft - 1);
									AddKeyName( keyname);
								}
								break;

							case '=':
								valuename = line.substr( 0, pLeft);
								value = line.substr( pLeft + 1);
								SetValue( keyname, valuename, value);
								break;

							case ';':
							case '#':
								if ( !names.size())
									HeaderComment( line.substr( pLeft + 1));
								else
									keyComment( keyname, line.substr( pLeft + 1));
								break;
						}
					}
				}
			}

			if ( names.size())
				return true;
			return false;
		}


		bool inifile::writeFile()
		{
			unsigned commentID, keyID, valueID;
			// Normally you would use ofstream, but the SGI CC compiler has
			// a few bugs with ofstream. So ... fstream used.
			fstream f;

			f.open(mPath.c_str(), ios::out);
			if ( f.fail())
				return false;

			// Write header comments.
			for ( commentID = 0; commentID < comments.size(); ++commentID)
				f << ';' << comments[commentID] << iniEOL;
			if ( comments.size())
				f << iniEOL;

			// Write keys and values.
			for ( keyID = 0; keyID < keys.size(); ++keyID) {
				f << '[' << names[keyID] << ']' << iniEOL;
				// Comments.
				for ( commentID = 0; commentID < keys[keyID].comments.size(); ++commentID)
					f << ';' << keys[keyID].comments[commentID] << iniEOL;
				// Values.
				for ( valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
					f << keys[keyID].names[valueID] << '=' << keys[keyID].values[valueID] << iniEOL;
				f << iniEOL;
			}
			f.close();

			return true;
		}

		long inifile::FindKey( const string keyname) const
		{
			for ( unsigned keyID = 0; keyID < names.size(); ++keyID)
				if ( checkCase( names[keyID]) == checkCase( keyname))
					return long(keyID);
			return noID;
		}

		long inifile::FindValue( const unsigned int keyID, const string valuename) const
		{
			if ( !keys.size() || keyID >= keys.size())
				return noID;

			for ( unsigned valueID = 0; valueID < keys[keyID].names.size(); ++valueID)
				if ( checkCase( keys[keyID].names[valueID]) == checkCase( valuename))
					return long(valueID);
			return noID;
		}

		unsigned inifile::AddKeyName( const string keyname)
		{
			names.resize( names.size() + 1, keyname);
			keys.resize( keys.size() + 1);
			return names.size() - 1;
		}

		string inifile::KeyName( const unsigned int keyID) const
		{
			if ( keyID < names.size())
				return names[keyID];
			else
				return "";
		}

		unsigned inifile::NumValues( const unsigned int keyID)
		{
			if ( keyID < keys.size())
				return keys[keyID].names.size();
			return 0;
		}

		unsigned inifile::NumValues( const string keyname)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return 0;
			return keys[keyID].names.size();
		}

		string inifile::ValueName( const unsigned int keyID, const unsigned int valueID) const
		{
			if ( keyID < keys.size() && valueID < keys[keyID].names.size())
				return keys[keyID].names[valueID];
			return "";
		}

		string inifile::ValueName( const string keyname, const unsigned int valueID) const
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return "";
			return ValueName( keyID, valueID);
		}

		bool inifile::SetValue( const unsigned int keyID, const unsigned int valueID, const string value)
		{
			if ( keyID < keys.size() && valueID < keys[keyID].names.size())
				keys[keyID].values[valueID] = value;

			return false;
		}

		bool inifile::SetValue( const string keyname, const string valuename, const string value, bool const create)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID) {
				if ( create)
					keyID = long( AddKeyName( keyname));
				else
					return false;
			}

			long valueID = FindValue( unsigned(keyID), valuename);
			if ( valueID == noID) {
				if ( !create)
					return false;
				keys[keyID].names.resize( keys[keyID].names.size() + 1, valuename);
				keys[keyID].values.resize( keys[keyID].values.size() + 1, value);
			} else
				keys[keyID].values[valueID] = value;

			return true;
		}

		bool inifile::SetValueI( const string keyname, const string valuename, int const value, bool const create)
		{
			return SetValue( keyname, valuename,
					lexical_cast<string>(value));
		}

		bool inifile::SetValueF( const string keyname, const string valuename, double const value, bool const create)
		{
			return SetValue( keyname, valuename,
					lexical_cast<string>(value));
		}

		bool inifile::SetValueV( const string keyname, const string valuename, char *format, ...)
		{
			va_list args;
			char value[MAX_VALUEDATA];
			va_start( args, format);
			vsnprintf( value, MAX_VALUEDATA, format, args);
			va_end( args);
			return SetValue( keyname, valuename, value);
		}

		string inifile::GetValue( const unsigned int keyID, const unsigned int valueID, const string defValue) const
		{
			if ( keyID < keys.size() && valueID < keys[keyID].names.size())
				return keys[keyID].values[valueID];
			return defValue;
		}

		string inifile::GetValue( const string keyname, const string valuename, const string defValue) const
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
			{
				//cout<<"clé "<<keyname<<" pas trouvée"<<endl;
				return defValue;
			}
			long valueID = FindValue( unsigned(keyID), valuename);
			if ( valueID == noID)
			{
				//cout<<"valeur "<<valuename<<" pas trouvée"<<endl;
				return defValue;
			}
			return keys[keyID].values[valueID];
		}

		std::string inifile::GetValue(const unsigned int keyID, const std::string valuename, const std::string defValue /*= ""*/ ) const
		{
			if (keyID < keys.size())
			{
				const unsigned int valueID = FindValue(keyID, valuename);
				return GetValue(keyID, valueID, defValue);
			}
			return defValue;
		}

		int inifile::GetValueI(const string keyname, const string valuename, int const defValue) const
		{
			std::string defStr = lexical_cast<string>(defValue);
			return lexical_cast<int>(GetValue( keyname, valuename, defStr));
		}

		int inifile::GetValueI( const unsigned int keyID, const std::string valuename, int const defValue /*= 0*/ ) const
		{
			int ret = defValue;
			if (keyID < keys.size())
			{
				const unsigned int valueID = FindValue(keyID, valuename);
				if (valueID != noID)
				{
					// Use istringstream for direct correspondence with toString
					std::istringstream str(keys[keyID].values[valueID]);
					str >> ret;
				}
			}
			return ret;
		}

		double inifile::GetValueF(const string keyname, const string valuename, double const defValue) const
		{
			std::string defStr = lexical_cast<string>(defValue);
			return lexical_cast<double>(GetValue( keyname, valuename, defStr));
		}

		double inifile::GetValueF(const unsigned int keyID, const std::string valuename, double const defValue /*= 0.0*/) const
		{
			double ret = defValue;
			if (keyID < keys.size())
			{
				const unsigned int valueID = FindValue(keyID, valuename);
				if (valueID != noID)
				{
					// Use istringstream for direct correspondence with toString
					std::istringstream str(keys[keyID].values[valueID]);
					str >> ret;
				}
			}
			return ret;
		}


		// 16 variables may be a bit of over kill, but hey, it's only code.
		unsigned inifile::GetValueV( const string keyname, const string valuename, char *format,
				void *v1, void *v2, void *v3, void *v4,
				void *v5, void *v6, void *v7, void *v8,
				void *v9, void *v10, void *v11, void *v12,
				void *v13, void *v14, void *v15, void *v16)
		{
			string   value;
			// va_list  args;
			unsigned nVals;


			value = GetValue( keyname, valuename);
			if ( !value.length())
				return false;
			// Why is there not vsscanf() function. Linux man pages say that there is
			// but no compiler I've seen has it defined. Bummer!
			//
			// va_start( args, format);
			// nVals = vsscanf( value.c_str(), format, args);
			// va_end( args);

			nVals = sscanf( value.c_str(), format,
					v1, v2, v3, v4, v5, v6, v7, v8,
					v9, v10, v11, v12, v13, v14, v15, v16);

			return nVals;
		}

		bool inifile::DeleteValue( const string keyname, const string valuename)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return false;

			long valueID = FindValue( unsigned(keyID), valuename);
			if ( valueID == noID)
				return false;

			// This looks strange, but is neccessary.
			vector<string>::iterator npos = keys[keyID].names.begin() + valueID;
			vector<string>::iterator vpos = keys[keyID].values.begin() + valueID;
			keys[keyID].names.erase( npos, npos + 1);
			keys[keyID].values.erase( vpos, vpos + 1);

			return true;
		}

		bool inifile::deleteKey( const string keyname)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return false;

			// Now hopefully this destroys the vector lists within keys.
			// Looking at <vector> source, this should be the case using the destructor.
			// If not, I may have to do it explicitly. Memory leak check should tell.
			// memleak_test.cpp shows that the following not required.
			//keys[keyID].names.clear();
			//keys[keyID].values.clear();

			vector<string>::iterator npos = names.begin() + keyID;
			vector<key>::iterator    kpos = keys.begin() + keyID;
			names.erase( npos, npos + 1);
			keys.erase( kpos, kpos + 1);

			return true;
		}

		void inifile::Clear()
		{
			// This loop not needed. The vector<> destructor seems to do
			// all the work itself. memleak_test.cpp shows this.
			//for ( unsigned i = 0; i < keys.size(); ++i) {
			//  keys[i].names.clear();
			//  keys[i].values.clear();
			//}
			names.clear();
			keys.clear();
			comments.clear();
		}

		void inifile::HeaderComment( const string comment)
		{
			comments.resize( comments.size() + 1, comment);
		}

		string inifile::HeaderComment( const unsigned int commentID) const
		{
			if ( commentID < comments.size())
				return comments[commentID];
			return "";
		}

		bool inifile::DeleteHeaderComment( unsigned commentID)
		{
			if ( commentID < comments.size()) {
				vector<string>::iterator cpos = comments.begin() + commentID;
				comments.erase( cpos, cpos + 1);
				return true;
			}
			return false;
		}

		unsigned inifile::numKeyComments( const unsigned int keyID) const
		{
			if ( keyID < keys.size())
				return keys[keyID].comments.size();
			return 0;
		}

		unsigned inifile::numKeyComments( const string keyname) const
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return 0;
			return keys[keyID].comments.size();
		}

		bool inifile::keyComment( const unsigned int keyID, const string comment)
		{
			if ( keyID < keys.size()) {
				keys[keyID].comments.resize( keys[keyID].comments.size() + 1, comment);
				return true;
			}
			return false;
		}

		bool inifile::keyComment( const string keyname, const string comment)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return false;
			return keyComment( unsigned(keyID), comment);
		}

		string inifile::keyComment( const unsigned int keyID, const unsigned int commentID) const
		{
			if ( keyID < keys.size() && commentID < keys[keyID].comments.size())
				return keys[keyID].comments[commentID];
			return "";
		}

		string inifile::keyComment( const string keyname, const unsigned int commentID) const
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return "";
			return keyComment( unsigned(keyID), commentID);
		}

		bool inifile::deleteKeyComment( const unsigned int keyID, const unsigned int commentID)
		{
			if ( keyID < keys.size() && commentID < keys[keyID].comments.size()) {
				vector<string>::iterator cpos = keys[keyID].comments.begin() + commentID;
				keys[keyID].comments.erase( cpos, cpos + 1);
				return true;
			}
			return false;
		}

		bool inifile::deleteKeyComment( const string keyname, const unsigned int commentID)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return false;
			return deleteKeyComment( unsigned(keyID), commentID);
		}

		bool inifile::deleteKeyComments( const unsigned int keyID)
		{
			if ( keyID < keys.size()) {
				keys[keyID].comments.clear();
				return true;
			}
			return false;
		}

		bool inifile::deleteKeyComments( const string keyname)
		{
			long keyID = FindKey( keyname);
			if ( keyID == noID)
				return false;
			return deleteKeyComments( unsigned(keyID));
		}

		string inifile::checkCase( string s) const
		{
			if (mbCaseInsensitive)
				for ( string::size_type i = 0; i < s.length(); ++i)
					s[i] = tolower(s[i]);
			return s;
		}

		void inifile::GetValues(const std::string keyname, NameValuePairList & values)
		{
			GetValues(FindKey(keyname), values);
		}

		void inifile::GetValues(const unsigned int keyID, NameValuePairList & values)
		{
			if (keyID < keys.size())
			{
				const key & key_ = keys[keyID];
				const unsigned int numNames = key_.names.size();
				const unsigned int numValues = key_.values.size();

				std::string name, value;
				for (unsigned int idx = 0; idx < numValues; ++idx)
				{
					name = (idx < numNames)? key_.names[idx] : "";
					value = (idx < numValues)? key_.values[idx] : "";

					// insert only pairs with non empty names
					if (!name.empty())
						values.insert(std::make_pair(name, value));
				}
			}
		}

	}
}
