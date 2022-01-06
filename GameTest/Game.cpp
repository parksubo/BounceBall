#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

Game::Game() : 
	mWindow(nullptr),
	mRenderer(nullptr),
	mTicksCount(0),
	mIsRunning(true),
	mPaddleDir(0)
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
		0		// 플래그 (0은 어떠한 플래그도 설정되지 않음을 의미)
		//SDL_WINDOW_RESIZABLE	// 윈도우 크기 조절 가능 플래그
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create windows: %s", SDL_GetError());
		return false;
	}

	// Create SDL Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,	// 렌더링을 위해 생성한 윈도우
		-1,			// 게임이 여러 윈도우를 가질때만 의미있음, 하나의 윈도우만 생성한다면 SDL이
					// 그래픽 카드를 결정하라는 의미로 기본값 -1씀
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC	// 그래픽 하드웨어 사용 플래그 | Vsync사용 플래그
	);

	// paddle, ball 위치 초기화
	mPaddlePos.x = 10.0f;
	mPaddlePos.y = 768.0f / 2.0f;
	mBallPos.x = 1024.0f / 2.0f;
	mBallPos.y = 768.0f / 2.0f;
	mBallVel.x = -200.0f;
	mBallVel.y = 235.0f;
	return true;

}

void Game::Shutdown()
{
	// SDL_Window 객체 해제
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	// mIsRunning이 false가 될 때 까지 게임루프를 반복해서 실행
	while (mIsRunning)
	{
		ProcessInput();
		UpdateGame();
		GenarateOutput();
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

	// 패들 위치를 위(W)아래(S) 키를 이용해 이동
	mPaddleDir = 0;
	if (state[SDL_SCANCODE_W])
	{
		mPaddleDir -= 1;	
	}
	if (state[SDL_SCANCODE_S])
	{
		mPaddleDir += 1;
	}

}


void Game::UpdateGame()
{
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// 목표 deltatime 16ms 를 보장 (프레임제한)
	// 델타 시간은 마지막 프레임 틱값과 현재 프레임 틱값의 차
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;	// SDL_GetTicks는 SDL_Init 함수 호출 이후 경과된 시간(ms)를 반환	
	// 최대 델타 시간값으로 고정
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();	// 다음 프레임을 위해 틱값을 갱신

	// 패들 위치 업데이트
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 500.0f * deltaTime;	// 방향 * 속도 * 델타시간
		// 패들이 화면 밖으로 넘어가지 않기 위함
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - (paddleH / 2.0f + thickness)) )
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// 공 위치 업데이트
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// 공이 상단 벽과 부딪힐 때
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1.0;	// 공의 y위치 반전
	}
	// 공이 하단 벽과 부딪힐 때
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1.0;	// 공의 y위치 반전
	}
	// 공이 우측 벽과 부딪힐 때
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f; // 공의 x위치 반전
	}
	// 공이 스크린 밖으로 나갔을 때
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;	// 게임오버
	}

	// 패들과 공의 y값의 차의 절댓값 계산
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;

	if (
		// 차의 절반이 패들의 절반보다 작고
		diff <= paddleH / 2.0f &&
		// 공의 x위치가 패들과 나란하고
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// 공이 왼쪽으로 움직이고 있다면
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;	// 공의 x위치 반전
	}

}


void Game::GenarateOutput()
{
	// 배경 색 지정
	SDL_SetRenderDrawColor(
		mRenderer,
		0,	// R
		0,	// G
		255,// B
		255	// A
	);
	SDL_RenderClear(mRenderer);	 // 현재 그리기 색상으로 후면버퍼 지우기

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);	// wall들의 색상 지정
	// 사각형 wall 구조체
	SDL_Rect wall{
		0,	// 왼쪽 상단x
		0,	// 왼쪽 상단y
		1024,	// 너비
		thickness // 높이
	};
	SDL_RenderFillRect(mRenderer, &wall);	// top wall 그리기 

	// y값만 바꿔서 bottom wall 그리기
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// 오른쪽 wall 그리기
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);	// ball의 색상 지정
	// ball 그리기
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);	// paddle의 색상 지정
	// paddle 그리기
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);	// 전면 후면 버퍼 교환
}