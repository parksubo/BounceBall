#include "Game.h"

Game::Game() : 
	mWindow(nullptr),
	mIsRunning(true)
{

}


bool Game::Initialize()
{
	// Initialize SDL 	
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	// SDL_Init�� 0�� �ƴϸ� �ʱ�ȭ�� ���� -> false ��ȯ
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	// Create SDL
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Test1)",	// ������ ����
		100,	// �������� ���� ��� x��ǥ
		100,	// �������� ���� ��� y��ǥ
		1024,	// �������� �ʺ�
		768,	// ������ ����
		//0		// �÷��� (0�� ��� �÷��׵� �������� ������ �ǹ�)
		SDL_WINDOW_RESIZABLE
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create windows: %s", SDL_GetError());
		return false;
	}

	// Create SDL Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,	// �������� ���� ������ ������
		-1,			// �Ϲ������� -1��
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	)


}

void Game::Shutdown()
{
	// SDL_Window ��ü ����
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	// mIsRunning�� false�� �� �� ���� ���ӷ����� �ݺ��ؼ� ����
	while (mIsRunning)
	{
		ProcessInput();
		//UpdateGame();
		//GenarateOutput();
	}
}

void Game::ProcessInput()
{
	SDL_Event event;
	// ť�� ������ �̺�Ʈ�� ���� �ִ� ����
	while (SDL_PollEvent(&event))	// SDL_PollEvent �Լ��� SDL_Event�� ������, �� ť���� �� ���ŵ� �̺�Ʈ�� ���� ���� ���ڷ� ����
	{
		// ���⼭ ���� Ÿ���� �̺�Ʈ�� �ٷ�
		switch (event.type)
		{
			// ������ �����츦 ������ �ϴ� ���
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// Ű���� �Է����� �����츦 ���� �� �ֵ��� ó�� (keycode = escape)
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

