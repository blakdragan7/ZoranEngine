#pragma once

enum KeyEventType {
	KeyEventType_Key_Down,
	KeyEventType_Key_Up
};

enum MouseEventType {
	MouseEventType_L_Down,
	MouseEventType_R_Down,
	MouseEventType_L_Up,
	MouseEventType_R_Up,
	MouseEventType_M_Down,
	MouseEventType_M_Up,
	MouseEventType_Scroll_Down,
	MouseEventType_Scroll_Up
};

// Global Keyboad Values
constexpr unsigned Key_A = 'A';
constexpr unsigned Key_B = 'B';
constexpr unsigned Key_C = 'C';
constexpr unsigned Key_D = 'D';
constexpr unsigned Key_E = 'E';
constexpr unsigned Key_F = 'F';
constexpr unsigned Key_G = 'G';
constexpr unsigned Key_H = 'H';
constexpr unsigned Key_I = 'I';
constexpr unsigned Key_J = 'J';
constexpr unsigned Key_K = 'K';
constexpr unsigned Key_L = 'L';
constexpr unsigned Key_M = 'M';
constexpr unsigned Key_N = 'N';
constexpr unsigned Key_O = 'O';
constexpr unsigned Key_P = 'P';
constexpr unsigned Key_Q = 'Q';
constexpr unsigned Key_R = 'R';
constexpr unsigned Key_S = 'S';
constexpr unsigned Key_T = 'T';
constexpr unsigned Key_U = 'U';
constexpr unsigned Key_V = 'V';
constexpr unsigned Key_W = 'W';
constexpr unsigned Key_X = 'X';
constexpr unsigned Key_Y = 'Y';
constexpr unsigned Key_Z = 'Z';

constexpr unsigned Key_1 = '1';
constexpr unsigned Key_2 = '2';
constexpr unsigned Key_3 = '3';
constexpr unsigned Key_4 = '4';
constexpr unsigned Key_5 = '5';
constexpr unsigned Key_6 = '6';
constexpr unsigned Key_7 = '7';
constexpr unsigned Key_8 = '8';
constexpr unsigned Key_9 = '9';
	 
constexpr unsigned Key_F1 = 1000;
constexpr unsigned Key_F2 = 1001;
constexpr unsigned Key_F3 = 1002;
constexpr unsigned Key_F4 = 1003;
constexpr unsigned Key_F5 = 1004;
constexpr unsigned Key_F6 = 1005;
constexpr unsigned Key_F7 = 1006;
constexpr unsigned Key_F8 = 1007;
constexpr unsigned Key_F9 = 1008;
constexpr unsigned Key_F10 = 1009;
constexpr unsigned Key_F11 = 1010;
constexpr unsigned Key_F12 = 1011;
	 
constexpr unsigned Key_Up_Arrow = 0x26;
constexpr unsigned Key_Down_Arrow = 0x28;
constexpr unsigned Key_Right_Arrow = 0x27;
constexpr unsigned Key_Left_Arrow = 0x25;

constexpr unsigned Key_Space = 0x20;
constexpr unsigned Key_Esc = 0x1B;
constexpr unsigned Key_Shift = 0x10;
constexpr unsigned Key_Tab = 0x09;
constexpr unsigned Key_Enter = 0x0D;
