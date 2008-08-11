//! @file	icSffLoader.h

#ifndef INCL_CLASS_icSff
#define INCL_CLASS_icSff

#include "icCore.h"

namespace ic {

class icTexture;

class icSff {
	uint32_t	m_nCountImage;
	std::vector<icTexture*> m_pTexture;
public:
	enum enumFlag {
		NORMAL,
	};

	bool Create( Cat_Stream* pStream, enumFlag eFlag = NORMAL );
	uint32_t	GetTextureCount( void ) const;
	icTexture* SearchFromIndex( uint32_t nIndex );
};

} // namespace ic

#endif // INCL_CLASS_icSff
