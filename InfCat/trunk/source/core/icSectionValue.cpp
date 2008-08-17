//! @file icSectionValue.cpp

#include "icCore.h"

namespace ic {

int32_t
icSectionValue::Count( const std::string& strSection, const std::string& strKey )
{
	SECTION_IT p = m_Sections.find( strSection );
	if(p == m_Sections.end()) {
		return 0;
	}
	KEY& key = p->second;
	KEY_IT pp = key.find( strKey );
	if(pp == key.end()) {
		return 0;
	}
	return pp->second.size();
}

std::string
icSectionValue::GetValue( const std::string& strSection, const std::string& strKey, int32_t nIndex )
{
	return m_Sections[strSection][strKey][nIndex];
}

void
icSectionValue::Clear( void )
{
	m_Sections.clear();
}

void
icSectionValue::Clear( const std::string& strSection )
{
	m_Sections.erase( strSection );
}

void
icSectionValue::Clear( const std::string& strSection, const std::string& strKey )
{
	SECTION_IT p = m_Sections.find( strSection );
	if(p == m_Sections.end()) {
		return;
	}
	KEY& key = p->second;
	key.erase( strKey );
}

void
icSectionValue::Register( const std::string& strSection, const std::string& strKey, const std::string& strValue )
{
	m_Sections[strSection][strKey].push_back( strValue );
}

} // namespace ic
