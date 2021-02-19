
class CfgPatches
{
	class DayZDoom_Scripts
	{
        units[] = {};
        weapons[] = {};
        requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Scripts"};
	};
};

class CfgMods 
{
	class DZ_DayZDoom
	{
		name = "Mod Template";
		dir = "DayZDoom";
		credits = "";
		author = "";
		creditsJson = "DayZDoom/Scripts/Data/Credits.json";
		versionPath = "DayZDoom/scripts/Data/Version.hpp";
		inputs = "DayZDoom/Scripts/Data/Inputs.xml";
		type = "mod";
		dependencies[] =
		{
			"Game", "World", "Mission"
		};
		class defs
		{
			class imageSets
			{
				files[]=
				{
					"DayZDoom/gui/imagesets/dayz_editor_gui.imageset"
				};
			};
			class engineScriptModule
			{
				value = "";
				files[] =
				{
					"DayZDoom/scripts/common",
					"DayZDoom/scripts/1_core"
				};
			};

			class gameScriptModule
			{
				value="";
				files[] = 
				{
					"DayZDoom/scripts/common",
					"DayZDoom/scripts/3_Game"
				};
			};
			class worldScriptModule
			{
				value="";
				files[] = 
				{
					"DayZDoom/scripts/common",
					"DayZDoom/scripts/4_World"
				};
			};

			class missionScriptModule 
			{
				value="";
				files[] = 
				{
					"DayZDoom/scripts/common",
					"DayZDoom/scripts/5_Mission"
				};
			};
		};
	};
};
