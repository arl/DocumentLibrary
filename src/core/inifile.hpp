// inifile.cpp:  Implementation of the inifile class.
// Written by:   Adam Clauss
// Email: cabadam@tamu.edu
// You may use this class/code as you wish in your programs.  Feel free to distribute it, and
// email suggested changes to me.
//
// Rewritten by: Shane Hill
// Date:         21/08/2001
// Email:        Shane.Hill@dsto.defence.gov.au
// Reason:       Remove dependancy on MFC. Code should compile on any
//               platform. Tested on Windows/Linux/Irix
//////////////////////////////////////////////////////////////////////

#ifndef DOCLIB_CORE_INIFILE_HPP
#define DOCLIB_CORE_INIFILE_HPP

#include "core_includes.hpp"

namespace doclib
{

	namespace core
	{

		class inifile
		{
			static const int MAX_KEYNAME	= 128;
			static const int MAX_VALUENAME	= 128;
			static const int MAX_VALUEDATA	= 128;

			private:

				bool		mbCaseInsensitive;
				std::string mPath;
				struct key
				{
					std::vector<std::string> names;
					std::vector<std::string> values; 
					std::vector<std::string> comments;
				};
				std::vector<key>			keys; 
				std::vector<std::string>	names; 
				std::vector<std::string>	comments;
				std::string checkCase( std::string s) const;

				// added to decouple reading from a stream (fie or stringstream) 
				bool readStream(std::istream & str);

			public:

				enum errors{ noID = -1};
				inifile( const std::string iniPath = "");
				virtual ~inifile()                            {}

				// Sets whether or not keynames and valuenames should be case sensitive.
				// The default is case insensitive.
				void caseSensitive()                           {mbCaseInsensitive = false;}
				void caseInsensitive()                         {mbCaseInsensitive = true;}

				// Sets path of ini file to read and write from.
				void path(const std::string newPath)                {mPath = newPath;}
				std::string path() const                            {return mPath;}

				// Reads ini file specified using path.
				// Returns true if successful, false otherwise.
				bool readFile();

				// read ini file from a string
				bool readIniFromString(const std::string & iniStr);

				// Writes data stored in class to ini file.
				bool writeFile(); 

				// Deletes all stored ini data.
				void Clear();

				// Returns index of specified key, or noID if not found.
				long FindKey(const std::string keyname) const;

				// Returns index of specified value, in the specified key, or noID if not found.
				long FindValue(const unsigned int keyID, const std::string valuename) const;

				// Returns number of keys currently in the ini.
				unsigned NumKeys() const                       {return names.size();}
				unsigned GetNumKeys() const                    {return NumKeys();}

				// Add a key name.
				unsigned AddKeyName(const std::string keyname);

				// Returns key names by index.
				std::string KeyName(const unsigned int keyID) const;
				std::string GetKeyName(const unsigned int keyID) const {return KeyName(keyID);}

				// Returns number of values stored for specified key.
				unsigned NumValues(const unsigned int keyID);
				unsigned GetNumValues(const unsigned int keyID)   {return NumValues( keyID);}
				unsigned NumValues(const std::string keyname);
				unsigned GetNumValues(const std::string keyname)   {return NumValues( keyname);}

				// Returns value name by index for a given keyname or keyID.
				std::string ValueName(const unsigned int keyID, const unsigned int valueID) const;
				std::string GetValueName(const unsigned int keyID, const unsigned int valueID) const {
					return ValueName(keyID, valueID);
				}
				std::string ValueName(const std::string keyname, const unsigned int valueID) const;
				std::string GetValueName(const std::string keyname, const unsigned int valueID) const {
					return ValueName(keyname, valueID);
				}

				// Gets value of [keyname] valuename =.
				// Overloaded to return string, int, and double.
				// Returns defValue if key/value not found.
				std::string GetValue(const unsigned int keyID, const unsigned int valueID, const std::string defValue = "") const;
				std::string GetValue(const std::string keyname, const std::string valuename, const std::string defValue = "") const; 
				int    GetValueI(const std::string keyname, const std::string valuename, int const defValue = 0) const;
				bool   GetValueB(const std::string keyname, const std::string valuename, bool const defValue = false) const 
				{
					return bool(GetValueI(keyname, valuename, int(defValue)) != 0);
				}
				double   GetValueF(const std::string keyname, const std::string valuename, double const defValue = 0.0) const;

				std::string GetValue(const unsigned int keyID, const std::string valuename, const std::string defValue = "") const; 
				int    GetValueI(const unsigned int keyID, const std::string valuename, int const defValue = 0) const;
				bool   GetValueB(const unsigned int keyID, const std::string valuename, bool const defValue = false) const
				{
					return bool(GetValueI(keyID, valuename, int(defValue)) != 0);
				}
				double   GetValueF(const unsigned int keyID, const std::string valuename, double const defValue = 0.0) const;

				// This is a variable length formatted GetValue routine. All these voids
				// are required because there is no vsscanf() like there is a vsprintf().
				// Only a maximum of 16 variable can be read.
				unsigned GetValueV(const std::string keyname, const std::string valuename, char *format,
						void *v1 = 0, void *v2 = 0, void *v3 = 0, void *v4 = 0,
						void *v5 = 0, void *v6 = 0, void *v7 = 0, void *v8 = 0,
						void *v9 = 0, void *v10 = 0, void *v11 = 0, void *v12 = 0,
						void *v13 = 0, void *v14 = 0, void *v15 = 0, void *v16 = 0);

				/** Retrieve all name-value pairs contained in a key, as a NameValuePairList */
				void GetValues(const std::string keyname, NameValuePairList & values);

				/** Retrieve all name-value pairs contained in a key, as a NameValuePairList */
				void GetValues(const unsigned int keyID, NameValuePairList & values);

				// Sets value of [keyname] valuename =.
				// Specify the optional paramter as false (0) if you do not want it to create
				// the key if it doesn't exist. Returns true if data entered, false otherwise.
				// Overloaded to accept string, int, and double.
				bool SetValue(const unsigned int keyID, const unsigned int valueID, const std::string value);
				bool SetValue(const std::string keyname, const std::string valuename, const std::string value, bool const create = true);
				bool SetValueI(const std::string keyname, const std::string valuename, int const value, bool const create = true);
				bool SetValueB(const std::string keyname, const std::string valuename, bool const value, bool const create = true) {
					return SetValueI(keyname, valuename, int(value), create);
				}
				bool SetValueF(const std::string keyname, const std::string valuename, double const value, bool const create = true);
				bool SetValueV(const std::string keyname, const std::string valuename, char *format, ...);

				// Deletes specified value.
				// Returns true if value existed and deleted, false otherwise.
				bool DeleteValue(const std::string keyname, const std::string valuename);

				// Deletes specified key and all values contained within.
				// Returns true if key existed and deleted, false otherwise.
				bool deleteKey(std::string keyname);

				// Header comment functions.
				// Header comments are those comments before the first key.
				//
				// Number of header comments.
				unsigned NumHeaderComments()                  {return comments.size();}
				// Add a header comment.
				void     HeaderComment(const std::string comment);
				// Return a header comment.
				std::string   HeaderComment(const unsigned int commentID) const;
				// Delete a header comment.
				bool     DeleteHeaderComment(unsigned commentID);
				// Delete all header comments.
				void     DeleteHeaderComments()               {comments.clear();}

				// Key comment functions.
				// Key comments are those comments within a key. Any comments
				// defined within value names will be added to this list. Therefore,
				// these comments will be moved to the top of the key definition when
				// the inifile::writeFile() is called.
				//
				// Number of key comments.
				unsigned numKeyComments(const unsigned int keyID) const;
				unsigned numKeyComments(const std::string keyname) const;

				// Add a key comment.
				bool     keyComment(const unsigned int keyID, const std::string comment);
				bool     keyComment(const std::string keyname, const std::string comment);

				// Return a key comment.
				std::string   keyComment(const unsigned int keyID, const unsigned int commentID) const;
				std::string   keyComment(const std::string keyname, const unsigned int commentID) const;

				// Delete a key comment.
				bool     deleteKeyComment(const unsigned int keyID, const unsigned int commentID);
				bool     deleteKeyComment(const std::string keyname, const unsigned int commentID);

				// Delete all comments for a key.
				bool     deleteKeyComments(const unsigned int keyID);
				bool     deleteKeyComments(const std::string keyname);
		};
	}
}

#endif // !DOCLIB_CORE_INIFILE_HPP
