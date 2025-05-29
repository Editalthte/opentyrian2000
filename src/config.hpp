#pragma once

#include <fstream>
#include <map>
#include <nlohmann/json.hpp> // https://github.com/nlohmann/json
#include <string>

using nlohmann::json;
using std::string;

enum Control_Type
{
	keyboard,
	mouse,
	joystick
};

struct Controls_Config
{
	string up_key;
	string down_key;
	string left_key;
	string right_key;

	string fire_key;
	string rear_mode_change_key;
	string sidekick_left_key;
	string sidekick_right_key;
};

struct Video_Config
{
	bool fullscreen;
	int scaler;
	string scaling_mode;
};

class Config
{
	std::map<Control_Type, Controls_Config> controls_config;
	Video_Config video_config;
};