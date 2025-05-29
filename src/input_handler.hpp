#pragma once

#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_types.h"
#include "SDL2/SDL_version.h"
#include "SDL2/SDL_hints.h"
#include "SDL2/SDL_events.h"
#include <SDL2/SDL_stdinc.h>

class Keyboard
{
	public:

		// Default Constructor
		Keyboard()
		{
			//newkey = newmouse = false;
			//keydown = mousedown = false;

			SDL_ShowCursor(SDL_FALSE);

			#if SDL_VERSION_ATLEAST(2, 26, 0)
				SDL_SetHint(SDL_HINT_MOUSE_RELATIVE_SYSTEM_SCALE, "1");
			#endif
		}

};

class Joystick
{

};

class Mouse
{
	private:

		SDL_bool windowHasFocus;
		
		bool mousedown;
		Uint8 most_recent_mouse_button;
		
		Sint32 mouse_position_x;
		Sint32 mouse_position_y;

		static Sint32 relative_mouse_position_x;
		static Sint32 relative_mouse_position_y;
		static bool relative_mode_enabled;

	public:

		// Default Constructor
		Mouse()
		{ }

		void set_relative_mode(SDL_bool enable)
		{
			SDL_SetRelativeMouseMode(enable /* && windowHasFocus */);

			this->relative_mode_enabled = enable;

			this->relative_mouse_position_x = 0;
			this->relative_mouse_position_y = 0;
		}

		unsigned short mousePosition(unsigned short *mouse_x_position, unsigned short *mouse_y_position)
		{
			// service_SDL_events(false);
			*mouse_x_position = this->mouse_position_x;
			*mouse_y_position = this->mouse_position_y;

			return this->mousedown ? this->most_recent_mouse_button : 0;
		}

		void get_relative_mouse_position(Sint32 *const out_x, Sint32 *const out_y)
		{
			// service_SDL_events(false);

			// scaleWindowDistanceToScreen(&mouseWindowXRelative, &mouseWindowYRelative);
			*out_x = this->relative_mouse_position_x;
			*out_y = this->relative_mouse_position_y;

			this->relative_mouse_position_x = 0;
			this->relative_mouse_position_y = 0;
		}
};

class Input_Handler
{
	public:

		// Default Constructor
		Input_Handler()
		{ }

		void flush_events_buffer(void)
		{
			SDL_Event sdl_event;
			while (SDL_PollEvent(&sdl_event));
		}

		// void wait_input(bool keyboard, bool mouse, bool joystick)
		// {
		// 	service_SDL_events(false);
		// 	while (!((keyboard && keydown) || (mouse && mousedown) || (joystick && joydown)))
		// 	{
		// 		SDL_Delay(SDL_POLL_INTERVAL);
		// 		push_joysticks_as_keyboard();
		// 		service_SDL_events(false);

		// #ifdef WITH_NETWORK
		// 		if (isNetworkGame)
		// 		{ network_check(); }
					
		// #endif
		// 	}
		// }
};