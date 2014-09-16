#ifndef X2D_ENGINE_H
#define X2D_ENGINE_H

#include "math/math.h"
#include "scripts/scriptarray.h"

#include "config.h"

#define LOG(str, ...) XConsole::Log(str, __VA_ARGS__)

#define NOT_IMPLEMENTED(func)			LOG("%s does not have a implementation.", #func);
#define NOT_IMPLEMENTED_RET(func, ret)	LOG("%s does not have a implementation.", #func); return ret;

/*********************************************************************
**	Game console													**
**********************************************************************/

class XFileSystem;
class XFileReader;
class XFileWriter;
class XDebugger;

class XDAPI XConsole
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	XConsole();
	virtual ~XConsole();

	void log(const string &msg);
	void logf(const char *msg, ...);
	string getLog() const;

	void clear();
	void exportFile() const;

	string readBuffer();
	bool hasBuffer() const;
	void clearBuffer();

	static void Log(const char *msg, ...);

protected:
	virtual void syslog(const string &msg) {}

private:
	void call_log(const char *msg, va_list args);

	string m_log;
	string m_buffer;
	
	XDebugger *m_debugger;
	XFileWriter *m_output;
	
	static bool s_initialized;
	static XConsole *s_this;
};

/*********************************************************************
**	Asset manager													**
**********************************************************************/

// Image formats
enum XImageFormat
{
	UNKNOWN_IMAGE_TYPE,
	PNG_IMAGE_TYPE,
	BMP_IMAGE_TYPE,
	DDS_IMAGE_TYPE,
	PSD_IMAGE_TYPE,
	TGA_IMAGE_TYPE,
	JPG_IMAGE_TYPE
};

// Sound formats
enum XSoundFormat
{
	UNKNOWN_SOUND_TYPE,
	WAV_SOUND_TYPE,
	MP3_SOUND_TYPE,
	OGG_SOUND_TYPE
};

class XAudioBuffer;
class XPixmap;

class XDAPI XAssetManager
{
public:
	static XPixmap *LoadPixmap(string filePath, const XImageFormat format = UNKNOWN_IMAGE_TYPE);
	static void SavePixmap(string filePath, XPixmap *pixmap, const XImageFormat format = UNKNOWN_IMAGE_TYPE);

	static XAudioBuffer *LoadSound(string filePath, const XSoundFormat format = UNKNOWN_SOUND_TYPE);
	static void SaveSound(string filePath, XAudioBuffer *buffer, const XSoundFormat format = UNKNOWN_SOUND_TYPE);
};

/*********************************************************************
**	Window class													**
**********************************************************************/
class XDAPI XWindow
{
public:
	AS_DECL_SINGLETON

	virtual ~XWindow() {}

	virtual void processEvents() { NOT_IMPLEMENTED(processEvents) }

	// Virtual window functions
	virtual void enableFullscreen()									{ NOT_IMPLEMENTED(enableFullscreen) }
	virtual void disableFullscreen()								{ NOT_IMPLEMENTED(disableFullscreen) }
	virtual bool isFullscreen() const								{ NOT_IMPLEMENTED_RET(isFullscreen, false) }
	virtual XScriptArray* getResolutionList() const					{ NOT_IMPLEMENTED_RET(getResolutionList, 0) }

	// Window flags
	virtual void enableResize()										{ NOT_IMPLEMENTED(enableResize) }
	virtual void disableResize()									{ NOT_IMPLEMENTED(disableResize) }
	virtual bool hasFocus()											{ NOT_IMPLEMENTED_RET(hasFocus, true) }

	// Window actions
	virtual void     setPosition(const Vector2i &pos)				{ NOT_IMPLEMENTED(move) }
	virtual Vector2i getPosition() const							{ NOT_IMPLEMENTED_RET(pos, Vector2i(0)) }
	virtual void     setSize(const Vector2i &size)					{ NOT_IMPLEMENTED(resize) }
	virtual Vector2i getSize() const								{ NOT_IMPLEMENTED_RET(size, Vector2i(0)) }
	virtual void	 minimize()										{ NOT_IMPLEMENTED(minimize) }
	virtual void	 maximize()										{ NOT_IMPLEMENTED(maximize) }
	virtual void	 restore()										{ NOT_IMPLEMENTED(restore) }
};

/*********************************************************************
**	Graphics														**
**********************************************************************/

class XTexture;
class XShader;
class XVertexBufferObject;
class XFrameBufferObject;

XDAPI extern uint QUAD_INDICES[6];
XDAPI extern Vector4 QUAD_VERTICES[4];
XDAPI extern Vector2 QUAD_TEXCOORD[4];

class XDAPI XGraphics
{
	friend class XEngine;
	friend class XBatch;
public:
	AS_DECL_SINGLETON

	XGraphics();
	virtual ~XGraphics() {}

	enum Feature
	{
		VertexBufferObjects,
		FrameBufferObjects
	};

	// Global factories
	static XTexture*			CreateTexture(const XPixmap &pixmap);
	static XTexture*			CreateTexture(const string &filePath);
	static XTexture*			CreateTexture(const int width, const int height);
	static XTexture*			CreateTexture(const XTexture &texture);
	static XShader*				CreateShader(const string &vertFilePath, const string &fragFilePath);
	static XVertexBufferObject*	CreateVertexBufferObject();
	static XFrameBufferObject*	CreateFrameBufferObject();
	static bool					IsSupported(Feature feature);

	// Refresh rate
	void setRefreshRate(const int hz);
	int getRefreshRate() const;

	// Vsync
	virtual void enableVsync()										{ NOT_IMPLEMENTED(enableVsync) }
	virtual void disableVsync()										{ NOT_IMPLEMENTED(disableVsync) }

	// Time step & fps
	float getTimeStep() const;
	float getFPS() const;

	// Swap buffers
	virtual void swapBuffers() = 0;

private:
	float m_framesPerSecond;
	int m_refreshRate;
	float m_timeStep;
	
	virtual void renderBatch(const XBatch &batch) = 0;
	virtual XTexture*				createTexture(const XPixmap &pixmap) = 0;
	virtual XShader*				createShader(const string &vertFilePath, const string &fragFilePath) = 0;
	virtual XVertexBufferObject*	createVertexBufferObject() = 0;
	virtual XFrameBufferObject*		createFrameBufferObject() = 0;
	virtual bool					isSupported(Feature feature) = 0;

	static XGraphics *s_this;
};


/*********************************************************************
**	Abstract Audio Manager											**
**********************************************************************/

class XAudioSource;
class XAudioBuffer;

class XDAPI XAudioManager
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON
		
	// Listener position
	virtual void setPosition(const Vector2&)	{ NOT_IMPLEMENTED(setPosition) }
	virtual Vector2 getPosition() const			{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Listener velocity
	virtual void setVelocity(const Vector2&)	{ NOT_IMPLEMENTED(setVelocity) }
	virtual Vector2 getVelocity() const			{ NOT_IMPLEMENTED_RET(getVelocity, Vector2(0.0f)) }

	// Listener orientation
	virtual void setOrientation(const Vector3&) { NOT_IMPLEMENTED(setOrientation) }
	virtual Vector3 getOrientation() const		{ NOT_IMPLEMENTED_RET(getOrientation, Vector3(0.0f)) }
	
	// Global factories
	static XAudioSource *CreateSource(XAudioBuffer *buffer);
	static XAudioSource *CreateSource(const string &path);

private:
	// XAudioManager instance
	static XAudioManager *s_this;

	// Virtual factory functions
	virtual XAudioSource *createSource(XAudioBuffer *buffer) = 0;
};

/*********************************************************************
**	Event handler													**
**********************************************************************/
class XDAPI XTimer
{
public:
	AS_DECL_SINGLETON

	XTimer();
	
	// High-resolution timing
	virtual void  start()			= 0;
	virtual float getTime() const	= 0;

	// System clock
	void getTicksPerSecond();
	long getTickCount() const;

private:
	int m_ticksPerSec;
};

/*********************************************************************
**	Event handler													**
**********************************************************************/
class XDAPI XEventHandler
{
	virtual void processEvents() = 0;
};

/*********************************************************************
**	Thread manager													**
**********************************************************************/
class XThreadManager
{
public:
	virtual void setupThread() = 0;
	virtual void cleanupThread() = 0;
};

/*********************************************************************
**	Script engine													**
**********************************************************************/

class StringStream;

struct ScriptValue
{
	ScriptValue();
	~ScriptValue();

	void set(void *value, int typeId);
	void clear();

	bool valid;
	int typeId;
	void *value;
};

// A asIScriptEngine wrapper (mainly used by external plugins)
class XDAPI XScriptEngine
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	XScriptEngine();
	~XScriptEngine();

	// Script classes
	uint classCount() const;
	int classIdByName(const string className) const;
	string classNameById(const uint idx) const;
	bool isClassName(const string className) const;
	bool classDerivesFromName(const string class1, const string class2) const;
	bool classDerivesFromId(const uint id1, const uint id2) const;

	// Script objects
	string objectClassName(void *obj, int typeId) const;

	// Execute string
	void executeString(const string &str) const;

	// Serializing
	void serialize(asIScriptObject *object, string &path);
	void serialize(asIScriptObject *object, StringStream &ss);
	asIScriptObject *deserialize(string &path);
	asIScriptObject *deserialize(StringStream &ss);

	static asIScriptContext *CreateContext();
	static asIScriptEngine *GetAngelScript();
	static asIScriptModule *GetModule();

private:
	static asIScriptEngine *s_engine;
	static asIScriptModule *s_module;
	static XDebugger *s_debugger;
};

// AngelScript functions
void asMessageCallback(const asSMessageInfo *msg, void *param);
int  asCompileModule(const string &name);

/*********************************************************************
**	File reader class												**
**********************************************************************/
class XDAPI XFileReader
{
public:
	virtual ~XFileReader() {}

	virtual bool isOpen() = 0;
	virtual bool isEOF() = 0;
	virtual void close() = 0;

	virtual string readLine() = 0;
	virtual string readAll() = 0;
};

/*********************************************************************
**	File writer class												**
**********************************************************************/
class XDAPI XFileWriter
{
public:
	virtual ~XFileWriter() {}

	virtual bool isOpen() = 0;
	virtual void close() = 0;

	virtual void clear() = 0;
	virtual void append(const char* data, const int length) = 0;
	void append(const string &str) { append(str.data(), str.size()); }
	virtual void flush() = 0;
};

/*********************************************************************
**	File system class												**
**********************************************************************/
class XDAPI XFileSystem
{
	friend class XEngine;
public:
	AS_DECL_SINGLETON

	// File buffers
	bool readFile(string filePath, string &conent) const;
	bool writeFile(string filePath, const string content) const;

	// OS spesifics
	virtual bool fileExists(string &filePath) const;
	// NOTE TO SELF: I might want to consider making a DirectoryIterator instead of using this function
	virtual XScriptArray *listFiles(string &directory, const string &mask, const bool recursive) const		{ NOT_IMPLEMENTED_RET(listFiles, 0) }			// Optional
	virtual XScriptArray *listFolders(string &directory, const string &mask, const bool recursive) const	{ NOT_IMPLEMENTED_RET(listFolders, 0) }			// Optional
	virtual bool remove(string &path)																		{ NOT_IMPLEMENTED_RET(remove, false) }			// Optional

	// System windows
	virtual string showSaveDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showSaveDialog, "") }				// Optional
	virtual string showOpenDialog(const string &file, const string &ext, const string &title, const string &folder) const	{ NOT_IMPLEMENTED_RET(showOpenDialog, "") }				// Optional

	// Static factories
	static XFileWriter *CreateFileWriter(const string &filePath) { return s_this->createFileWriter(filePath); }
	static XFileReader *CreateFileReader(const string &filePath) { return s_this->createFileReader(filePath); }

	// Static functions
	static bool ReadFile(string path, string &content);
	static bool WriteFile(string path, const string &content);
	static bool MakeDir(string path);

protected:
	static XFileSystem *s_this;
	virtual XFileWriter *createFileWriter(const string &filePath) = 0;
	virtual XFileReader *createFileReader(const string &filePath) = 0;
	virtual bool makeDir(const string &path) = 0;
};

/*********************************************************************
**	Input class														**
**********************************************************************/

// Mouse buttons
enum XMouseButton
{
	X2D_LeftMouseButton,
	X2D_MiddleMouseButton,
	X2D_RightMouseButton
};

// Virtual keys
enum XVirtualKey
{
	// Standard ASCII-Mapped keys
	XD_KEY_NULL = 0x00,
	XD_KEY_BACKSPACE = 0x08,
	XD_KEY_TAB = 0x09,
	XD_KEY_ENTER = 0x0D,
	XD_KEY_ESCAPE = 0x1B,
	XD_KEY_SPACE = 0x20,

	XD_KEY_0 = 0x30,
	XD_KEY_1,
	XD_KEY_2,
	XD_KEY_3,
	XD_KEY_4,
	XD_KEY_5,
	XD_KEY_6,
	XD_KEY_7,
	XD_KEY_8,
	XD_KEY_9,

	// Alphabetic keys
	XD_KEY_A = 0x41,
	XD_KEY_B,
	XD_KEY_C,
	XD_KEY_D,
	XD_KEY_E,
	XD_KEY_F,
	XD_KEY_G,
	XD_KEY_H,
	XD_KEY_I,
	XD_KEY_J,
	XD_KEY_K,
	XD_KEY_L,
	XD_KEY_M,
	XD_KEY_N,
	XD_KEY_O,
	XD_KEY_P,
	XD_KEY_Q,
	XD_KEY_R,
	XD_KEY_S,
	XD_KEY_T,
	XD_KEY_U,
	XD_KEY_V,
	XD_KEY_W,
	XD_KEY_X,
	XD_KEY_Y,
	XD_KEY_Z,

	// Mouse keys
	X2D_LeftMouse = 0x80,
	X2D_RightMouse,
	X2D_MiddleMouse,

	// Arrow keys
	XD_KEY_Left,
	XD_KEY_Up,
	XD_KEY_Right,
	XD_KEY_Down,

	// Special keys
	XD_KEY_Shift,
	XD_KEY_LeftShift,
	XD_KEY_RightShift,
	XD_KEY_Control,
	XD_KEY_LeftControl,
	XD_KEY_RightControl,
	XD_KEY_Menu, // Windows button
	XD_KEY_LeftMenu,
	XD_KEY_RightMenu,
	XD_KEY_Space,
	XD_KEY_PageUp,
	XD_KEY_PageDown,
	XD_KEY_End,
	XD_KEY_Home,
	XD_KEY_Snapshot,
	XD_KEY_Insert,
	XD_KEY_Delete,

	// Numpad keys
	XD_KEY_Numpad0,
	XD_KEY_Numpad1,
	XD_KEY_Numpad2,
	XD_KEY_Numpad3,
	XD_KEY_Numpad4,
	XD_KEY_Numpad5,
	XD_KEY_Numpad6,
	XD_KEY_Numpad7,
	XD_KEY_Numpad8,
	XD_KEY_Numpad9,
	XD_KEY_Plus,
	XD_KEY_Hyphen,

	// Other keys
	XD_KEY_Caret,
	XD_KEY_Apostrophe,
	XD_KEY_QuotationMark,
	XD_KEY_Backslash,
	XD_KEY_Slash,
	XD_KEY_Asterisk,
	XD_KEY_Pipe,
	XD_KEY_Colon,
	XD_KEY_Semicolon,
	XD_KEY_Tilde,
	XD_KEY_Comma,
	XD_KEY_Period,
	XD_KEY_Greater,
	XD_KEY_Less,
	XD_KEY_Equals
};

class XDAPI XInput
{
public:
	AS_DECL_SINGLETON

	virtual ~XInput();

	// Desktop cursor functions
	virtual void     setCursorPos(const Vector2i &pos)			{ NOT_IMPLEMENTED(setCursorPos) }
	virtual Vector2i getCursorPos() const						{ NOT_IMPLEMENTED_RET(getCursorPos, Vector2i(0)) }
	virtual void     setCursorLimits(const Recti &area)			{ NOT_IMPLEMENTED(setCursorLimits) }

	// Key state function
	virtual bool getKeyState(const XVirtualKey key) const		{ NOT_IMPLEMENTED_RET(getKeyState, false) }

	// General position
	virtual Vector2 getPosition() const 						{ NOT_IMPLEMENTED_RET(getPosition, Vector2(0.0f)) }

	// Key binding
	void bind(const XVirtualKey key, asIScriptFunction *func);
	void unbind(const XVirtualKey key);
	void unbindAll();
	void updateBindings();
	void checkBindings();

	// Keyboard listener
	void addKeyboardListener(asIScriptObject *object);
	void charEvent(uint utf8char);
	void keyPressed(const XVirtualKey key);
	void keyReleased(const XVirtualKey key);
	
	// Overloads
	void setCursorPos(const int x, const int y) { setCursorPos(Vector2i(x, y)); }
	void setCursorLimits(const int x, const int y, const int w, const int h) { setCursorLimits(Recti(x, y, w, h)); }

private:
	struct KeyBind
	{
		bool pressed;
		asIScriptFunction *function;
	};

	map<XVirtualKey, KeyBind> m_keyBindings;
	vector<asIScriptObject*> m_keyListeners;
};


/*********************************************************************
**	Profiler														**
**********************************************************************/
class XDAPI XProfiler
{
	struct Profile
	{
		Profile *parent;
		map<string, Profile*> children;
		float currentTime;
		vector<float> eplacedTimes;
	};
public:
	AS_DECL_SINGLETON

	XProfiler(XTimer *timer);
	virtual ~XProfiler();

	void start();
	void stop();

	void push(const string &name);
	void pop();

	void printResults();
	void printChildren(Profile *node, const string &name, const int depth, const float parentTotal, const float globalTotal);

	bool isActive() const { return m_active; }

	// Static functions
	static void Push(const string &name) { s_this->push(name); }
	static void Pop() { s_this->pop(); }

private:
	XTimer *m_timer;
	bool m_active;
	float m_startTime;
	float m_totalTime;
	Profile *m_root;
	string m_rootName;
	Profile *m_currentNode;
	map<string, Profile> m_profiles;

	static XProfiler *s_this;
};

/*********************************************************************
**	Default Debugger												**
**********************************************************************/

// Packet types
enum XPacketType
{
	X2D_NULL_PACKET = 0x00,
	X2D_CONNECTED_PACKET,
	X2D_INITIALIZED_PACKET,
	X2D_MESSAGE_PACKET,
	X2D_COMPILE_PACKET,
	X2D_BREAK_PACKET,
	X2D_TEXT_VAR_PACKET,
	X2D_IMAGE_VAR_PACKET
};

class XDebugger
{
public:
	XDebugger() :
		m_connected(false),
		m_command(NoCommand),
		m_prevStackSize(0),
		m_timeoutValue(60000)
	{
	}
	
	virtual ~XDebugger() {
	}

	/*********************************************************************
	**	Virtual part													**
	**********************************************************************/
	virtual bool init()											{ NOT_IMPLEMENTED_RET(init, false) }
	virtual void setTimeoutValue(const uint ms)					{ NOT_IMPLEMENTED(setTimeoutValue) }
	virtual bool listen(const ushort port)						{ NOT_IMPLEMENTED_RET(listen, false) }
	virtual bool accept()										{ NOT_IMPLEMENTED_RET(accept, false) }
	virtual void disconnect()									{ NOT_IMPLEMENTED(disconnect) }
	virtual bool send(const char *data)							{ NOT_IMPLEMENTED_RET(send, false) }
	virtual bool recv(char **data)								{ NOT_IMPLEMENTED_RET(recv, false) }
	virtual int  bytesReady()									{ NOT_IMPLEMENTED_RET(bytesReady, 0) }
	
	/*********************************************************************
	**	Implemented part												**
	**********************************************************************/
	bool connect();
	void sendPacket(XPacketType type, const char *data = 0);
	void recvPacket(char **data);

	void lineCallback(void *ctx);
	bool isBreakpoint(void *ctx);
	void takeCommands(void *ctx);

private:
	enum Command
	{
		NoCommand = 0x00,
		Continue,
		Interupt,
		StepOver,
		StepInto,
		StepOut,
		AddBreakpoint,
		RemoveBreakpoint
	};

	struct Breakpoint
	{
		string file;
		int line;
        bool operator==(const Breakpoint &other)
		{
            return file == other.file && line == other.line;
        }
	};

	bool m_connected;
	Command m_command;
	uint m_prevStackSize;
	uint m_timeoutValue;
	list<Breakpoint> m_breakpoints;
};

/*********************************************************************
**	Exception class													**
**********************************************************************/
class XException
{
public:
	XException(XRetCode code, string msg) :
		m_errorCode(code),
		m_message(msg)
	{
	}

	XRetCode errorCode() const
	{
		return m_errorCode;
	}

	string message() const
	{
		return m_message;
	}

private:
	string m_message;
	XRetCode m_errorCode;
};

/*********************************************************************
**	Engine config													**
**********************************************************************/
typedef int (*LoadPluginsFunc)(XEngine*);
typedef int (*LoadEventsFunc)(XEngine*);

struct XDAPI XConfig
{
	XConfig();

	bool isValid() const
	{
		return strlen(platform) > 0 && strlen(workDir) > 0 &&
			fileSystem != 0 && timer != 0 && input != 0 &&
			graphics != 0 && audio != 0;
	}

	void operator=(const XConfig &other)
	{
		flags = other.flags;
		platform = other.platform;
		workDir = other.workDir;
		saveDir = other.saveDir;
		loadPluginsFunc = other.loadPluginsFunc;
		timer = other.timer;
		fileSystem = other.fileSystem;
		window = other.window;
		input = other.input;
		graphics = other.graphics;
		audio = other.audio;
		console = other.console;
	}

	// Engine running flags (optional)
	int				flags;

	// Platform string (required)
	const char*		platform;

	// Engine working directory (required)
	const char*		workDir;

	// System save directory (optional)
	const char*		saveDir;

	// Syster user directory (optional)
	const char*		userDir;

	// Plugin load function (optional)
	LoadPluginsFunc	loadPluginsFunc;

	// Load events
	LoadEventsFunc loadEventsFunc;

	// Engine timer object (required)
	XTimer*			timer;

	// File system manager (required)
	XFileSystem*	fileSystem;

	// Game window manager (optional)
	XWindow*		window;

	// Input class (required)
	XInput*			input;

	// Graphics manager (required)
	XGraphics*		graphics;

	// Audio manager (required)
	XAudioManager*	audio;

	// Game console (optional)
	XConsole*		console;
};

/*********************************************************************
**	Game engine														**
**********************************************************************/
class XDAPI XEngine
{
public:
	AS_DECL_SINGLETON

	XEngine();
	~XEngine();

	// Initialize the engine
	int init(const XConfig &config);

	// Run game
	int run();
	
	// Exit game
	void exit();

	// Exception
	//virtual void exception(const xdRetCode) = 0;

	// Platform string
	string getPlatformString() const;

	// Working directory
	string getWorkingDirectory() const;

	// Save directory
	string getSaveDirectory() const;

	// Profiler
	void toggleProfiler();
	void pushProfile(const string &profile);
	void popProfile();
	void setProfiler(XProfiler *profiler) { m_profiler = profiler; }

	// Scene
	void pushScene(asIScriptObject *object);
	void popScene();

	// Debugger
	void setDebugger(XDebugger *debugger) { m_debugger = debugger; }

	// Exceptions
	void exception(XRetCode errorCode, const char* message);

	// Static functions
	static bool IsEnabled(const XEngineFlag flag) { return (s_this->m_flags & flag) != 0; }
	static string GetWorkingDirectory() { return s_this->m_workDir; }
	static string GetSaveDirectory() { return s_this->m_saveDir; }

private:
	int m_flags;

	string m_platformString;
	string m_workDir;
	string m_saveDir;
	
	XFileSystem*	m_fileSystem;
	XGraphics*		m_graphics;
	XAudioManager*	m_audio;
	XProfiler*		m_profiler;
	XDebugger*		m_debugger;
	XTimer*			m_timer;
	XScriptEngine*	m_scripts;
	XConsole*		m_console;
	XWindow*		m_window;
	XMath*			m_math;
	XInput*			m_input;
	XAssetManager*	m_assetManager;

	// Game loop
	void draw();
	void update();
	void pause() { m_paused = true; }
	void resume() { m_paused = false; }
	void close() { m_running = false; }
	
	// State
	bool m_running;
	bool m_paused;

	bool m_toggleProfiler;

	// Scene stack
	stack<asIScriptObject*> m_sceneStack;

	// Event functions
	asIScriptFunction *m_defaultUpdateFunc;
	asIScriptFunction *m_defaultDrawFunc;
	asIScriptFunction *m_sceneUpdateFunc;
	asIScriptFunction *m_sceneDrawFunc;

	bool m_initialized;

	static XEngine *s_this;
};

XDAPI XEngine *CreateEngine();

#endif // X2D_ENGINE_H