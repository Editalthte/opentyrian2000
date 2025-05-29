#pragma once

#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_types.h"
#include "SDL2/SDL_version.h"
#include "SDL2/SDL_hints.h"
#include "SDL2/SDL_events.h"
#include "SDL2/SDL_stdinc.h"
#include "SDL2/SDL_scancode.h"
#include "SDL2/SDL_keycode.h"
#include "SDL2/SDL_timer.h"

#include <map>
#include <string>

using std::string;

const int SDL_POLL_INTERVAL = 10;

class Keyboard
{
	public:

		// Data Members
		bool new_key{false};
		bool key_down{false};
		Uint8 active_keys[SDL_NUM_SCANCODES];
		SDL_Scancode last_key_scancode;
		Uint16 last_key_modifier;

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
	public:

		// Data Members
		bool joy_down;
};

class Mouse
{
	public:

		// Data Members
		bool mouse_down{false};
		bool new_mouse{false};
		bool relative_mode_setting; // Represents if the user *wants* this to occur at all
		Uint8 most_recent_mouse_button;
		
		Sint32 mouse_position_x;
		Sint32 mouse_position_y;

		Sint32 last_mouse_position_x;
		Sint32 last_mouse_position_y;

		Sint32 relative_mouse_position_x;
		Sint32 relative_mouse_position_y;

		bool inactive;

		


		struct Button
		{
			// Data Members
			int id;
			bool pressed;

			// Default Constructor
			Button(const int INIT_ID, const bool INIT_PRESSED = false)
			{
				this->id = INIT_ID;
				this->pressed = INIT_PRESSED;
			}

			// case 1: // Fire Main Weapons
			// mouse_pressed[0] = mousedown;
			// break;
			// case 2: // Fire Left Sidekick
			// mouse_pressed[1] = mousedown;
			// break;
			// case 3: // Fire Right Sidekick
			// mouse_pressed[2] = mousedown;
			// break;
			// case 4: // Fire BOTH Sidekicks
			// mouse_pressed[1] = mousedown;
			// mouse_pressed[2] = mousedown;
			// break;
			// case 5: // Change Rear Mode
			// mouse_pressed[3] = mousedown;
			// break;
		};

		std::map<int, Button> buttons
		{
			{0, Button(0)},
			{1, Button(1)},
			{2, Button(2)},
			{3, Button(3)}
		};

		// Default Constructor
		Mouse()
		{ }

		// This should probably be moved elsewhere, as this isn't really a concern of the mouse itself
		void set_sdl_relative_mode(bool WINDOW_HAS_FOCUS)
		{
			// Stupid hacky "casting"
			SDL_bool sdl_enable = (this->relative_mode_setting && WINDOW_HAS_FOCUS) ? SDL_bool::SDL_TRUE : SDL_bool::SDL_FALSE;
			SDL_SetRelativeMouseMode(sdl_enable);

			this->relative_mouse_position_x = 0;
			this->relative_mouse_position_y = 0;
		}


		// Methods

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

			// scaleWindowDistanceToScreen(&mouseWindowXRelative, &mouseWindowYRelative);  // video.h
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
		char last_text[SDL_TEXTINPUTEVENT_TEXT_SIZE];

		bool window_has_focus;

		// Methods
		void set_sdl_relative_mode()
		{ this->mouse.set_sdl_relative_mode(this->window_has_focus); }

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
					{
						switch (sdl_event.window.event)
						{
							case SDL_WINDOWEVENT_FOCUS_LOST:
							{
								this->window_has_focus = false;
								this->set_sdl_relative_mode();
								break;
							}

							case SDL_WINDOWEVENT_FOCUS_GAINED:
							{
								this->window_has_focus = true;
								this->set_sdl_relative_mode();
								break;
							}

							case SDL_WINDOWEVENT_RESIZED:
							{
								// video_on_win_resize(); // video.h
								break;
							}
						}

						break;
					}

					case SDL_KEYDOWN:
					{
						/* <alt><enter> toggle fullscreen */
						if (sdl_event.key.keysym.mod & KMOD_ALT && sdl_event.key.keysym.scancode == SDL_SCANCODE_RETURN)
						{
							// toggle_fullscreen(); // video.h
							break;
						}

						this->keyboard.active_keys[sdl_event.key.keysym.scancode] = 1;
						this->keyboard.new_key = true;

						this->keyboard.last_key_scancode = sdl_event.key.keysym.scancode;
						this->keyboard.last_key_modifier = sdl_event.key.keysym.mod;
						this->keyboard.key_down = true;

						this->mouse.inactive = true;

						return;
					}


					case SDL_KEYUP:
					{
						this->keyboard.active_keys[sdl_event.key.keysym.scancode] = 0;
						this->keyboard.key_down = false;

						return;
					}


					case SDL_MOUSEMOTION:
					{
						this->mouse.mouse_position_x = sdl_event.motion.x;
						this->mouse.mouse_position_y = sdl_event.motion.y;
						// mapWindowPointToScreen(&this->mouse.mouse_position_x, &this->mouse.mouse_position_y); // video.h

						if (this->mouse.relative_mode_setting && this->window_has_focus)
						{
							this->mouse.relative_mouse_position_x += sdl_event.motion.xrel;
							this->mouse.relative_mouse_position_y += sdl_event.motion.yrel;
						}

						// Show system mouse pointer if outside screen.
						const int VGA_WIDTH = 320;
						const int VGA_HEIGHT = 200;
						SDL_ShowCursor(
					this->mouse.mouse_position_x < 0 || 
							this->mouse.mouse_position_x >= VGA_WIDTH ||
							this->mouse.relative_mouse_position_y < 0 ||
							this->mouse.relative_mouse_position_y >= VGA_HEIGHT ? SDL_TRUE : SDL_FALSE
						);

						if (sdl_event.motion.xrel != 0 || sdl_event.motion.yrel != 0)
						{ this->mouse.inactive = false; }
						
						break;
					}

					case SDL_MOUSEBUTTONDOWN:
					{ this->mouse.inactive = false; }
						
						// fall through -- does the brace syntax prevent fallthrough?
					case SDL_MOUSEBUTTONUP:
					{
						// mapWindowPointToScreen(&sdl_event.button.x, &sdl_event.button.y); // video.h

						if (sdl_event.type == SDL_MOUSEBUTTONDOWN)
						{
							this->mouse.new_mouse = true;
							this->mouse.most_recent_mouse_button = sdl_event.button.button;
	
							this->mouse.last_mouse_position_x = sdl_event.button.x;
							this->mouse.last_mouse_position_y = sdl_event.button.y;
							this->mouse.mouse_down = true;
						}

						else
						{ this->mouse.mouse_down = false; }


						int mouse_button_id = -1;
						switch (sdl_event.button.button)
						{
							case SDL_BUTTON_LEFT:   mouse_button_id = 0; break;
							case SDL_BUTTON_RIGHT:  mouse_button_id = 1; break;
							case SDL_BUTTON_MIDDLE: mouse_button_id = 2; break;
						}
	
						if (mouse_button_id < 0)
						{ break; }
						
						// TODO: replace with actual settings/configuration logic
						const uint8_t MOUSE_SETTINGS[3]{ 1, 4, 5 };

						switch (MOUSE_SETTINGS[mouse_button_id])
						{
							case 1: // Fire Main Weapons
							{
								this->mouse.buttons[0].pressed = this->mouse.mouse_down;
								break;
							}

							case 2: // Fire Left Sidekick
							{
								this->mouse.buttons[1].pressed = this->mouse.mouse_down;
								break;								
							}

							case 3: // Fire Right Sidekick
							{
								this->mouse.buttons[2].pressed = this->mouse.mouse_down;
								break;								
							}

							case 4: // Fire BOTH Sidekicks
							{
								this->mouse.buttons[1].pressed = this->mouse.mouse_down;
								this->mouse.buttons[2].pressed = this->mouse.mouse_down;
								break;								
							}

							case 5: // Change Rear Mode
							{
								this->mouse.buttons[3].pressed = this->mouse.mouse_down;
								break;								
							}
						}

						break;
					}
					

					case SDL_TEXTINPUT:
					{
						SDL_strlcpy(this->last_text, sdl_event.text.text, COUNTOF(this->last_text));
						this->new_text = true;

						break;						
					}

					case SDL_TEXTEDITING:
					{ break; }

					case SDL_QUIT:
					{
						/* TODO: Call the cleanup code here. */
						exit(0);
						break;						
					}
				}
			}
		}

	public:

		// Default Constructor
		Input_Handler()
		{ }

		// Methods
		void flush_events_buffer(void)
		{
			SDL_Event sdl_event;
			while (SDL_PollEvent(&sdl_event));
		}

		void wait_input(bool keyboard_bool, bool mouse_bool, bool joystick_bool)
		{
			service_SDL_events(false);

			// Continue to poll for input on all devices until input is detected
			while (
				!(
					(keyboard_bool && this->keyboard.key_down) ||
					(mouse_bool && this->mouse.mouse_down) ||
					(joystick_bool && this->joystick.joy_down)
				)
			)
			{
				SDL_Delay(SDL_POLL_INTERVAL);
				// push_joysticks_as_keyboard();
				service_SDL_events(false);

				// if (isNetworkGame)
				// { network_check(); }
			}
		}
};