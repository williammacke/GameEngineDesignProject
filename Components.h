#ifndef COMPONENTS_H_
#define COMPONENTS_H_

class WindowManager {
public:
	WindowManager();
	~WindowManager();
	static WindowManager get();
};

class RenderEngine {
public:
	RenderEngine();
	~RenderEngine();
	static RenderEngine get();
private:
	WindowManager w;
};

class GameEngine {
public:
	GameEngine();
	~GameEngine();
	static GameEngine get();
private:
	RenderEngine r;
};

#endif
