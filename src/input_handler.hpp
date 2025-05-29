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

		bool new_key{false};
		bool keydown{false};

		// Default Constructor
		Keyboard()
		{
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


	public:

		bool mouse_down{false};
		bool new_mouse{false};
		Uint8 most_recent_mouse_button;
		
		Sint32 mouse_position_x;
		Sint32 mouse_position_y;

		Sint32 relative_mouse_position_x;
		Sint32 relative_mouse_position_y;
		bool relative_mode_enabled;


		// Default Constructor
		Mouse()
		{ }

		void relative_mode_enable()
		{ this->set_relative_mode(true); }

		void relative_mode_disable()
		{ this->set_relative_mode(false); }

		void set_relative_mode(bool enable)
		{
			// Stupid hacky "casting"
			SDL_bool sdl_enable = enable ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE;
			SDL_SetRelativeMouseMode(sdl_enable);

			this->relative_mode_enabled = enable;

			this->relative_mouse_position_x = 0;
			this->relative_mouse_position_y = 0;
		}

		unsigned short mousePosition(unsigned short *mouse_x_position, unsigned short *mouse_y_position)
		{
			// service_SDL_events(false);
			*mouse_x_position = this->mouse_position_x;
			*mouse_y_position = this->mouse_position_y;

			return this->mouse_down ? this->most_recent_mouse_button : 0;
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
	private:

		// Data Members

		Mouse mouse;
		Keyboard keyboard;
		Joystick joystick;

		bool new_text;
		bool window_has_focus;

		// Methods

		void service_SDL_events(bool clear_new)
		{
			SDL_Event sdl_event;

			if (clear_new)
			{
				this->keyboard.new_key = false;
				this->mouse.new_mouse = false;

				this->new_text = false;
			}

			while (SDL_PollEvent(&sdl_event))
			{
				switch (sdl_event.type)
				{
					case SDL_WINDOWEVENT:
						switch (sdl_event.window.event)
						{
							case SDL_WINDOWEVENT_FOCUS_LOST:
								this->window_has_focus = false;

								this->mouse.set_relative_mode(this->mouse.relative_mode_enabled);
								break;

							case SDL_WINDOWEVENT_FOCUS_GAINED:
								this->window_has_focus = true;

								mouseSetRelative(mouseRelativeEnabled);
								break;

							case SDL_WINDOWEVENT_RESIZED:
								video_on_win_resize();
								break;
							}
						break;

					case SDL_KEYDOWN:
						/* <alt><enter> toggle fullscreen */
						if (sdl_event.key.keysym.mod & KMOD_ALT && sdl_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
						{
							toggle_fullscreen();
							break;
						}

						keysactive[ev.key.keysym.scancode] = 1;

						newkey = true;
						lastkey_scan = ev.key.keysym.scancode;
						lastkey_mod = ev.key.keysym.mod;
						keydown = true;

						mouseInactive = true;
						return;

					case SDL_KEYUP:
						keysactive[ev.key.keysym.scancode] = 0;
						keydown = false;
						return;

					case SDL_MOUSEMOTION:
						mouse_x = ev.motion.x;
						mouse_y = ev.motion.y;
						mapWindowPointToScreen(&mouse_x, &mouse_y);

						if (mouseRelativeEnabled && windowHasFocus)
						{
							mouseWindowXRelative += ev.motion.xrel;
							mouseWindowYRelative += ev.motion.yrel;
						}

						// Show system mouse pointer if outside screen.
						SDL_ShowCursor(mouse_x < 0 || mouse_x >= vga_width ||
									mouse_y < 0 || mouse_y >= vga_height ? SDL_TRUE : SDL_FALSE);

						if (ev.motion.xrel != 0 || ev.motion.yrel != 0)
							mouseInactive = false;
						break;

					case SDL_MOUSEBUTTONDOWN:
						mouseInactive = false;

						// fall through
					case SDL_MOUSEBUTTONUP:
						mapWindowPointToScreen(&sdl_event.button.x, &sdl_event.button.y);
						if (sdl_event.type == SDL_MOUSEBUTTONDOWN)
						{
							newmouse = true;
							most_recent_mouse_button = ev.button.button;
							lastmouse_x = ev.button.x;
							lastmouse_y = ev.button.y;
							mousedown = true;
						}
						else
						{
							mousedown = false;
						}

						int whichMB = -1;
						switch (sdl_event.button.button)
						{
							case SDL_BUTTON_LEFT:   whichMB = 0; break;
							case SDL_BUTTON_RIGHT:  whichMB = 1; break;
							case SDL_BUTTON_MIDDLE: whichMB = 2; break;
						}
						if (whichMB < 0)
							break;

						switch (mouseSettings[whichMB])
						{
							case 1: // Fire Main Weapons
								mouse_pressed[0] = mousedown;
								break;
							case 2: // Fire Left Sidekick
								mouse_pressed[1] = mousedown;
								break;
							case 3: // Fire Right Sidekick
								mouse_pressed[2] = mousedown;
								break;
							case 4: // Fire BOTH Sidekicks
								mouse_pressed[1] = mousedown;
								mouse_pressed[2] = mousedown;
								break;
							case 5: // Change Rear Mode
								mouse_pressed[3] = mousedown;
								break;
						}
						break;

					case SDL_TEXTINPUT:
						SDL_strlcpy(last_text, ev.text.text, COUNTOF(last_text));
						new_text = true;
						break;

					case SDL_TEXTEDITING:
						break;

					case SDL_QUIT:
						/* TODO: Call the cleanup code here. */
						exit(0);
						break;
				}
			}
		}

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