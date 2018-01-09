#pragma once

//-----------------------------------------------------------------------------
// Engine Definitions
//-----------------------------------------------------------------------------
namespace mage {

	template< typename CallerT >
	inline CallerT *GetWindowCaller(HWND window, 
		                            UINT message, 
		                            [[maybe_unused]] WPARAM wParam, 
		                            [[maybe_unused]] LPARAM lParam) noexcept {
		
		if (WM_NCCREATE != message) {
			// Retrieves information about the specified window.
			// 1. A handle to the window and, indirectly, the class to which 
			// the window belongs.
			// 2. Retrieves the user data associated with the window.
			return reinterpret_cast< CallerT * >(
				GetWindowLongPtr(window, GWLP_USERDATA));
		}

		CallerT * const caller = reinterpret_cast< CallerT * >(
			reinterpret_cast< CREATESTRUCT * >(lParam)->lpCreateParams);

		// Changes an attribute of the specified window.
		// 1. A handle to the window and, indirectly, the class to which the 
		// window belongs.
		// 2. Sets the user data associated with the window.
		// 3. The replacement value.
		SetWindowLongPtr(window, 
			             GWLP_USERDATA, 
			             reinterpret_cast< LONG_PTR >(caller));

		return caller;
	}

	template< typename CallerT >
	inline CallerT *GetDialogCaller(HWND dialog, 
		                            UINT message, 
		                            [[maybe_unused]] WPARAM wParam, 
		                            [[maybe_unused]] LPARAM lParam) noexcept {
		
		if (WM_INITDIALOG != message) {
			// Retrieves information about the specified window.
			// 1. A handle to the window and, indirectly, the class to which  
			// the window belongs.
			// 2. Retrieves the user data associated with the window.
			return reinterpret_cast< CallerT * >(
				GetWindowLongPtr(dialog, GWLP_USERDATA));
		}

		CallerT * const caller = reinterpret_cast< CallerT * >(lParam);

		// Changes an attribute of the specified window.
		// 1. A handle to the window and, indirectly, the class to which the 
		// window belongs.
		// 2. Sets the user data associated with the window.
		// 3. The replacement value.
		SetWindowLongPtr(dialog, 
			             GWLP_USERDATA, 
			             reinterpret_cast< LONG_PTR >(caller));

		return caller;
	}
}