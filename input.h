//Copyright 2014 by Pavel Chistyakov
//
//   Licensed under the Apache License, Version 2.0 (the "License");
//   you may not use this file except in compliance with the License.
//   You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
//   Unless required by applicable law or agreed to in writing, software
//   distributed under the License is distributed on an "AS IS" BASIS,
//   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//   See the License for the specific language governing permissions and
//   limitations under the License.

#include "point.h"

#pragma once

enum events {
	// input events
	ControlLocal = 0xD000,
	InputSymbol = 0xE000, InputTimer, InputEdit, InputEditPlus, InputEditMinus, InputExit, InputResize, InputIdle,
	InputChoose, InputDropDown, InputMenu, InputSet,
	ScrollV, ScrollH, SplitterV, SplitterH,
	TreeType, TreeFlags,
	HtmlLink, HtmlControl, TabsControl, TabsCloseControl,
	Executed,
	// control keys
	MouseLeft = 0xE800, MouseLeftDBL, MouseRight,
	MouseMove, MouseWheelUp, MouseWheelDown,
	KeyLeft, KeyRight, KeyUp, KeyDown, KeyPageUp, KeyPageDown, KeyHome, KeyEnd,
	KeyBackspace, KeyEnter, KeyDelete, KeyEscape, KeySpace, KeyTab,
	F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
	// named keys range
	Alpha,
	FirstKey = MouseLeft,
	FirstMouse = MouseLeft, LastMouse = MouseWheelDown,
	// support
	CommandMask 	= 0x0000FFFF,
	// misc events can be combination with previous
	Ctrl			= 0x00010000,
	Alt				= 0x00020000,
	Shift			= 0x00040000,
	// control visual flags
    HideActiveBorder= 0x00100000,
	HideBackground  = 0x00200000,
	HideBorder		= 0x00400000,
	// command flags (common for all)
    ShowText		= 0x01000000,
	NoHandleTab		= 0x02000000,
	NoFocusing		= 0x04000000,
    // state flags
	Focused			= 0x10000000, // Control has keyboard input and can change visual form.
	Checked 		= 0x20000000, // Use in background virtual method.
	Disabled		= 0x40000000, // Control not grant any input.
	FirstInput = InputSymbol,
};
enum areas {
	AreaNormal, // Area not have mouse
	AreaHilited, // Area have mouse
	AreaHilitedPressed, // Area have mouse and mouse button is pressed
};
namespace hot
{
	void						clear();
	extern int					key; // [in] if pressed key or mouse this field has key
	extern point				mouse; // current mouse coordinates
	extern bool					pressed; // flag if any of mouse keys is pressed
	extern int					param; // command or input event parameter
}