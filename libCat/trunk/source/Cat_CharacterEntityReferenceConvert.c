//! @file	Cat_CharacterEntityReferenceConvert.c
// 文字実体参照(Character entity reference)の変換をする

#include <stdio.h>
#include <string.h>

//! 変換テーブル用構造体
struct ConvertTable {
	char* pszKey;		/*!< キー */
	char* pszText;		/*!< 文字 */
};

//! 変換テーブル
static struct ConvertTable tblConvert[] = {
// ASCII Entities with Entity Names
	{ "&quot;",		"\"" },				// quotation mark
	{ "&#34;",		"\"" },				// quotation mark
	{ "&#034;",		"\"" },				// quotation mark
	{ "&apos;",		"'" },				// apostrophe
	{ "&#39;",		"'" },				// apostrophe
	{ "&#039;",		"'" },				// apostrophe
	{ "&amp;",		"&" },				// ampersand
	{ "&#38;",		"&" },				// ampersand
	{ "&#038;",		"&" },				// ampersand
	{ "&lt;",		"<" },				// less-than
	{ "&#60;",		"<" },				// less-than
	{ "&#060;",		"<" },				// less-than
	{ "&gt;",		">" },				// greater-than
	{ "&#62;",		">" },				// greater-than
	{ "&#062;",		">" },				// greater-than
// ISO 8859-1 Symbol Entities
	{ "&nbsp;",		" " },				// non-breaking space
	{ "&#160;",		" " },				// non-breaking space
	{ "&iexcl;",	"\xc2\xa1" },		// inverted exclamation mark
	{ "&#161;",		"\xc2\xa1" },		// inverted exclamation mark
	{ "&cent;",		"\xc2\xa2" },		// cent
	{ "&#162;",		"\xc2\xa2" },		// cent
	{ "&pound;",	"\xc2\xa3" },		// pound
	{ "&#163;",		"\xc2\xa3" },		// pound
	{ "&curren;",	"\xc2\xa4" },		// currency
	{ "&#164;",		"\xc2\xa4" },		// currency
	{ "&yen;",		"\xc2\xa5" },		// yen
	{ "&#165;",		"\xc2\xa5" },		// yen
	{ "&brvbar;",	"\xc2\xa6" },		// broken vertical bar
	{ "&#166;",		"\xc2\xa6" },		// broken vertical bar
	{ "&sect;",		"\xc2\xa7" },		// section
	{ "&#167;",		"\xc2\xa7" },		// section
	{ "&uml;",		"\xc2\xa8" },		// spacing diaeresis
	{ "&#168;",		"\xc2\xa8" },		// spacing diaeresis
	{ "&copy;",		"\xc2\xa9" },		// copyright
	{ "&#169;",		"\xc2\xa9" },		// copyright
	{ "&ordf;",		"\xc2\xaa" },		// feminine ordinal indicator
	{ "&#170;",		"\xc2\xaa" },		// feminine ordinal indicator
	{ "&laquo;",	"\xc2\xab" },		// angle quotation mark (left)
	{ "&#171;",		"\xc2\xab" },		// angle quotation mark (left)
	{ "&not;",		"\xc2\xac" },		// negation
	{ "&#172;",		"\xc2\xac" },		// negation
	{ "&shy;",		"\xc2\xad" },		// soft hyphen
	{ "&#173;",		"\xc2\xad" },		// soft hyphen
	{ "&reg;",		"\xc2\xae" },		// registered trademark
	{ "&#174;",		"\xc2\xae" },		// registered trademark
	{ "&macr;",		"\xc2\xaf" },		// spacing macron
	{ "&#175;",		"\xc2\xaf" },		// spacing macron
	{ "&deg;",		"\xc2\xb0" },		// degree
	{ "&#176;",		"\xc2\xb0" },		// degree
	{ "&plusmn;",	"\xc2\xb1" },		// plus-or-minus
	{ "&#177;",		"\xc2\xb1" },		// plus-or-minus
	{ "&sup2;",		"\xc2\xb2" },		// superscript 2
	{ "&#178;",		"\xc2\xb2" },		// superscript 2
	{ "&sup3;",		"\xc2\xb3" },		// superscript 3
	{ "&#179;",		"\xc2\xb3" },		// superscript 3
	{ "&acute;",	"\xc2\xb4" },		// spacing acute
	{ "&#180;",		"\xc2\xb4" },		// spacing acute
	{ "&micro;",	"\xc2\xb5" },		// micro
	{ "&#181;",		"\xc2\xb5" },		// micro
	{ "&para;",		"\xc2\xb6" },		// paragraph
	{ "&#182;",		"\xc2\xb6" },		// paragraph
	{ "&middot;",	"\xc2\xb7" },		// middle dot
	{ "&#183;",		"\xc2\xb7" },		// middle dot
	{ "&cedil;",	"\xc2\xb8" },		// spacing cedilla
	{ "&#184;",		"\xc2\xb8" },		// spacing cedilla
	{ "&sup1;",		"\xc2\xb9" },		// superscript 1
	{ "&#185;",		"\xc2\xb9" },		// superscript 1
	{ "&ordm;",		"\xc2\xba" },		// masculine ordinal indicator
	{ "&#186;",		"\xc2\xba" },		// masculine ordinal indicator
	{ "&raquo;",	"\xc2\xbb" },		// angle quotation mark (right)
	{ "&#187;",		"\xc2\xbb" },		// angle quotation mark (right)
	{ "&frac14;",	"\xc2\xbc" },		// fraction 1/4
	{ "&#188;",		"\xc2\xbc" },		// fraction 1/4
	{ "&frac12;",	"\xc2\xbd" },		// fraction 1/2
	{ "&#189;",		"\xc2\xbd" },		// fraction 1/2
	{ "&frac34;",	"\xc2\xbe" },		// fraction 3/4
	{ "&#190;",		"\xc2\xbe" },		// fraction 3/4
	{ "&iquest;",	"\xc2\xbf" },		// inverted question mark
	{ "&#191;",		"\xc2\xbf" },		// inverted question mark
	{ "&times;",	"\xc3\x97" },		// multiplication
	{ "&#251;",		"\xc3\x97" },		// multiplication
	{ "&divide;",	"\xc3\xb7" },		// division
	{ "&#247;",		"\xc3\xb7" },		// division
//ISO 8859-1 Character Entities
	{ "&Agrave;",	"\xc3\x80" },		// capital a, grave accent
	{ "&#192;",		"\xc3\x80" },		// capital a, grave accent
	{ "&Aacute;",	"\xc3\x81" },		// capital a, grave accent
	{ "&#193;",		"\xc3\x81" },		// capital a, acute accent
	{ "&Acirc;",	"\xc3\x82" },		// capital a, circumflex accent
	{ "&#194;",		"\xc3\x82" },		// capital a, circumflex accent
	{ "&Atilde;",	"\xc3\x83" },		// capital a, tilde
	{ "&#195;",		"\xc3\x83" },		// capital a, tilde
	{ "&Auml;",		"\xc3\x84" },		// capital a, umlaut mark
	{ "&#196;",		"\xc3\x84" },		// capital a, umlaut mark
	{ "&Aring;",	"\xc3\x85" },		// capital a, ring
	{ "&#197;",		"\xc3\x85" },		// capital a, ring
	{ "&AElig;",	"\xc3\x86" },		// capital ae
	{ "&#198;",		"\xc3\x86" },		// capital ae
	{ "&Ccedil;",	"\xc3\x87" },		// capital c, cedilla
	{ "&#199;",		"\xc3\x87" },		// capital c, cedilla
	{ "&Egrave;",	"\xc3\x88" },		// capital e, grave accent
	{ "&#200;",		"\xc3\x88" },		// capital e, grave accent
	{ "&Eacute;",	"\xc3\x89" },		// capital e, acute accent
	{ "&#201;",		"\xc3\x89" },		// capital e, acute accent
	{ "&Ecirc;",	"\xc3\x8a" },		// capital e, circumflex accent
	{ "&#202;",		"\xc3\x8a" },		// capital e, circumflex accent
	{ "&Euml;",		"\xc3\x8b" },		// capital e, umlaut mark
	{ "&#203;",		"\xc3\x8b" },		// capital e, umlaut mark
	{ "&Igrave;",	"\xc3\x8c" },		// capital i, grave accent
	{ "&#204;",		"\xc3\x8c" },		// capital i, grave accent
	{ "&Iacute;",	"\xc3\x8d" },		// capital i, acute accent
	{ "&#205;",		"\xc3\x8d" },		// capital i, acute accent
	{ "&Icirc;",	"\xc3\x8e" },		// capital i, circumflex accent
	{ "&#206;",		"\xc3\x8e" },		// capital i, circumflex accent
	{ "&Iuml;",		"\xc3\x8f" },		// capital i, umlaut mark
	{ "&#207;",		"\xc3\x8f" },		// capital i, umlaut mark
	{ "&ETH;",		"\xc3\x90" },		// capital eth, Icelandic
	{ "&#208;",		"\xc3\x90" },		// capital eth, Icelandic
	{ "&Ntilde;",	"\xc3\x91" },		// capital n, tilde
	{ "&#209;",		"\xc3\x91" },		// capital n, tilde
	{ "&Ograve;",	"\xc3\x92" },		// capital o, grave accent
	{ "&#210;",		"\xc3\x92" },		// capital o, grave accent
	{ "&Oacute;",	"\xc3\x93" },		// capital o, acute accent
	{ "&#211;",		"\xc3\x93" },		// capital o, acute accent
	{ "&Ocirc;",	"\xc3\x94" },		// capital o, circumflex accent
	{ "&#212;",		"\xc3\x94" },		// capital o, circumflex accent
	{ "&Otilde;",	"\xc3\x95" },		// capital o, tilde
	{ "&#213;",		"\xc3\x95" },		// capital o, tilde
	{ "&Ouml;",		"\xc3\x96" },		// capital o, umlaut mark
	{ "&#214;",		"\xc3\x96" },		// capital o, umlaut mark
	{ "&Oslash;",	"\xc3\x98" },		// capital o, slash
	{ "&#216;",		"\xc3\x98" },		// capital o, slash
	{ "&Ugrave;",	"\xc3\x99" },		// capital u, grave accent
	{ "&#217;",		"\xc3\x99" },		// capital u, grave accent
	{ "&Uacute;",	"\xc3\x9a" },		// capital u, acute accent
	{ "&#218;",		"\xc3\x9a" },		// capital u, acute accent
	{ "&Ucirc;",	"\xc3\x9b" },		// capital u, circumflex accent
	{ "&#219;",		"\xc3\x9b" },		// capital u, circumflex accent
	{ "&Uuml;",		"\xc3\x9c" },		// capital u, umlaut mark
	{ "&#220;",		"\xc3\x9c" },		// capital u, umlaut mark
	{ "&Yacute;",	"\xc3\x9d" },		// capital y, acute accent
	{ "&#221;",		"\xc3\x9d" },		// capital y, acute accent
	{ "&THORN;",	"\xc3\x9e" },		// capital THORN, Icelandic
	{ "&#222;",		"\xc3\x9e" },		// capital THORN, Icelandic
	{ "&szlig;",	"\xc3\x9f" },		// small sharp s, German
	{ "&#223;",		"\xc3\x9f" },		// small sharp s, German
	{ "&agrave;",	"\xc3\xa0" },		// small a, grave accent
	{ "&#224;",		"\xc3\xa0" },		// small a, grave accent
	{ "&aacute;",	"\xc3\xa1" },		// small a, acute accent
	{ "&#225;",		"\xc3\xa1" },		// small a, acute accent
	{ "&acirc;",	"\xc3\xa2" },		// small a, circumflex accent
	{ "&#226;",		"\xc3\xa2" },		// small a, circumflex accent
	{ "&atilde;",	"\xc3\xa3" },		// small a, tilde
	{ "&#227;",		"\xc3\xa3" },		// small a, tilde
	{ "&auml;",		"\xc3\xa4" },		// small a, umlaut mark
	{ "&#228;",		"\xc3\xa4" },		// small a, umlaut mark
	{ "&aring;",	"\xc3\xa5" },		// small a, ring
	{ "&#229;",		"\xc3\xa5" },		// small a, ring
	{ "&aelig;",	"\xc3\xa6" },		// small ae
	{ "&#230;",		"\xc3\xa6" },		// small ae
	{ "&ccedil;",	"\xc3\xa7" },		// small c, cedilla
	{ "&#231;",		"\xc3\xa7" },		// small c, cedilla
	{ "&egrave;",	"\xc3\xa8" },		// small e, grave accent
	{ "&#232;",		"\xc3\xa8" },		// small e, grave accent
	{ "&eacute;",	"\xc3\xa9" },		// small e, acute accent
	{ "&#233;",		"\xc3\xa9" },		// small e, acute accent
	{ "&ecirc;",	"\xc3\xaa" },		// small e, circumflex accent
	{ "&#234;",		"\xc3\xaa" },		// small e, circumflex accent
	{ "&euml;",		"\xc3\xab" },		// small e, umlaut mark
	{ "&#235;",		"\xc3\xab" },		// small e, umlaut mark
	{ "&igrave;",	"\xc3\xac" },		// small i, grave accent
	{ "&#236;",		"\xc3\xac" },		// small i, grave accent
	{ "&iacute;",	"\xc3\xad" },		// small i, acute accent
	{ "&#237;",		"\xc3\xad" },		// small i, acute accent
	{ "&icirc;",	"\xc3\xae" },		// small i, circumflex accent
	{ "&#238;",		"\xc3\xae" },		// small i, circumflex accent
	{ "&iuml;",		"\xc3\xaf" },		// small i, umlaut mark
	{ "&#239;",		"\xc3\xaf" },		// small i, umlaut mark
	{ "&eth;",		"\xc3\xb0" },		// small eth, Icelandic
	{ "&#240;",		"\xc3\xb0" },		// small eth, Icelandic
	{ "&ntilde;",	"\xc3\xb1" },		// small n, tilde
	{ "&#241;",		"\xc3\xb1" },		// small n, tilde
	{ "&ograve;",	"\xc3\xb2" },		// small o, grave accent
	{ "&#242;",		"\xc3\xb2" },		// small o, grave accent
	{ "&oacute;",	"\xc3\xb3" },		// small o, acute accent
	{ "&#243;",		"\xc3\xb3" },		// small o, acute accent
	{ "&ocirc;",	"\xc3\xb4" },		// small o, circumflex accent
	{ "&#244;",		"\xc3\xb4" },		// small o, circumflex accent
	{ "&otilde;",	"\xc3\xb5" },		// small o, tilde
	{ "&#245;",		"\xc3\xb5" },		// small o, tilde
	{ "&ouml;",		"\xc3\xb6" },		// small o, umlaut mark
	{ "&#246;",		"\xc3\xb6" },		// small o, umlaut mark
	{ "&oslash;",	"\xc3\xb8" },		// small o, slash
	{ "&#248;",		"\xc3\xb8" },		// small o, slash
	{ "&ugrave;",	"\xc3\xb9" },		// small u, grave accent
	{ "&#249;",		"\xc3\xb9" },		// small u, grave accent
	{ "&uacute;",	"\xc3\xba" },		// small u, acute accent
	{ "&#250;",		"\xc3\xba" },		// small u, acute accent
	{ "&ucirc;",	"\xc3\xbb" },		// small u, circumflex accent
	{ "&#251;",		"\xc3\xbb" },		// small u, circumflex accent
	{ "&uuml;",		"\xc3\xbc" },		// small u, umlaut mark
	{ "&#252;",		"\xc3\xbc" },		// small u, umlaut mark
	{ "&yacute;",	"\xc3\xbd" },		// small y, acute accent
	{ "&#253;",		"\xc3\xbd" },		// small y, acute accent
	{ "&thorn;",	"\xc3\xbe" },		// small thorn, Icelandic
	{ "&#254;",		"\xc3\xbe" },		// small thorn, Icelandic
	{ "&yuml;",		"\xc3\xbf" },		// small y, umlaut mark
	{ "&#255;",		"\xc3\xbf" },		// small y, umlaut mark
// Math Symbols Supported by HTML
	{ "&forall;",	"\xe2\x88\x80" },	// for all
	{ "&#8704;",	"\xe2\x88\x80" },	// for all
	{ "&part;"	,	"\xe2\x88\x82" },	// part
	{ "&#8706;",	"\xe2\x88\x82" },	// part
	{ "&exists;",	"\xe2\x88\x83" },	// exists
	{ "&#8707;",	"\xe2\x88\x83" },	// exists
	{ "&empty;",	"\xe2\x88\x85" },	// empty
	{ "&#8709;",	"\xe2\x88\x85" },	// empty
	{ "&nabla;",	"\xe2\x88\x87" },	// nabla
	{ "&#8711;",	"\xe2\x88\x87" },	// nabla
	{ "&isin;",		"\xe2\x88\x88" },	// isin
	{ "&#8712;",	"\xe2\x88\x88" },	// isin
	{ "&notin;",	"\xe2\x88\x89" },	// notin
	{ "&#8713;",	"\xe2\x88\x89" },	// notin
	{ "&ni;",		"\xe2\x88\x8b" },	// ni
	{ "&#8715;",	"\xe2\x88\x8b" },	// ni
	{ "&prod;",		"\xe2\x88\x8f" },	// prod
	{ "&#8719;",	"\xe2\x88\x8f" },	// prod
	{ "&sum;",		"\xe2\x88\x91" },	// sum
	{ "&#8721;",	"\xe2\x88\x91" },	// sum
	{ "&minus;",	"\xe2\x88\x92" },	// minus
	{ "&#8722;",	"\xe2\x88\x92" },	// minus
	{ "&lowast;",	"\xe2\x88\x97" },	// lowast
	{ "&#8727;",	"\xe2\x88\x97" },	// lowast
	{ "&radic;",	"\xe2\x88\x9a" },	// square root
	{ "&#8730;",	"\xe2\x88\x9a" },	// square root
	{ "&prop;",		"\xe2\x88\x9d" },	// proportional to
	{ "&#8733;",	"\xe2\x88\x9d" },	// proportional to
	{ "&infin;",	"\xe2\x88\x9e" },	// infinity
	{ "&#8734;",	"\xe2\x88\x9e" },	// infinity
	{ "&ang;",		"\xe2\x88\xa0" },	// angle
	{ "&#8736;",	"\xe2\x88\xa0" },	// angle
	{ "&and;",		"\xe2\x88\xa7" },	// and
	{ "&#8743;",	"\xe2\x88\xa7" },	// and
	{ "&or;",		"\xe2\x88\xa8" },	// or
	{ "&#8744;",	"\xe2\x88\xa8" },	// or
	{ "&cap;",		"\xe2\x88\xa9" },	// cap
	{ "&#8745;",	"\xe2\x88\xa9" },	// cap
	{ "&cup;",		"\xe2\x88\xaa" },	// cup
	{ "&#8746;",	"\xe2\x88\xaa" },	// cup
	{ "&int;",		"\xe2\x88\xab" },	// integral
	{ "&#8747;",	"\xe2\x88\xab" },	// integral
	{ "&there4;",	"\xe2\x88\xb4" },	// therefore
	{ "&#8756;",	"\xe2\x88\xb4" },	// therefore
	{ "&sim;",		"\xe2\x88\xbc" },	// simular to
	{ "&#8764;",	"\xe2\x88\xbc" },	// simular to
	{ "&cong;",		"\xe2\x89\x85" },	// approximately equal
	{ "&#8773;",	"\xe2\x89\x85" },	// approximately equal
	{ "&asymp;",	"\xe2\x89\x88" },	// almost equal
	{ "&#8776;",	"\xe2\x89\x88" },	// almost equal
	{ "&ne;",		"\xe2\x89\xa0" },	// not equal
	{ "&#8800;",	"\xe2\x89\xa0" },	// not equal
	{ "&equiv;",	"\xe2\x89\xa1" },	// equivalent
	{ "&#8801;",	"\xe2\x89\xa1" },	// equivalent
	{ "&le;",		"\xe2\x89\xa4" },	// less or equal
	{ "&#8804;",	"\xe2\x89\xa4" },	// less or equal
	{ "&ge;",		"\xe2\x89\xa5" },	// greater or equal
	{ "&#8805;",	"\xe2\x89\xa5" },	// greater or equal
	{ "&sub;",		"\xe2\x8a\x82" },	// subset of
	{ "&#8834;",	"\xe2\x8a\x82" },	// subset of
	{ "&sup;",		"\xe2\x8a\x83" },	// superset of
	{ "&#8835;",	"\xe2\x8a\x83" },	// superset of
	{ "&nsub;",		"\xe2\x8a\x84" },	// not subset of
	{ "&#8836;",	"\xe2\x8a\x84" },	// not subset of
	{ "&sube;",		"\xe2\x8a\x86" },	// subset or equal
	{ "&#8838;",	"\xe2\x8a\x86" },	// subset or equal
	{ "&supe;",		"\xe2\x8a\x87" },	// superset or equal
	{ "&#8839;",	"\xe2\x8a\x87" },	// superset or equal
	{ "&oplus;",	"\xe2\x8a\x95" },	// circled plus
	{ "&#8853;",	"\xe2\x8a\x95" },	// circled plus
	{ "&otimes;",	"\xe2\x8a\x97" },	// cirled times
	{ "&#8855;",	"\xe2\x8a\x97" },	// cirled times
	{ "&perp;",		"\xe2\x8a\xa5" },	// perpendicular
	{ "&#8869;",	"\xe2\x8a\xa5" },	// perpendicular
	{ "&sdot;",		"\xe2\x8b\x85" },	// dot operator
	{ "&#8901;",	"\xe2\x8b\x85" },	// dot operator
// Greek Letters Supported by HTML
	{ "&Alpha;",	"\xce\x91" },		// Alpha
	{ "&#913;",		"\xce\x91" },		// Alpha
	{ "&Beta;",		"\xce\x92" },		// Beta
	{ "&#914;",		"\xce\x92" },		// Beta
	{ "&Gamma;",	"\xce\x93" },		// Gamma
	{ "&#915;",		"\xce\x93" },		// Gamma
	{ "&Delta;",	"\xce\x94" },		// Delta
	{ "&#916;",		"\xce\x94" },		// Delta
	{ "&Epsilon;",	"\xce\x95" },		// Epsilon
	{ "&#917;",		"\xce\x95" },		// Epsilon
	{ "&Zeta;",		"\xce\x96" },		// Zeta
	{ "&#918;",		"\xce\x96" },		// Zeta
	{ "&Eta;",		"\xce\x97" },		// Eta
	{ "&#919;",		"\xce\x97" },		// Eta
	{ "&Theta;",	"\xce\x98" },		// Theta
	{ "&#920;",		"\xce\x98" },		// Theta
	{ "&Iota;",		"\xce\x99" },		// Iota
	{ "&#921;",		"\xce\x99" },		// Iota
	{ "&Kappa;",	"\xce\x9a" },		// Kappa
	{ "&#922;",		"\xce\x9a" },		// Kappa
	{ "&Lambda;",	"\xce\x9b" },		// Lambda
	{ "&#923;",		"\xce\x9b" },		// Lambda
	{ "&Mu;",		"\xce\x9c" },		// Mu
	{ "&#924;",		"\xce\x9c" },		// Mu
	{ "&Nu;",		"\xce\x9d" },		// Nu
	{ "&#925;",		"\xce\x9d" },		// Nu
	{ "&Xi;",		"\xce\x9e" },		// Xi
	{ "&#926;",		"\xce\x9e" },		// Xi
	{ "&Omicron;",	"\xce\x9f" },		// Omicron
	{ "&#927;",		"\xce\x9f" },		// Omicron
	{ "&Pi;",		"\xce\xa0" },		// Pi
	{ "&#928;",		"\xce\xa0" },		// Pi
	{ "&Rho;",		"\xce\xa1" },		// Rho
	{ "&#929;",		"\xce\xa1" },		// Rho
	{ "&Sigma;",	"\xce\xa3" },		// Sigma
	{ "&#931;",		"\xce\xa3" },		// Sigma
	{ "&Tau;",		"\xce\xa4" },		// Tau
	{ "&#932;",		"\xce\xa4" },		// Tau
	{ "&Upsilon;",	"\xce\xa5" },		// Upsilon
	{ "&#933;",		"\xce\xa5" },		// Upsilon
	{ "&Phi;",		"\xce\xa6" },		// Phi
	{ "&#934;",		"\xce\xa6" },		// Phi
	{ "&Chi;",		"\xce\xa7" },		// Chi
	{ "&#935;",		"\xce\xa7" },		// Chi
	{ "&Psi;",		"\xce\xa8" },		// Psi
	{ "&#936;",		"\xce\xa8" },		// Psi
	{ "&Omega;",	"\xce\xa9" },		// Omega
	{ "&#937;",		"\xce\xa9" },		// Omega
	{ "&alpha;",	"\xce\xb1" },		// alpha
	{ "&#945;",		"\xce\xb1" },		// alpha
	{ "&beta;",		"\xce\xb2" },		// beta
	{ "&#946;",		"\xce\xb2" },		// beta
	{ "&gamma;",	"\xce\xb3" },		// gamma
	{ "&#947;",		"\xce\xb3" },		// gamma
	{ "&delta;",	"\xce\xb4" },		// delta
	{ "&#948;",		"\xce\xb4" },		// delta
	{ "&epsilon;",	"\xce\xb5" },		// epsilon
	{ "&#949;",		"\xce\xb5" },		// epsilon
	{ "&zeta;",		"\xce\xb6" },		// zeta
	{ "&#950;",		"\xce\xb6" },		// zeta
	{ "&zeta;",		"\xce\xb6" },		// zeta
	{ "&#950;",		"\xce\xb6" },		// zeta
	{ "&eta;",		"\xce\xb7" },		// eta
	{ "&#951;",		"\xce\xb7" },		// eta
	{ "&theta;",	"\xce\xb8" },		// theta
	{ "&#952;",		"\xce\xb8" },		// theta
	{ "&iota;",		"\xce\xb9" },		// iota
	{ "&#953;",		"\xce\xb9" },		// iota
	{ "&kappa;",	"\xce\xba" },		// kappa
	{ "&#954;",		"\xce\xba" },		// kappa
	{ "&lambda;",	"\xce\xbb" },		// lambda
	{ "&#955;",		"\xce\xbb" },		// lambda
	{ "&mu;",		"\xce\xbc" },		// mu
	{ "&#956;",		"\xce\xbc" },		// mu
	{ "&nu;",		"\xce\xbd" },		// nu
	{ "&#957;",		"\xce\xbd" },		// nu
	{ "&xi;",		"\xce\xbe" },		// xi
	{ "&#958;",		"\xce\xbe" },		// xi
	{ "&omicron;",	"\xce\xbf" },		// omicron
	{ "&#959;",		"\xce\xbf" },		// omicron
	{ "&pi;",		"\xcf\x80" },		// pi
	{ "&#960;",		"\xcf\x80" },		// pi
	{ "&rho;",		"\xcf\x81" },		// rho
	{ "&#961;",		"\xcf\x81" },		// rho
	{ "&sigmaf;",	"\xcf\x82" },		// sigmaf
	{ "&#962;",		"\xcf\x82" },		// sigmaf
	{ "&sigma;",	"\xcf\x83" },		// sigma
	{ "&#963;",		"\xcf\x83" },		// sigma
	{ "&tau;",		"\xcf\x84" },		// tau
	{ "&#964;",		"\xcf\x84" },		// tau
	{ "&upsilon;",	"\xcf\x85" },		// upsilon
	{ "&#965;",		"\xcf\x85" },		// upsilon
	{ "&phi;",		"\xcf\x86" },		// phi
	{ "&#966;",		"\xcf\x86" },		// phi
	{ "&chi;",		"\xcf\x87" },		// chi
	{ "&#967;",		"\xcf\x87" },		// chi
	{ "&psi;",		"\xcf\x88" },		// psi
	{ "&#968;",		"\xcf\x88" },		// psi
	{ "&omega;",	"\xcf\x89" },		// omega
	{ "&#969;",		"\xcf\x89" },		// omega
	{ "&thetasym;",	"\xcf\x91" },		// theta symbol
	{ "&#977;",		"\xcf\x91" },		// theta symbol
	{ "&upsih;",	"\xcf\x92" },		// upsilon symbol
	{ "&#978;",		"\xcf\x92" },		// upsilon symbol
	{ "&piv;",		"\xcf\x96" },		// pi symbol
	{ "&#982;",		"\xcf\x96" },		// pi symbol
// Some Other Entities Supported by HTML
	{ "&OElig;",	"\xc5\x92" },		// capital ligature OE
	{ "&#338;",		"\xc5\x92" },		// capital ligature OE
	{ "&oelig;",	"\xc5\x93" },		// small ligature oe
	{ "&#339;",		"\xc5\x93" },		// small ligature oe
	{ "&Scaron;",	"\xc5\xa0" },		// capital S with caron
	{ "&#352;",		"\xc5\xa0" },		// capital S with caron
	{ "&scaron;",	"\xc5\xa1" },		// small S with caron
	{ "&#353;",		"\xc5\xa1" },		// small S with caron
	{ "&Yuml;",		"\xc5\xb8" },		// capital Y with diaeres
	{ "&#376;",		"\xc5\xb8" },		// capital Y with diaeres
	{ "&fnof;",		"\xc6\x92" },		// f with hook
	{ "&#402;",		"\xc6\x92" },		// f with hook
	{ "&circ;",		"\xcb\x86" },		// modifier letter circumflex accent
	{ "&#710;",		"\xcb\x86" },		// modifier letter circumflex accent
	{ "&tilde;",	"\xcb\x9c" },		// small tilde
	{ "&#732;",		"\xcb\x9c" },		// small tilde
	{ "&ensp;",		"\xe2\x80\x82" },	// en space
	{ "&#8194;",	"\xe2\x80\x82" },	// en space
	{ "&emsp;",		"\xe2\x80\x83" },	// em space
	{ "&#8195;",	"\xe2\x80\x83" },	// em space
	{ "&thinsp;",	"\xe2\x80\x89" },	// thin space
	{ "&#8201;",	"\xe2\x80\x89" },	// thin space
	{ "&zwnj;",		"\xe2\x80\x8c" },	// zero width non-joiner
	{ "&#8204;",	"\xe2\x80\x8c" },	// zero width non-joiner
	{ "&zwnj;",		"\xe2\x80\x8d" },	// zero width joiner
	{ "&#8205;",	"\xe2\x80\x8d" },	// zero width joiner
	{ "&lrm;",		"\xe2\x80\x8e" },	// left-to-right mark
	{ "&#8206;",	"\xe2\x80\x8e" },	// left-to-right mark
	{ "&rlm;",		"\xe2\x80\x8f" },	// right-to-left mark
	{ "&#8207;",	"\xe2\x80\x8f" },	// right-to-left mark
	{ "&ndash;",	"\xe2\x80\x93" },	// en dash
	{ "&#8211;",	"\xe2\x80\x93" },	// en dash
	{ "&mdash;",	"\xe2\x80\x94" },	// em dash
	{ "&#8212;",	"\xe2\x80\x94" },	// em dash
	{ "&mdash;",	"\xe2\x80\x94" },	// em dash
	{ "&#8212;",	"\xe2\x80\x94" },	// em dash
	{ "&lsquo;",	"\xe2\x80\x98" },	// left single quotation mark
	{ "&#8216;",	"\xe2\x80\x98" },	// left single quotation mark
	{ "&rsquo;",	"\xe2\x80\x99" },	// right single quotation mark
	{ "&#8217;",	"\xe2\x80\x99" },	// right single quotation mark
	{ "&sbquo;",	"\xe2\x80\x9a" },	// single low-9 quotation mark
	{ "&#8218;",	"\xe2\x80\x9a" },	// single low-9 quotation mark
	{ "&ldquo;",	"\xe2\x80\x9c" },	// left double quotation mark
	{ "&#8220;",	"\xe2\x80\x9c" },	// left double quotation mark
	{ "&rdquo;",	"\xe2\x80\x9d" },	// right double quotation mark
	{ "&#8221;",	"\xe2\x80\x9d" },	// right double quotation mark
	{ "&bdquo;",	"\xe2\x80\x9e" },	// double low-9 quotation mark
	{ "&#8222;",	"\xe2\x80\x9e" },	// double low-9 quotation mark
	{ "&dagger;",	"\xe2\x80\xa0" },	// dagger
	{ "&#8224;",	"\xe2\x80\xa0" },	// dagger
	{ "&Dagger;",	"\xe2\x80\xa1" },	// double dagger
	{ "&#8225;",	"\xe2\x80\xa1" },	// double dagger
	{ "&bull;",		"\xe2\x80\xa2" },	// bullet
	{ "&#8226;",	"\xe2\x80\xa2" },	// bullet
	{ "&hellip;",	"\xe2\x80\xa6" },	// horizontal ellipsis
	{ "&#8230;",	"\xe2\x80\xa6" },	// horizontal ellipsis
	{ "&permil;",	"\xe2\x80\xb0" },	// per mille
	{ "&#8240;",	"\xe2\x80\xb0" },	// per mille
	{ "&prime;",	"\xe2\x80\xb2" },	// minutes
	{ "&#8242;",	"\xe2\x80\xb2" },	// minutes
	{ "&Prime;",	"\xe2\x80\xb3" },	// seconds
	{ "&#8243;",	"\xe2\x80\xb3" },	// seconds
	{ "&lsaquo;",	"\xe2\x80\xb9" },	// single left angle quotation
	{ "&#8249;",	"\xe2\x80\xb9" },	// single left angle quotation
	{ "&rsaquo;",	"\xe2\x80\xba" },	// single right angle quotation
	{ "&#8250;",	"\xe2\x80\xba" },	// single right angle quotation
	{ "&oline;",	"\xe2\x80\xbe" },	// overline
	{ "&#8254;",	"\xe2\x80\xbe" },	// overline
	{ "&euro;",		"\xe2\x82\xac" },	// euro
	{ "&#8364;",	"\xe2\x82\xac" },	// euro
	{ "&trade;",	"\xe2\x84\xa2" },	// trademark
	{ "&#8482;",	"\xe2\x84\xa2" },	// trademark
	{ "&larr;",		"\xe2\x86\x90" },	// left arrow
	{ "&#8592;",	"\xe2\x86\x90" },	// left arrow
	{ "&uarr;",		"\xe2\x86\x91" },	// up arrow
	{ "&#8593;",	"\xe2\x86\x91" },	// up arrow
	{ "&rarr;",		"\xe2\x86\x92" },	// right arrow
	{ "&#8594;",	"\xe2\x86\x92" },	// right arrow
	{ "&darr;",		"\xe2\x86\x93" },	// down arrow
	{ "&#8595;",	"\xe2\x86\x93" },	// down arrow
	{ "&harr;",		"\xe2\x86\x94" },	// left right arrow
	{ "&#8596;",	"\xe2\x86\x94" },	// left right arrow
	{ "&crarr;",	"\xe2\x86\xb5" },	// carriage return arrow
	{ "&#8629;",	"\xe2\x86\xb5" },	// carriage return arrow
	{ "&lceil;",	"\xe2\x8c\x88" },	// left ceiling
	{ "&#8968;",	"\xe2\x8c\x88" },	// left ceiling
	{ "&rceil;",	"\xe2\x8c\x89" },	// right ceiling
	{ "&#8969;",	"\xe2\x8c\x89" },	// right ceiling
	{ "&lfloor;",	"\xe2\x8c\x8a" },	// left floor
	{ "&#8970;",	"\xe2\x8c\x8a" },	// left floor
	{ "&rfloor;",	"\xe2\x8c\x8b" },	// right floor
	{ "&#8971;",	"\xe2\x8c\x8b" },	// right floor
	{ "&loz;",		"\xe2\x97\x8a" },	// lozenge
	{ "&#9674;",	"\xe2\x97\x8a" },	// lozenge
	{ "&spades;",	"\xe2\x99\xa0" },	// spade
	{ "&#9824;",	"\xe2\x99\xa0" },	// spade
	{ "&clubs;",	"\xe2\x99\xa3" },	// club
	{ "&#9827;",	"\xe2\x99\xa3" },	// club
	{ "&hearts;",	"\xe2\x99\xa5" },	// heart
	{ "&#9829;",	"\xe2\x99\xa5" },	// heart
	{ "&diams;",	"\xe2\x99\xa6" },	// diamond
	{ "&#9830;",	"\xe2\x99\xa6" },	// diamond
};

//! 文字実体参照(Character entity reference)をデコードする
/*!
	\a pszText に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@code
		// "&lt;abcde&gt;" → "<abcde>"
		int nSize = Cat_CharacterEntityReferenceDecode( "&lt;abcde&gt;", 0 );	// 必要なバッファサイズを取得
		char* pszText = malloc( nSize );
		Cat_CharacterEntityReferenceDecode( "&lt;abcde&gt;", pszText );			// 実際に変換
	@endcode

	@param[in]	pszHTML	デコードする文字列(UTF-8)
	@param[out]	pszText	デコードされた文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_CharacterEntityReferenceDecode( const char* pszHTML, char* pszText )
{
	const char* p;
	char* pOut;
	int nLength;

	/* 引数チェック */
	if(pszHTML == 0) {
		return 0;
	}

	p = pszHTML;
	pOut = pszText;

	if(pOut) {
		*pOut = 0;
	}
	nLength = 1;
	while(*p) {
		if(*p == '&') {
			int i;
			for(i = 0; i < sizeof(tblConvert)/sizeof(tblConvert[0]); i++) {
				if(strncmp( p, tblConvert[i].pszKey, strlen( tblConvert[i].pszKey ) ) == 0) {
					if(pOut) {
						strcat( pOut, tblConvert[i].pszText );
						pOut += strlen( tblConvert[i].pszText );
					}
					p += strlen( tblConvert[i].pszKey );
					nLength += strlen( tblConvert[i].pszText );
					break;
				}
			}
			if(i >= sizeof(tblConvert)/sizeof(tblConvert[0])) {
				if(pOut) {
					*pOut++ = *p;
					*pOut   = 0;
				}
				p++;
				nLength++;
			}
		} else {
			if(pOut) {
				*pOut++ = *p;
				*pOut   = 0;
			}
			p++;
			nLength++;
		}
	}

	return nLength;
}

//! 文字実体参照(Character entity reference)へエンコードする
/*!
	\a pszHTML に0を渡すと、変換に必要なバッファサイズを計算することができる。

	@code
		// "<abcde>" → "&lt;abcde&gt;"
		int nSize = Cat_CharacterEntityReferenceEncode( "<abcde>", 0 );	// 必要なバッファサイズを取得
		char* pszHTML = malloc( nSize );
		Cat_CharacterEntityReferenceEncode( "<abcde>", pszHTML );		// 実際に変換
	@endcode

	@param[in]	pszText	エンコードする文字列(UTF-8)
	@param[out]	pszHTML	エンコードされた文字列(UTF-8) (0を渡すことも可)
	@return	NULLターミネイトを含んだ変換に必要なバッファのサイズ(バイト単位)
*/
int
Cat_CharacterEntityReferenceEncode( const char* pszText, char* pszHTML )
{
	const char* p;
	char* pOut;
	int nLength;

	/* 引数チェック */
	if(pszText == 0) {
		return 0;
	}

	p = pszText;
	pOut = pszHTML;

	if(pOut) {
		*pOut = 0;
	}
	nLength = 1;
	while(*p) {
		int i;
		for(i = 0; i < sizeof(tblConvert)/sizeof(tblConvert[0]); i++) {
			if(strncmp( p, tblConvert[i].pszText, strlen( tblConvert[i].pszText ) ) == 0) {
				if(pOut) {
					strcat( pOut, tblConvert[i].pszKey );
					pOut += strlen( tblConvert[i].pszKey );
				}
				p += strlen( tblConvert[i].pszText );
				nLength += strlen( tblConvert[i].pszKey );
				break;
			}
		}
		if(i >= sizeof(tblConvert)/sizeof(tblConvert[0])) {
			if(pOut) {
				*pOut++ = *p;
				*pOut   = 0;
			}
			p++;
			nLength++;
		}
	}

	return nLength;
}
