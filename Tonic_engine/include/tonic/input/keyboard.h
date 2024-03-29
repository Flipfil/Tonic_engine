#pragma once

#include <array>

namespace tonic::input
{
	class Keyboard
	{
	public:
		static void Initialize();
		static void Update();

        static bool PressedAnyKey();
		static bool HoldingKey(int button);
        static bool PressedKey(int button);
        static bool ReleasedKey(int button);

	private:

        constexpr static const int m_key_count = 287; // SDL supports up to index 286

		static std::array<bool, m_key_count> keys_down;
        static std::array<bool, m_key_count> keys_down_last;
	};
}

enum {
    TONIC_KEY_FIRST              = 4,
    TONIC_KEY_A                  = TONIC_KEY_FIRST,
    TONIC_KEY_B                  = 5,
    TONIC_KEY_C                  = 6,
    TONIC_KEY_D                  = 7,
    TONIC_KEY_E                  = 8,
    TONIC_KEY_F                  = 9,
    TONIC_KEY_G                  = 10,
    TONIC_KEY_H                  = 11,
    TONIC_KEY_I                  = 12,
    TONIC_KEY_J                  = 13,
    TONIC_KEY_K                  = 14,
    TONIC_KEY_L                  = 15,
    TONIC_KEY_M                  = 16,
    TONIC_KEY_N                  = 17,
    TONIC_KEY_O                  = 18,
    TONIC_KEY_P                  = 19,
    TONIC_KEY_Q                  = 20,
    TONIC_KEY_R                  = 21,
    TONIC_KEY_S                  = 22,
    TONIC_KEY_T                  = 23,
    TONIC_KEY_U                  = 24,
    TONIC_KEY_V                  = 25,
    TONIC_KEY_W                  = 26,
    TONIC_KEY_X                  = 27,
    TONIC_KEY_Y                  = 28,
    TONIC_KEY_Z                  = 29,
    TONIC_KEY_1                  = 30,
    TONIC_KEY_2                  = 31,
    TONIC_KEY_3                  = 32,
    TONIC_KEY_4                  = 33,
    TONIC_KEY_5                  = 34,
    TONIC_KEY_6                  = 35,
    TONIC_KEY_7                  = 36,
    TONIC_KEY_8                  = 37,
    TONIC_KEY_9                  = 38,
    TONIC_KEY_0                  = 39,
    TONIC_KEY_RETURN             = 40,
    TONIC_KEY_ESCAPE             = 41,
    TONIC_KEY_BACKSPACE          = 42,
    TONIC_KEY_TAB                = 43,
    TONIC_KEY_SPACE              = 44,
    TONIC_KEY_MINUS              = 45,
    TONIC_KEY_EQUALS             = 46,
    TONIC_KEY_LEFTBRACKET        = 47,
    TONIC_KEY_RIGHTBRACKET       = 48,
    TONIC_KEY_BACKSLASH          = 49,
    TONIC_KEY_NONUSHASH          = 50,
    TONIC_KEY_SEMICOLON          = 51,
    TONIC_KEY_APOSTROPHE         = 52,
    TONIC_KEY_GRAVE              = 53,
    TONIC_KEY_COMMA              = 54,
    TONIC_KEY_PERIOD             = 55,
    TONIC_KEY_SLASH              = 56,
    TONIC_KEY_CAPSLOCK           = 57,
    TONIC_KEY_F1                 = 58,
    TONIC_KEY_F2                 = 59,
    TONIC_KEY_F3                 = 60,
    TONIC_KEY_F4                 = 61,
    TONIC_KEY_F5                 = 62,
    TONIC_KEY_F6                 = 63,
    TONIC_KEY_F7                 = 64,
    TONIC_KEY_F8                 = 65,
    TONIC_KEY_F9                 = 66,
    TONIC_KEY_F10                = 67,
    TONIC_KEY_F11                = 68,
    TONIC_KEY_F12                = 69,
    TONIC_KEY_PRINTSCREEN        = 70,
    TONIC_KEY_SCROLLLOCK         = 71,
    TONIC_KEY_PAUSE              = 72,
    TONIC_KEY_INSERT             = 73,
    TONIC_KEY_HOME               = 74,
    TONIC_KEY_PAGEUP             = 75,
    TONIC_KEY_DELETE             = 76,
    TONIC_KEY_END                = 77,
    TONIC_KEY_PAGEDOWN           = 78,
    TONIC_KEY_RIGHT              = 79,
    TONIC_KEY_LEFT               = 80,
    TONIC_KEY_DOWN               = 81,
    TONIC_KEY_UP                 = 82,
    TONIC_KEY_NUMLOCKCLEAR       = 83,
    TONIC_KEY_KP_DIVIDE          = 84,
    TONIC_KEY_KP_MULTIPLY        = 85,
    TONIC_KEY_KP_MINUS           = 86,
    TONIC_KEY_KP_PLUS            = 87,
    TONIC_KEY_KP_ENTER           = 88,
    TONIC_KEY_KP_1               = 89,
    TONIC_KEY_KP_2               = 90,
    TONIC_KEY_KP_3               = 91,
    TONIC_KEY_KP_4               = 92,
    TONIC_KEY_KP_5               = 93,
    TONIC_KEY_KP_6               = 94,
    TONIC_KEY_KP_7               = 95,
    TONIC_KEY_KP_8               = 96,
    TONIC_KEY_KP_9               = 97,
    TONIC_KEY_KP_0               = 98,
    TONIC_KEY_KP_PERIOD          = 99,
    TONIC_KEY_NONUSBACKSLASH     = 100,
    TONIC_KEY_APPLICATION        = 101,
    TONIC_KEY_POWER              = 102,
    TONIC_KEY_KP_EQUALS          = 103,
    TONIC_KEY_F13                = 104,
    TONIC_KEY_F14                = 105,
    TONIC_KEY_F15                = 106,
    TONIC_KEY_F16                = 107,
    TONIC_KEY_F17                = 108,
    TONIC_KEY_F18                = 109,
    TONIC_KEY_F19                = 110,
    TONIC_KEY_F20                = 111,
    TONIC_KEY_F21                = 112,
    TONIC_KEY_F22                = 113,
    TONIC_KEY_F23                = 114,
    TONIC_KEY_F24                = 115,
    TONIC_KEY_EXECUTE            = 116,
    TONIC_KEY_HELP               = 117,
    TONIC_KEY_MENU               = 118,
    TONIC_KEY_SELECT             = 119,
    TONIC_KEY_STOP               = 120,
    TONIC_KEY_AGAIN              = 121,
    TONIC_KEY_UNDO               = 122,
    TONIC_KEY_CUT                = 123,
    TONIC_KEY_COPY               = 124,
    TONIC_KEY_PASTE              = 125,
    TONIC_KEY_FIND               = 126,
    TONIC_KEY_MUTE               = 127,
    TONIC_KEY_VOLUMEUP           = 128,
    TONIC_KEY_VOLUMEDOWN         = 129,
                              // 130 - 132 
    TONIC_KEY_KP_COMMA           = 133,
    TONIC_KEY_KP_EQUALSAS400     = 134,
    TONIC_KEY_INTERNATIONAL1     = 135,
    TONIC_KEY_INTERNATIONAL2     = 136,
    TONIC_KEY_INTERNATIONAL3     = 137,
    TONIC_KEY_INTERNATIONAL4     = 138,
    TONIC_KEY_INTERNATIONAL5     = 139,
    TONIC_KEY_INTERNATIONAL6     = 140,
    TONIC_KEY_INTERNATIONAL7     = 141,
    TONIC_KEY_INTERNATIONAL8     = 142,
    TONIC_KEY_INTERNATIONAL9     = 143,
    TONIC_KEY_LANG1              = 144,
    TONIC_KEY_LANG2              = 145,
    TONIC_KEY_LANG3              = 146,
    TONIC_KEY_LANG4              = 147,
    TONIC_KEY_LANG5              = 148,
    TONIC_KEY_LANG6              = 149,
    TONIC_KEY_LANG7              = 150,
    TONIC_KEY_LANG8              = 151,
    TONIC_KEY_LANG9              = 152,
    TONIC_KEY_ALTERASE           = 153,
    TONIC_KEY_SYSREQ             = 154,
    TONIC_KEY_CANCEL             = 155,
    TONIC_KEY_CLEAR              = 156,
    TONIC_KEY_PRIOR              = 157,
    TONIC_KEY_RETURN2            = 158,
    TONIC_KEY_SEPARATOR          = 159,
    TONIC_KEY_OUT                = 160,
    TONIC_KEY_OPER               = 161,
    TONIC_KEY_CLEARAGAIN         = 162,
    TONIC_KEY_CRSEL              = 163,
    TONIC_KEY_EXSEL              = 164,
                              // 165 - 175
    TONIC_KEY_KP_00              = 176,
    TONIC_KEY_KP_000             = 177,
    TONIC_KEY_THOUSANDSSEPARATOR = 178,
    TONIC_KEY_DECIMALSEPARATOR   = 179,
    TONIC_KEY_CURRENCYUNIT       = 180,
    TONIC_KEY_CURRENCYSUBUNIT    = 181,
    TONIC_KEY_KP_LEFTPAREN       = 182,
    TONIC_KEY_KP_RIGHTPAREN      = 183,
    TONIC_KEY_KP_LEFTBRACE       = 184,
    TONIC_KEY_KP_RIGHTBRACE      = 185,
    TONIC_KEY_KP_TAB             = 186,
    TONIC_KEY_KP_BACKSPACE       = 187,
    TONIC_KEY_KP_A               = 188,
    TONIC_KEY_KP_B               = 189,
    TONIC_KEY_KP_C               = 190,
    TONIC_KEY_KP_D               = 191,
    TONIC_KEY_KP_E               = 192,
    TONIC_KEY_KP_F               = 193,
    TONIC_KEY_KP_XOR             = 194,
    TONIC_KEY_KP_POWER           = 195,
    TONIC_KEY_KP_PERCENT         = 196,
    TONIC_KEY_KP_LESS            = 197,
    TONIC_KEY_KP_GREATER         = 198,
    TONIC_KEY_KP_AMPERSAND       = 199,
    TONIC_KEY_KP_DBLAMPERSAND    = 200,
    TONIC_KEY_KP_VERTICALBAR     = 201,
    TONIC_KEY_KP_DBLVERTICALBAR  = 202,
    TONIC_KEY_KP_COLON           = 203,
    TONIC_KEY_KP_HASH            = 204,
    TONIC_KEY_KP_SPACE           = 205,
    TONIC_KEY_KP_AT              = 206,
    TONIC_KEY_KP_EXCLAM          = 207,
    TONIC_KEY_KP_MEMSTORE        = 208,
    TONIC_KEY_KP_MEMRECALL       = 209,
    TONIC_KEY_KP_MEMCLEAR        = 210,
    TONIC_KEY_KP_MEMADD          = 211,
    TONIC_KEY_KP_MEMSUBTRACT     = 212,
    TONIC_KEY_KP_MEMMULTIPLY     = 213,
    TONIC_KEY_KP_MEMDIVIDE       = 214,
    TONIC_KEY_KP_PLUSMINUS       = 215,
    TONIC_KEY_KP_CLEAR           = 216,
    TONIC_KEY_KP_CLEARENTRY      = 217,
    TONIC_KEY_KP_BINARY          = 218,
    TONIC_KEY_KP_OCTAL           = 219,
    TONIC_KEY_KP_DECIMAL         = 220,
    TONIC_KEY_KP_HEXADECIMAL     = 221,
                              // 222 - 223 
    TONIC_KEY_LCTRL              = 224,
    TONIC_KEY_LSHIFT             = 225,
    TONIC_KEY_LALT               = 226,
    TONIC_KEY_LGUI               = 227,
    TONIC_KEY_RCTRL              = 228,
    TONIC_KEY_RSHIFT             = 229,
    TONIC_KEY_RALT               = 230,
    TONIC_KEY_RGUI               = 231,
    TONIC_KEY_MODE               = 257,
    TONIC_KEY_AUDIONEXT          = 258,
    TONIC_KEY_AUDIOPREV          = 259,
    TONIC_KEY_AUDIOSTOP          = 260,
    TONIC_KEY_AUDIOPLAY          = 261,
    TONIC_KEY_AUDIOMUTE          = 262,
    TONIC_KEY_MEDIASELECT        = 263,
    TONIC_KEY_WWW                = 264,
    TONIC_KEY_MAIL               = 265,
    TONIC_KEY_CALCULATOR         = 266,
    TONIC_KEY_COMPUTER           = 267,
    TONIC_KEY_AC_SEARCH          = 268,
    TONIC_KEY_AC_HOME            = 269,
    TONIC_KEY_AC_BACK            = 270,
    TONIC_KEY_AC_FORWARD         = 271,
    TONIC_KEY_AC_STOP            = 272,
    TONIC_KEY_AC_REFRESH         = 273,
    TONIC_KEY_AC_BOOKMARKS       = 274,
    TONIC_KEY_BRIGHTNESSDOWN     = 275,
    TONIC_KEY_BRIGHTNESSUP       = 276,
    TONIC_KEY_DISPLAYSWITCH      = 277,
    TONIC_KEY_KBDILLUMTOGGLE     = 278,
    TONIC_KEY_KBDILLUMDOWN       = 279,
    TONIC_KEY_KBDILLUMUP         = 280,
    TONIC_KEY_EJECT              = 281,
    TONIC_KEY_SLEEP              = 282,
    TONIC_KEY_APP1               = 283,
    TONIC_KEY_APP2               = 284,
    TONIC_KEY_AUDIOREWIND        = 285,
    TONIC_KEY_AUDIOFASTFORWARD   = 286,
    TONIC_KEY_LAST               = TONIC_KEY_AUDIOFASTFORWARD,
};