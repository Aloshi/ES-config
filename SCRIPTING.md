First, an example (you might want to save this as an .xml for proper syntax highlighting):

```
<!--Everything must be enclosed in an <emulator> element. Only one of these will be read per file. -->
<emulator>
	<!-- Name as it will appear in the emulator select screen. -->
	<name>DGen</name>
	<!-- Name as it will appear before the input name in the Test screen. -->
	<shortName>Gen</shortName>

	<!-- File we will write to. No expansion is currently performed. -->
	<configPath>dgen.cfg</configPath>

	<!-- Doesn't actually do anything yet, will eventually cap max players when saving. -->
	<maxPlayers>2</maxPlayers>

	<!-- Declare any global variables or functions here. They'll stay in scope across every script in this file. 
	Not wrapped with anything. -->
	<declarationScript>
	string value = "";
	string deviceType = "";
	string playerNum = "";
	
	void writePair(File@ f, string inputName)
	{
		f.write(deviceType + "_pad" + playerNum + "_" + inputName + " = " + value);
	}
	
	string getDGenKey(int val)
	{
		string name = getKeyName(val); //getKeyName is a global function defined by ES-config
		if(name == "left shift") //this is not an exhaustive list, but some of the more common keybinds
			name = "lshift";
		else if(name == "right shift")
			name = "rshift";
		else if(name == "return")
			name = "enter";
		else if(name == "left alt")
			name = "lalt";
		else if(name == "right alt")
			name = "ralt";
		
		return name;
	}
	</declarationScript>

	<!-- Called once, at the very beginning of writing the config file. Most useful for writing "always on" settings (e.g. "use joystick").
	Wrapped with: void init(File@ f) { -->
	<initScript>
	f.write("bool_joystick = true");
	</initScript>

	<!-- Called once before writing for each player. Most useful for writing the player's device ID, or performing initialization.
	Wrapped with: void perPlayer(File@ f, int PLAYER_NUMBER, int DEVICE_ID) { -->
	<perPlayerScript>
		playerNum = PLAYER_NUMBER + 1;
		
		if(DEVICE_ID == DEVICE_KEYBOARD)
			deviceType = "key";
		else
			deviceType = "joy";
	</perPlayerScript>

	<!-- Called once before each input. You should put code that would be repeated in your <input> blocks here.
	You probably want to update your "global" variables here for the following write. 
	Wrapped with: void perInput(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID) { -->
	<perInputScript>
	switch(INPUT_TYPE)
	{
		case TYPE_BUTTON:
			value = "joystick" + DEVICE_ID + "-button" + INPUT_ID;
			break;
		
		case TYPE_AXIS:
			value = "joystick" + DEVICE_ID + "-axis" + INPUT_ID + "-" + ((INPUT_VALUE > 0) ? "max" : "min");
			break;
			
		case TYPE_HAT:
			value = "joystick" + DEVICE_ID + "-hat" + INPUT_ID + "-" + getHatDir(INPUT_VALUE);
			break;
		
		case TYPE_KEY:
			value = getDGenKey(INPUT_ID);
			break;
		
		default:
			print("unexpected input type?! [" + INPUT_TYPE + "]");
			break;
	}
	</perInputScript>

	<!-- List of inputs. Inputs have attributes that are used within ES-config, and optionally contain a script for extra writing code. 
	Wrapped with: void input_{name}(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID) { -->
	<input name="A" location="btnDown" type="BUTTON|AXIS|HAT|KEY">writePair(f, "a");</input>
	<input name="B" location="btnRight" type="BUTTON|AXIS|HAT|KEY">writePair(f, "b");</input>
	<input name="L" location="lshoulder1" type="BUTTON|AXIS|HAT|KEY">writePair(f, "l");</input>
	<input name="R" location="rshoulder1" type="BUTTON|AXIS|HAT|KEY">writePair(f, "r");</input>
	
	<input name="Start" location="start" type="BUTTON|AXIS|HAT|KEY">writePair(f, "start");</input>
	<input name="Select" location="select" type="BUTTON|AXIS|HAT|KEY">writePair(f, "select");</input>
	
	<input name="Up" location="up" type="BUTTON|AXIS|HAT|KEY">writePair(f, "up");</input>
	<input name="Down" location="down" type="BUTTON|AXIS|HAT|KEY">writePair(f, "down");</input>
</emulator>
```

The scripting language is AngelScript, which is very similar to C/C++.  The most noticeable difference I've found is that "@" is used intead of "*" when dealing with pointers/references.

Stuff Registered with AngelScript
=================================

`enum InputType` - TYPE_BUTTON, TYPE_AXIS, TYPE_HAT, and TYPE_KEY.  This is the format of INPUT_TYPE.
`string getHatDir(int value)` - Returns the direction of a hat (normally the value is a bitmask so you can't use it directly). Returns "up", "down", "left", or "right".
`string getKeyName(int id)` - Returns the readable version of an SDL key ID. You may need to convert modifier keys (control, shift, alt) to a different format (e.g. "right shift" -> "rshift" for RetroArch).
`class File` - Cannot be instantiated within AngelScript. A reference to a file opened for `<configPath>` is passed with most functions.
`	void File.write(string line)` - Write a line to the file. A newline (\n) will be appended.
`	void File.open(string path)` - Close the open file and open `path`. The reference will not change.

How files are read
==================

As you can see, script files are the bastard child of XML and AngelScript.  They're read by ES-config like this:

1. Parse "meta" tags, such as name, short name, config path.

2. Patch together the AngelScript module. This is done by essentially pasting the content of a bunch of tags together, wrapping some in method declarations...

```
<declarationScript/>

void init(File@ f) {
	<initScript/>
}

void perPlayer(File@ f, int PLAYER_NUMBER, int DEVICE_ID) {
	<perPlayerScript/>
}

void perInput(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID) {
	<perInputScript/>
}

//for each input
void input_{name}(File@ f, string INPUT_NAME, InputType INPUT_TYPE, int INPUT_ID, int INPUT_VALUE, int DEVICE_ID) {
	<input/>
}
```

3. The emulator will eventually be written like so, called from the C++ side, similar to this AngelScript:

```
File@ f = new File(<configPath>);
init(f);
perPlayer(f, PLAYER_NUMBER, DEVICE_ID);

for each input
	perInput(f, INPUT_NAME, INPUT_TYPE, INPUT_ID, INPUT_VALUE, DEVICE_ID);
	input_{name}(f, INPUT_NAME, INPUT_TYPE, INPUT_ID, INPUT_VALUE, DEVICE_ID);
```


Meta Stuff
==========

Declared in the root of `<emulator>`.

REQUIRED:

`<name>` - contains the name of this emulator.

`<shortName>` - contains the abbreviated name for this emulator, used on the Test screen.

`<configPath>` - contains the path we will write the config file to.


Input Attributes
================

REQUIRED:

`name` - string for the name the input will be displayed as.

`location` - string for the location for this input.  *Inputs of the same location will be grouped together across emulators.*


OPTIONAL:

`require` - if "true", this input *must* be mapped.

`type` - list of accepted types, delimited with "|". Accepted types are "BUTTON", "AXIS", "HAT", and "KEY". Think of it like a bitmask.  If not supplied, the input will accept any type.

`mirrorAxis` - string of a `location` to require an axis mirror - that is, both this and the mirror location have the same INPUT_ID with opposing INPUT_VALUEs.  Will only be applied if either this or the mirror location is mapped to an Axis.  *Does not force either type to be an Axis - if you want that, use type="AXIS"!*

-Aloshi
http://www.aloshi.com
