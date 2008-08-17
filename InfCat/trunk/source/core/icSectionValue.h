#ifndef INCL_CLASS_icSectionValue
#define INCL_CLASS_icSectionValue

#include "icCore.h"

namespace ic {

class icSectionValue {
protected:
	typedef std::vector<std::string> VALUES;
	typedef std::map<std::string,VALUES> KEY;
	typedef std::map<std::string,VALUES>::iterator KEY_IT;
	typedef std::map<std::string,KEY> SECTION;
	typedef std::map<std::string,KEY>::iterator SECTION_IT;
public:
	int32_t Count( const std::string& strSection, const std::string& strKey );
	std::string GetValue( const std::string& strSection, const std::string& strKey, int32_t nIndex );
	void Clear( void );
	void Clear( const std::string& strSection );
	void Clear( const std::string& strSection, const std::string& strKey );
	void Register( const std::string& strSection, const std::string& strKey, const std::string& strValue );
protected:
	SECTION	m_Sections;
};

} // namespace ic

#endif // INCL_CLASS_icSectionValue
