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
	// SDL_Init이 0이 아니면 초기화가 실패 -> false 반환
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL : %s", SDL_GetError());
		return false;
	}

	// Create SDL
	mWindow = SDL_CreateWindow(
		"Game Programming in C++ (Test1)",	// 윈도우 제목
		100,	// 윈도우의 좌측 상단 x좌표
		100,	// 윈도우의 좌측 상단 y좌표
		1024,	// 윈도우의 너비
		768,	// 윈도우 높이
		//0		// 플래그 (0은 어떠한 플래그도 설정되지 않음을 의미)
		SDL_WINDOW_RESIZABLE
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create windows: %s", SDL_GetError());
		return false;
	}

	// Create SDL Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,	// 렌더링을 위해 생성한 윈도우
		-1,			// 일반적으로 -1씀
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	)


}

void Game::Shutdown()
{
	// SDL_Window 객체 해제
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void Game::RunLoop()
{
	// mIsRunning이 false가 될 때 까지 게임루프를 반복해서 실행
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
	// 큐에 여전히 이벤트가 남아 있는 동안
	while (SDL_PollEvent(&event))	// SDL_PollEvent 함수는 SDL_Event의 포인터, 즉 큐에서 막 제거된 이벤트에 관한 정보 인자로 받음
	{
		// 여기서 여러 타입의 이벤트를 다룸
		switch (event.type)
		{
			// 유저가 윈도우를 닫으려 하는 경우
			case SDL_QUIT:
				mIsRunning = false;
				break;
		}
	}

	// 키보드 입력으로 윈도우를 닫을 수 있도록 처리 (keycode = escape)
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_ESCAPE])
	{
		mIsRunning = false;
	}
}

