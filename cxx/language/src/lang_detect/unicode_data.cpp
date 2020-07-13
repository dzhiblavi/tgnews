#include "./unicode_data.h"
#include <set>

namespace langdetect {

std::set<uint32_t> const UnicodeData::uppercases_ = {0x0041, 0x0042, 0x0043, 0x0044, 0x0045, 0x0046, 0x0047, 0x0048, 0x0049, 0x004A, 0x004B, 0x004C, 0x004D, 0x004E, 0x004F, 0x0050, 0x0051, 0x0052, 0x0053, 0x0054, 0x0055, 0x0056, 0x0057, 0x0058, 0x0059, 0x005A, 0x00C0, 0x00C1, 0x00C2, 0x00C3, 0x00C4, 0x00C5, 0x00C6, 0x00C7, 0x00C8, 0x00C9, 0x00CA, 0x00CB, 0x00CC, 0x00CD, 0x00CE, 0x00CF, 0x00D0, 0x00D1, 0x00D2, 0x00D3, 0x00D4, 0x00D5, 0x00D6, 0x00D8, 0x00D9, 0x00DA, 0x00DB, 0x00DC, 0x00DD, 0x00DE, 0x0178, 0x0100, 0x0102, 0x0104, 0x0106, 0x0108, 0x010A, 0x010C, 0x010E, 0x0110, 0x0112, 0x0114, 0x0116, 0x0118, 0x011A, 0x011C, 0x011E, 0x0120, 0x0122, 0x0124, 0x0126, 0x0128, 0x012A, 0x012C, 0x012E, 0x0049, 0x0132, 0x0134, 0x0136, 0x0139, 0x013B, 0x013D, 0x013F, 0x0141, 0x0143, 0x0145, 0x0147, 0x014A, 0x014C, 0x014E, 0x0150, 0x0152, 0x0154, 0x0156, 0x0158, 0x015A, 0x015C, 0x015E, 0x0160, 0x0162, 0x0164, 0x0166, 0x0168, 0x016A, 0x016C, 0x016E, 0x0170, 0x0172, 0x0174, 0x0176, 0x0179, 0x017B, 0x017D, 0x0182, 0x0184, 0x0187, 0x018B, 0x0191, 0x0198, 0x01A0, 0x01A2, 0x01A4, 0x01A7, 0x01AC, 0x01AF, 0x01B3, 0x01B5, 0x01B8, 0x01BC, 0x01C4, 0x01C7, 0x01CA, 0x01CD, 0x01CF, 0x01D1, 0x01D3, 0x01D5, 0x01D7, 0x01D9, 0x01DB, 0x01DE, 0x01E0, 0x01E2, 0x01E4, 0x01E6, 0x01E8, 0x01EA, 0x01EC, 0x01EE, 0x01F1, 0x01F4, 0x01FA, 0x01FC, 0x01FE, 0x0200, 0x0202, 0x0204, 0x0206, 0x0208, 0x020A, 0x020C, 0x020E, 0x0210, 0x0212, 0x0214, 0x0216, 0x0181, 0x0186, 0x018A, 0x018E, 0x018F, 0x0190, 0x0193, 0x0194, 0x0197, 0x0196, 0x019C, 0x019D, 0x019F, 0x01A9, 0x01AE, 0x01B1, 0x01B2, 0x01B7, 0x0386, 0x0388, 0x0389, 0x038A, 0x0391, 0x0392, 0x0393, 0x0394, 0x0395, 0x0396, 0x0397, 0x0398, 0x0399, 0x039A, 0x039B, 0x039C, 0x039D, 0x039E, 0x039F, 0x03A0, 0x03A1, 0x03A3, 0x03A4, 0x03A5, 0x03A6, 0x03A7, 0x03A8, 0x03A9, 0x03AA, 0x03AB, 0x038C, 0x038E, 0x038F, 0x03E2, 0x03E4, 0x03E6, 0x03E8, 0x03EA, 0x03EC, 0x03EE, 0x0410, 0x0411, 0x0412, 0x0413, 0x0414, 0x0415, 0x0416, 0x0417, 0x0418, 0x0419, 0x041A, 0x041B, 0x041C, 0x041D, 0x041E, 0x041F, 0x0420, 0x0421, 0x0422, 0x0423, 0x0424, 0x0425, 0x0426, 0x0427, 0x0428, 0x0429, 0x042A, 0x042B, 0x042C, 0x042D, 0x042E, 0x042F, 0x0401, 0x0402, 0x0403, 0x0404, 0x0405, 0x0406, 0x0407, 0x0408, 0x0409, 0x040A, 0x040B, 0x040C, 0x040E, 0x040F, 0x0460, 0x0462, 0x0464, 0x0466, 0x0468, 0x046A, 0x046C, 0x046E, 0x0470, 0x0472, 0x0474, 0x0476, 0x0478, 0x047A, 0x047C, 0x047E, 0x0480, 0x0490, 0x0492, 0x0494, 0x0496, 0x0498, 0x049A, 0x049C, 0x049E, 0x04A0, 0x04A2, 0x04A4, 0x04A6, 0x04A8, 0x04AA, 0x04AC, 0x04AE, 0x04B0, 0x04B2, 0x04B4, 0x04B6, 0x04B8, 0x04BA, 0x04BC, 0x04BE, 0x04C1, 0x04C3, 0x04C7, 0x04CB, 0x04D0, 0x04D2, 0x04D4, 0x04D6, 0x04D8, 0x04DA, 0x04DC, 0x04DE, 0x04E0, 0x04E2, 0x04E4, 0x04E6, 0x04E8, 0x04EA, 0x04EE, 0x04F0, 0x04F2, 0x04F4, 0x04F8, 0x0531, 0x0532, 0x0533, 0x0534, 0x0535, 0x0536, 0x0537, 0x0538, 0x0539, 0x053A, 0x053B, 0x053C, 0x053D, 0x053E, 0x053F, 0x0540, 0x0541, 0x0542, 0x0543, 0x0544, 0x0545, 0x0546, 0x0547, 0x0548, 0x0549, 0x054A, 0x054B, 0x054C, 0x054D, 0x054E, 0x054F, 0x0550, 0x0551, 0x0552, 0x0553, 0x0554, 0x0555, 0x0556, 0x10A0, 0x10A1, 0x10A2, 0x10A3, 0x10A4, 0x10A5, 0x10A6, 0x10A7, 0x10A8, 0x10A9, 0x10AA, 0x10AB, 0x10AC, 0x10AD, 0x10AE, 0x10AF, 0x10B0, 0x10B1, 0x10B2, 0x10B3, 0x10B4, 0x10B5, 0x10B6, 0x10B7, 0x10B8, 0x10B9, 0x10BA, 0x10BB, 0x10BC, 0x10BD, 0x10BE, 0x10BF, 0x10C0, 0x10C1, 0x10C2, 0x10C3, 0x10C4, 0x10C5, 0x1E00, 0x1E02, 0x1E04, 0x1E06, 0x1E08, 0x1E0A, 0x1E0C, 0x1E0E, 0x1E10, 0x1E12, 0x1E14, 0x1E16, 0x1E18, 0x1E1A, 0x1E1C, 0x1E1E, 0x1E20, 0x1E22, 0x1E24, 0x1E26, 0x1E28, 0x1E2A, 0x1E2C, 0x1E2E, 0x1E30, 0x1E32, 0x1E34, 0x1E36, 0x1E38, 0x1E3A, 0x1E3C, 0x1E3E, 0x1E40, 0x1E42, 0x1E44, 0x1E46, 0x1E48, 0x1E4A, 0x1E4C, 0x1E4E, 0x1E50, 0x1E52, 0x1E54, 0x1E56, 0x1E58, 0x1E5A, 0x1E5C, 0x1E5E, 0x1E60, 0x1E62, 0x1E64, 0x1E66, 0x1E68, 0x1E6A, 0x1E6C, 0x1E6E, 0x1E70, 0x1E72, 0x1E74, 0x1E76, 0x1E78, 0x1E7A, 0x1E7C, 0x1E7E, 0x1E80, 0x1E82, 0x1E84, 0x1E86, 0x1E88, 0x1E8A, 0x1E8C, 0x1E8E, 0x1E90, 0x1E92, 0x1E94, 0x1EA0, 0x1EA2, 0x1EA4, 0x1EA6, 0x1EA8, 0x1EAA, 0x1EAC, 0x1EAE, 0x1EB0, 0x1EB2, 0x1EB4, 0x1EB6, 0x1EB8, 0x1EBA, 0x1EBC, 0x1EBE, 0x1EC0, 0x1EC2, 0x1EC4, 0x1EC6, 0x1EC8, 0x1ECA, 0x1ECC, 0x1ECE, 0x1ED0, 0x1ED2, 0x1ED4, 0x1ED6, 0x1ED8, 0x1EDA, 0x1EDC, 0x1EDE, 0x1EE0, 0x1EE2, 0x1EE4, 0x1EE6, 0x1EE8, 0x1EEA, 0x1EEC, 0x1EEE, 0x1EF0, 0x1EF2, 0x1EF4, 0x1EF6, 0x1EF8, 0x1F08, 0x1F09, 0x1F0A, 0x1F0B, 0x1F0C, 0x1F0D, 0x1F0E, 0x1F0F, 0x1F18, 0x1F19, 0x1F1A, 0x1F1B, 0x1F1C, 0x1F1D, 0x1F28, 0x1F29, 0x1F2A, 0x1F2B, 0x1F2C, 0x1F2D, 0x1F2E, 0x1F2F, 0x1F38, 0x1F39, 0x1F3A, 0x1F3B, 0x1F3C, 0x1F3D, 0x1F3E, 0x1F3F, 0x1F48, 0x1F49, 0x1F4A, 0x1F4B, 0x1F4C, 0x1F4D, 0x1F59, 0x1F5B, 0x1F5D, 0x1F5F, 0x1F68, 0x1F69, 0x1F6A, 0x1F6B, 0x1F6C, 0x1F6D, 0x1F6E, 0x1F6F, 0x1F88, 0x1F89, 0x1F8A, 0x1F8B, 0x1F8C, 0x1F8D, 0x1F8E, 0x1F8F, 0x1F98, 0x1F99, 0x1F9A, 0x1F9B, 0x1F9C, 0x1F9D, 0x1F9E, 0x1F9F, 0x1FA8, 0x1FA9, 0x1FAA, 0x1FAB, 0x1FAC, 0x1FAD, 0x1FAE, 0x1FAF, 0x1FB8, 0x1FB9, 0x1FD8, 0x1FD9, 0x1FE8, 0x1FE9, 0x24B6, 0x24B7, 0x24B8, 0x24B9, 0x24BA, 0x24BB, 0x24BC, 0x24BD, 0x24BE, 0x24BF, 0x24C0, 0x24C1, 0x24C2, 0x24C3, 0x24C4, 0x24C5, 0x24C6, 0x24C7, 0x24C8, 0x24C9, 0x24CA, 0x24CB, 0x24CC, 0x24CD, 0x24CE, 0x24CF, 0xFF21, 0xFF22, 0xFF23, 0xFF24, 0xFF25, 0xFF26, 0xFF27, 0xFF28, 0xFF29, 0xFF2A, 0xFF2B, 0xFF2C, 0xFF2D, 0xFF2E, 0xFF2F, 0xFF30, 0xFF31, 0xFF32, 0xFF33, 0xFF34, 0xFF35, 0xFF36, 0xFF37, 0xFF38, 0xFF39, 0xFF3A};

UnicodeBlock UnicodeData::unicodeblock(uint32_t const &code) {
    if(code <= 0x7f) {
        return UNICODE_BLOCK_BASIC_LATIN;
    } else if(code <= 0xff) {
        return UNICODE_BLOCK_LATIN_1_SUPPLEMENT;
    } else if(code <= 0x0fff) {
        if(code <= 0x02ff) {
            if(code <= 0x017f) return UNICODE_BLOCK_LATIN_EXTENDED_A;
            else if(code <= 0x024f) return UNICODE_BLOCK_LATIN_EXTENDED_B;
            else if(code <= 0x02af) return UNICODE_BLOCK_IPA_EXTENSIONS;
            else return UNICODE_BLOCK_SPACING_MODIFIER_LETTERS;
        } else if(code <= 0x03ff) {
            if(code <= 0x036f) return UNICODE_BLOCK_COMBINING_DIACRITICAL_MARKS;
            else return UNICODE_BLOCK_GREEK_AND_COPTIC;
        } else if(code <= 0x04ff) {
            return UNICODE_BLOCK_CYRILLIC;
        } else if(code <= 0x05ff) {
            if(code <= 0x052f) return UNICODE_BLOCK_CYRILLIC_SUPPLEMENT;
            else if(code <= 0x058f) return UNICODE_BLOCK_ARMENIAN;
            else return UNICODE_BLOCK_HEBREW;
        } else if(code <= 0x06ff) {
            return UNICODE_BLOCK_ARABIC;
        } else if(code <= 0x07ff) {
            if(code <= 0x074f) return UNICODE_BLOCK_SYRIAC;
            else if(code <= 0x077f) return UNICODE_BLOCK_ARABIC_SUPPLEMENT;
            else if(code <= 0x07bf) return UNICODE_BLOCK_THAANA;
            else return UNICODE_BLOCK_NKO;
        } else if(code <= 0x08ff) {
            if(code <= 0x083f) return UNICODE_BLOCK_SAMARITAN;
            else if(code <= 0x085f) return UNICODE_BLOCK_MANDAIC;
            else return UNICODE_BLOCK_ARABIC_EXTENDED_A;
        } else if(code <= 0x09ff) {
            if(code <= 0x097f) return UNICODE_BLOCK_DEVANAGARI;
            else return UNICODE_BLOCK_BENGALI;
        } else if(code <= 0x0aff) {
            if(code <= 0x0a7f) return UNICODE_BLOCK_GURMUKHI;
            else return UNICODE_BLOCK_GUJARATI;
        } else if(code <= 0x0bff) {
            if(code <= 0x0b7f) return UNICODE_BLOCK_ORIYA;
            else return UNICODE_BLOCK_TAMIL;
        } else if(code <= 0x0cff) {
            if(code <= 0x0c7f) return UNICODE_BLOCK_TELUGU;
            else return UNICODE_BLOCK_KANNADA;
        } else if(code <= 0x0dff) {
            if(code <= 0x0d7f) return UNICODE_BLOCK_MALAYALAM;
            else return UNICODE_BLOCK_SINHALA;
        } else if(code <= 0x0eff) {
            if(code <= 0x0e7f) return UNICODE_BLOCK_THAI;
            else return UNICODE_BLOCK_LAO;
        } else {
            return UNICODE_BLOCK_TIBETAN;
        }
    } else if(code <= 0x1fff) {
        if(code <= 0x10ff) {
            if(code <= 0x109f) return UNICODE_BLOCK_MYANMAR;
            else return UNICODE_BLOCK_GEORGIAN;
        } else if(code <= 0x11ff) {
            return UNICODE_BLOCK_HANGUL_JAMO;
        } else if(code <= 0x13ff) {
            if(code <= 0x137f) return UNICODE_BLOCK_ETHIOPIC;
            else if(code <= 0x139f) return UNICODE_BLOCK_ETHIOPIC_SUPPLEMENT;
            else return UNICODE_BLOCK_CHEROKEE;
        } else if(code <= 0x16ff) {
            if(code <= 0x167f) return UNICODE_BLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS;
            else if(code <= 0x169f) return UNICODE_BLOCK_OGHAM;
            else return UNICODE_BLOCK_RUNIC;
        } else if(code <= 0x17ff) {
            if(code <= 0x171f) return UNICODE_BLOCK_TAGALOG;
            else if(code <= 0x173f) return UNICODE_BLOCK_HANUNOO;
            else if(code <= 0x175f) return UNICODE_BLOCK_BUHID;
            else if(code <= 0x177f) return UNICODE_BLOCK_TAGBANWA;
            else return UNICODE_BLOCK_KHMER;
        } else if(code <= 0x18ff) {
            if(code <= 0x18af) return UNICODE_BLOCK_MONGOLIAN;
            else return UNICODE_BLOCK_UNIFIED_CANADIAN_ABORIGINAL_SYLLABICS_EXTENDED;
        } else if(code <= 0x19ff) {
            if(code <= 0x194f) return UNICODE_BLOCK_LIMBU;
            else if(code <= 0x197f) return UNICODE_BLOCK_TAI_LE;
            else if(code <= 0x19df) return UNICODE_BLOCK_NEW_TAI_LUE;
            else  return UNICODE_BLOCK_KHMER_SYMBOLS;
        } else if(code <= 0x1bff) {
            if(code <= 0x1a1f) return UNICODE_BLOCK_BUGINESE;
            else if(code <= 0x1aaf) return UNICODE_BLOCK_TAI_THAM;
            else if(code <= 0x1b7f) return UNICODE_BLOCK_BALINESE;
            else if(code <= 0x1bbf) return UNICODE_BLOCK_SUNDANESE;
            else return UNICODE_BLOCK_BATAK;
        } else if(code <= 0x1cff) {
            if(code <= 0x1c4f) return UNICODE_BLOCK_LEPCHA;
            else if(code <= 0x1c7f) return UNICODE_BLOCK_OL_CHIKI;
            else if(code <= 0x1ccf) return UNICODE_BLOCK_SUNDANESE_SUPPLEMENT;
            else return UNICODE_BLOCK_VEDIC_EXTENSIONS;
        } else if(code <= 0x1dff) {
            if(code <= 0x1d7f) return UNICODE_BLOCK_PHONETIC_EXTENSIONS;
            else if(code <= 0x1d8f) return UNICODE_BLOCK_PHONETIC_EXTENSIONS_SUPPLEMENT;
            else return UNICODE_BLOCK_COMBINING_DIACRITICAL_MARKS_SUPPLEMENT;
        } else if(code <= 0x1eff) {
            return UNICODE_BLOCK_LATIN_EXTENDED_ADDITIONAL;
        } else {
            return UNICODE_BLOCK_GREEK_EXTENDED;
        }
    } else if(code <= 0x2fff) {
        if(code <= 0x20ff) {
            if(code <= 0x206f) return UNICODE_BLOCK_GENERAL_PUNCTUATION;
            else if(code <= 0x209f) return UNICODE_BLOCK_SUPERSCRIPTS_AND_SUBSCRIPTS;
            else if(code <= 0x20cf) return UNICODE_BLOCK_CURRENCY_SYMBOLS;
            else return UNICODE_BLOCK_COMBINING_DIACRITICAL_MARKS_FOR_SYMBOLS;
        } else if(code <= 0x21ff) {
            if(code <= 0x214f) return UNICODE_BLOCK_LETTERLIKE_SYMBOLS;
            else if(code <= 0x218f) return UNICODE_BLOCK_NUMBER_FORMS;
            else return UNICODE_BLOCK_ARROWS;
        } else if(code <= 0x22ff) {
            return UNICODE_BLOCK_MATHEMATICAL_OPERATORS;
        } else if(code <= 0x23ff) {
            return UNICODE_BLOCK_MISCELLANEOUS_TECHNICAL;
        } else if(code <= 0x24ff) {
            if(code <= 0x243f) return UNICODE_BLOCK_CONTROL_PICTURES;
            else if(code <= 0x245f) return UNICODE_BLOCK_OPTICAL_CHARACTER_RECOGNITION;
            else return UNICODE_BLOCK_ENCLOSED_ALPHANUMERICS;
        } else if(code <= 0x25ff) {
            if(code <= 0x257f) return UNICODE_BLOCK_BOX_DRAWING;
            else if(code <= 0x259f) return UNICODE_BLOCK_BLOCK_ELEMENTS;
            else return UNICODE_BLOCK_GEOMETRIC_SHAPES;
        } else if(code <= 0x26ff) {
            return UNICODE_BLOCK_MISCELLANEOUS_SYMBOLS;
        } else if(code <= 0x27ff) {
            if(code <= 0x27bf) return UNICODE_BLOCK_DINGBATS;
            else if(code <= 0x27ef) return UNICODE_BLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_A;
            else return UNICODE_BLOCK_SUPPLEMENTAL_ARROWS_A;
        } else if(code <= 0x28ff) {
            return UNICODE_BLOCK_BRAILLE_PATTERNS;
        } else if(code <= 0x29ff) {
            if(code <= 0x297f) return UNICODE_BLOCK_SUPPLEMENTAL_ARROWS_B;
            else return UNICODE_BLOCK_MISCELLANEOUS_MATHEMATICAL_SYMBOLS_B;
        } else if(code <= 0x2aff) {
            return UNICODE_BLOCK_SUPPLEMENTAL_MATHEMATICAL_OPERATORS;
        } else if(code <= 0x2bff) {
            return UNICODE_BLOCK_MISCELLANEOUS_SYMBOLS_AND_ARROWS;
        } else if(code <= 0x2cff) {
            if(code <= 0x2c5f) return UNICODE_BLOCK_GLAGOLITIC;
            else return UNICODE_BLOCK_COPTIC;
        } else if(code <= 0x2dff) {
            if(code <= 0x2d2f) return UNICODE_BLOCK_GEORGIAN_SUPPLEMENT;
            else if(code <= 0x2d7f) return UNICODE_BLOCK_TIFINAGH;
            else if(code <= 0x2ddf) return UNICODE_BLOCK_ETHIOPIC_EXTENDED;
            else return UNICODE_BLOCK_CYRILLIC_EXTENDED_A;
        } else if(code <= 0x2eff) {
            if(code <= 0x2e7f) return UNICODE_BLOCK_SUPPLEMENTAL_PUNCTUATION;
            else return UNICODE_BLOCK_CJK_RADICALS_SUPPLEMENT;
        } else {
            if(code <= 0x2fdf) return UNICODE_BLOCK_KANGXI_RADICALS;
            else return UNICODE_BLOCK_IDEOGRAPHIC_DESCRIPTION_CHARACTERS;
        }
    } else if(code <= 0x9fff) {
        if(code <= 0x30ff) {
            if(code <= 0x303f) return UNICODE_BLOCK_CJK_SYMBOLS_AND_PUNCTUATION;
            else if(code <= 0x309f) return UNICODE_BLOCK_HIRAGANA;
            else return UNICODE_BLOCK_KATAKANA;
        } else if(code <= 0x31ff) {
            if(code <= 0x312f) return UNICODE_BLOCK_BOPOMOFO;
            else if(code <= 0x318f) return UNICODE_BLOCK_HANGUL_COMPATIBILITY_JAMO;
            else if(code <= 0x319f) return UNICODE_BLOCK_KANBUN;
            else if(code <= 0x31bf) return UNICODE_BLOCK_BOPOMOFO_EXTENDED;
            else if(code <= 0x31ef) return UNICODE_BLOCK_CJK_STROKES;
            else return UNICODE_BLOCK_KATAKANA_PHONETIC_EXTENSIONS;
        } else if(code <= 0x32ff) {
            return UNICODE_BLOCK_ENCLOSED_CJK_LETTERS_AND_MONTHS;
        } else if(code <= 0x33ff) {
            return UNICODE_BLOCK_CJK_COMPATIBILITY;
        } else if(code <= 0x4dff) {
            if(code <= 0x4dbf) return UNICODE_BLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_A;
            else return UNICODE_BLOCK_YIJING_HEXAGRAM_SYMBOLS;
        } else {
            return UNICODE_BLOCK_CJK_UNIFIED_IDEOGRAPHS;
        }
    } else if(code <= 0xdfff) {
        if(code <= 0xa4ff) {
            if(code <= 0xa48f) return UNICODE_BLOCK_YI_SYLLABLES;
            else if(code <= 0xa4cf) return UNICODE_BLOCK_YI_RADICALS;
            else return UNICODE_BLOCK_LISU;
        } else if(code <= 0xa6ff) {
            if(code <= 0xa63f) return UNICODE_BLOCK_VAI;
            else if(code <= 0xa69f) return UNICODE_BLOCK_CYRILLIC_EXTENDED_B;
            else return UNICODE_BLOCK_BAMUM;
        } else if(code <= 0xa7ff) {
            if(code <= 0xa71f) return UNICODE_BLOCK_MODIFIER_TONE_LETTERS;
            else return UNICODE_BLOCK_LATIN_EXTENDED_B;
        } else if(code <= 0xa8ff) {
            if(code <= 0xa82f) return UNICODE_BLOCK_SYLOTI_NAGRI;
            else if(code <= 0xa83f) return UNICODE_BLOCK_COMMON_INDIC_NUMBER_FORMS;
            else if(code <= 0xa87f) return UNICODE_BLOCK_PHAGS_PA;
            else if(code <= 0xa8df) return UNICODE_BLOCK_SAURASHTRA;
            else return UNICODE_BLOCK_DEVANAGARI_EXTENDED;
        } else if(code <= 0xaaff) {
            if(code <= 0xa92f) return UNICODE_BLOCK_KAYAH_LI;
            else if(code <= 0xa95f) return UNICODE_BLOCK_REJANG;
            else if(code <= 0xa97f) return UNICODE_BLOCK_HANGUL_JAMO_EXTENDED_A;
            else if(code <= 0xa9df) return UNICODE_BLOCK_JAVANESE;
            else if(code <= 0xaa5f) return UNICODE_BLOCK_CHAM;
            else if(code <= 0xaa7f) return UNICODE_BLOCK_MYANMAR_EXTENDED_A;
            else if(code <= 0xaadf) return UNICODE_BLOCK_TAI_VIET;
            else return UNICODE_BLOCK_MEETEI_MAYEK_EXTENSIONS;
        } else if(code <= 0xabff) {
            if(code <= 0xab2f) return UNICODE_BLOCK_ETHIOPIC_EXTENDED_A;
            else return UNICODE_BLOCK_MEETEI_MAYEK;
        } else if(code <= 0xd7ff) {
            if(code <= 0xd7af) return UNICODE_BLOCK_HANGUL_SYLLABLES;
            else return UNICODE_BLOCK_HANGUL_JAMO_EXTENDED_B;
        } else if(code <= 0xdbff) {
            if(code <= 0xdb7f) return UNICODE_BLOCK_HIGH_SURROGATES;
            else return UNICODE_BLOCK_HIGH_PRIVATE_USE_SURROGATES;
        } else {
            return UNICODE_BLOCK_LOW_SURROGATES;
        }
    } else if(code <= 0xffff) {
        if(code <= 0xf8ff) {
            return UNICODE_BLOCK_PRIVATE_USE_AREA;
        } else if(code <= 0xfaff) {
            return UNICODE_BLOCK_CJK_COMPATIBILITY_IDEOGRAPHS;
        } else if(code <= 0xfdff) {
            if(code <= 0xfb4f) return UNICODE_BLOCK_ALPHABETIC_PRESENTATION_FORMS;
            else return UNICODE_BLOCK_ARABIC_PRESENTATION_FORMS_A;
        } else if(code <= 0xfeff) {
            if(code <= 0xfe0f) return UNICODE_BLOCK_VARIATION_SELECTORS;
            if(code <= 0xfe1f) return UNICODE_BLOCK_VERTICAL_FORMS;
            if(code <= 0xfe2f) return UNICODE_BLOCK_COMBINING_HALF_MARKS;
            if(code <= 0xfe4f) return UNICODE_BLOCK_CJK_COMPATIBILITY_FORMS;
            if(code <= 0xfe6f) return UNICODE_BLOCK_SMALL_FORM_VARIANTS;
            else return UNICODE_BLOCK_ARABIC_PRESENTATION_FORMS_B;
        } else {
            if(code <= 0xffef) return UNICODE_BLOCK_HALFWIDTH_AND_FULLWIDTH_FORMS;
            else return UNICODE_BLOCK_SPECIALS;
        }
    } else if(code <= 0xfffff) {
        if(code <= 0x100ff) {
            if(code <= 0x1007f) return UNICODE_BLOCK_LINEAR_B_SYLLABARY;
            else return UNICODE_BLOCK_LINEAR_B_IDEOGRAMS;
        } else if(code <= 0x101ff) {
            if(code <= 0x1013f) return UNICODE_BLOCK_AEGEAN_NUMBERS;
            else if(code <= 0x1018f) return UNICODE_BLOCK_ANCIENT_GREEK_NUMBERS;
            else if(code <= 0x101cf) return UNICODE_BLOCK_ANCIENT_SYMBOLS;
            else return UNICODE_BLOCK_PHAISTOS_DISC;
        } else if(code <= 0x109ff) {
            if(code <= 0x1029f) return UNICODE_BLOCK_LYCIAN;
            else if(code <= 0x102df) return UNICODE_BLOCK_CARIAN;
            else if(code <= 0x1032f) return UNICODE_BLOCK_OLD_ITALIC;
            else if(code <= 0x1034f) return UNICODE_BLOCK_GOTHIC;
            else if(code <= 0x1039f) return UNICODE_BLOCK_UGARITIC;
            else if(code <= 0x103df) return UNICODE_BLOCK_OLD_PERSIAN;
            else if(code <= 0x1044f) return UNICODE_BLOCK_DESERET;
            else if(code <= 0x1047f) return UNICODE_BLOCK_SHAVIAN;
            else if(code <= 0x104af) return UNICODE_BLOCK_OSMANYA;
            else if(code <= 0x1083f) return UNICODE_BLOCK_CYPRIOT_SYLLABARY;
            else if(code <= 0x1085f) return UNICODE_BLOCK_IMPERIAL_ARAMAIC;
            else if(code <= 0x1091f) return UNICODE_BLOCK_PHOENICIAN;
            else if(code <= 0x1093f) return UNICODE_BLOCK_LYDIAN;
            else if(code <= 0x1099f) return UNICODE_BLOCK_MEROITIC_HIEROGLYPHS;
            else return UNICODE_BLOCK_MEROITIC_CURSIVE;
        } else if(code <= 0x110ff) {
            if(code <= 0x10a5f) return UNICODE_BLOCK_KHAROSHTHI;
            else if(code <= 0x10a7f) return UNICODE_BLOCK_OLD_SOUTH_ARABIAN;
            else if(code <= 0x10b3f) return UNICODE_BLOCK_AVESTAN;
            else if(code <= 0x10b5f) return UNICODE_BLOCK_INSCRIPTIONAL_PARTHIAN;
            else if(code <= 0x10b7f) return UNICODE_BLOCK_INSCRIPTIONAL_PAHLAVI;
            else if(code <= 0x10c4f) return UNICODE_BLOCK_OLD_TURKIC;
            else if(code <= 0x10e7f) return UNICODE_BLOCK_RUMI_NUMERAL_SYMBOLS;
            else if(code <= 0x1107f) return UNICODE_BLOCK_BRAHMI;
            else if(code <= 0x110cf) return UNICODE_BLOCK_KAITHI;
            else return UNICODE_BLOCK_SORA_SOMPENG;
        } else if(code <= 0x123ff) {
            if(code <= 0x1114f) return UNICODE_BLOCK_CHAKMA;
            else if(code <= 0x111df) return UNICODE_BLOCK_SHARADA;
            else if(code <= 0x116cf) return UNICODE_BLOCK_TAKRI;
            else return UNICODE_BLOCK_CUNEIFORM;
        } else if(code <= 0x1b0ff) {
            if(code <= 0x1247f) return UNICODE_BLOCK_CUNEIFORM_NUMBERS_AND_PUNCTUATION;
            else if(code <= 0x1342f) return UNICODE_BLOCK_EGYPTIAN_HIEROGLYPHS;
            else if(code <= 0x16a3f) return UNICODE_BLOCK_BAMUM_SUPPLEMENT;
            else if(code <= 0x16f9f) return UNICODE_BLOCK_MIAO;
            else return UNICODE_BLOCK_KANA_SUPPLEMENT;
        } else if(code <= 0x1d0ff) {
            return UNICODE_BLOCK_BYZANTINE_MUSICAL_SYMBOLS;
        } else if(code <= 0x1d1ff) {
            return UNICODE_BLOCK_MUSICAL_SYMBOLS;
        } else if(code <= 0x1d7ff) {
            if(code <= 0x1d24f) return UNICODE_BLOCK_ANCIENT_GREEK_MUSICAL_NOTATION;
            else if(code <= 0x1d35f) return UNICODE_BLOCK_TAI_XUAN_JING_SYMBOLS;
            else if(code <= 0x1d37f) return UNICODE_BLOCK_COUNTING_ROD_NUMERALS;
            else return UNICODE_BLOCK_MATHEMATICAL_ALPHANUMERIC_SYMBOLS;
        } else if(code <= 0x1eeff) {
            return UNICODE_BLOCK_ARABIC_MATHEMATICAL_ALPHABETIC_SYMBOLS;
        } else if(code <= 0x1f0ff) {
            if(code <= 0x1f02f) return UNICODE_BLOCK_MAHJONG_TILES;
            if(code <= 0x1f09f) return UNICODE_BLOCK_DOMINO_TILES;
            else return UNICODE_BLOCK_PLAYING_CARDS;
        } else if(code <= 0x1f1ff) {
            return UNICODE_BLOCK_ENCLOSED_ALPHANUMERIC_SUPPLEMENT;
        } else if(code <= 0x1f2ff) {
            return UNICODE_BLOCK_ENCLOSED_IDEOGRAPHIC_SUPPLEMENT;
        } else if(code <= 0x1f5ff) {
            return UNICODE_BLOCK_MISCELLANEOUS_SYMBOLS_AND_PICTOGRAPHS;
        } else if(code <= 0x1f6ff) {
            if(code <= 0x1f64f) return UNICODE_BLOCK_EMOTICONS;
            else return UNICODE_BLOCK_TRANSPORT_AND_MAP_SYMBOLS;
        } else {
            if(code <= 0x1f77f) return UNICODE_BLOCK_ALCHEMICAL_SYMBOLS;
            else if(code <= 0x2a6df) return UNICODE_BLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_B;
            else if(code <= 0x2b73f) return UNICODE_BLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_C;
            else if(code <= 0x2b81f) return UNICODE_BLOCK_CJK_UNIFIED_IDEOGRAPHS_EXTENSION_D;
            else if(code <= 0x2fa1f) return UNICODE_BLOCK_CJK_COMPATIBILITY_IDEOGRAPHS_SUPPLEMENT;
            else if(code <= 0xe007f) return UNICODE_BLOCK_TAGS;
            else if(code <= 0xe01ef) return UNICODE_BLOCK_VARIATION_SELECTORS_SUPPLEMENT;
            else return UNICODE_BLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_A;
        }
    } else {
        return UNICODE_BLOCK_SUPPLEMENTARY_PRIVATE_USE_AREA_B;
    }
}

    bool UnicodeData::isupper(uint32_t const &code) {
        return uppercases_.find(code) != uppercases_.end();
    }
}
