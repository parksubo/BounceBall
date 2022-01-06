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
		0		// �÷��� (0�� ��� �÷��׵� �������� ������ �ǹ�)
		//SDL_WINDOW_RESIZABLE	// ������ ũ�� ���� ���� �÷���
	);
	if (!mWindow)
	{
		SDL_Log("Failed to create windows: %s", SDL_GetError());
		return false;
	}

	// Create SDL Renderer
	mRenderer = SDL_CreateRenderer(
		mWindow,	// �������� ���� ������ ������
		-1,			// ������ ���� �����츦 �������� �ǹ�����, �ϳ��� �����츸 �����Ѵٸ� SDL��
					// �׷��� ī�带 �����϶�� �ǹ̷� �⺻�� -1��
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC	// �׷��� �ϵ���� ��� �÷��� | Vsync��� �÷���
	);

	// paddle, ball ��ġ �ʱ�ȭ
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
	// SDL_Window ��ü ����
	SDL_DestroyWindow(mWindow);
	SDL_DestroyRenderer(mRenderer);
	SDL_Quit();
}

void Game::RunLoop()
{
	// mIsRunning�� false�� �� �� ���� ���ӷ����� �ݺ��ؼ� ����
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

	// �е� ��ġ�� ��(W)�Ʒ�(S) Ű�� �̿��� �̵�
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
	while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));	// ��ǥ deltatime 16ms �� ���� (����������)
	// ��Ÿ �ð��� ������ ������ ƽ���� ���� ������ ƽ���� ��
	float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;	// SDL_GetTicks�� SDL_Init �Լ� ȣ�� ���� ����� �ð�(ms)�� ��ȯ	
	// �ִ� ��Ÿ �ð������� ����
	if (deltaTime > 0.05f)
	{
		deltaTime = 0.05f;
	}
	mTicksCount = SDL_GetTicks();	// ���� �������� ���� ƽ���� ����

	// �е� ��ġ ������Ʈ
	if (mPaddleDir != 0)
	{
		mPaddlePos.y += mPaddleDir * 500.0f * deltaTime;	// ���� * �ӵ� * ��Ÿ�ð�
		// �е��� ȭ�� ������ �Ѿ�� �ʱ� ����
		if (mPaddlePos.y < (paddleH / 2.0f + thickness))
		{
			mPaddlePos.y = paddleH / 2.0f + thickness;
		}
		else if (mPaddlePos.y > (768.0f - (paddleH / 2.0f + thickness)) )
		{
			mPaddlePos.y = 768.0f - paddleH / 2.0f - thickness;
		}
	}

	// �� ��ġ ������Ʈ
	mBallPos.x += mBallVel.x * deltaTime;
	mBallPos.y += mBallVel.y * deltaTime;

	// ���� ��� ���� �ε��� ��
	if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
	{
		mBallVel.y *= -1.0;	// ���� y��ġ ����
	}
	// ���� �ϴ� ���� �ε��� ��
	else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
	{
		mBallVel.y *= -1.0;	// ���� y��ġ ����
	}
	// ���� ���� ���� �ε��� ��
	else if (mBallPos.x >= (1024.0f - thickness) && mBallVel.x > 0.0f)
	{
		mBallVel.x *= -1.0f; // ���� x��ġ ����
	}
	// ���� ��ũ�� ������ ������ ��
	else if (mBallPos.x <= 0.0f)
	{
		mIsRunning = false;	// ���ӿ���
	}

	// �е�� ���� y���� ���� ���� ���
	float diff = mPaddlePos.y - mBallPos.y;
	diff = (diff > 0.0f) ? diff : -diff;

	if (
		// ���� ������ �е��� ���ݺ��� �۰�
		diff <= paddleH / 2.0f &&
		// ���� x��ġ�� �е�� �����ϰ�
		mBallPos.x <= 25.0f && mBallPos.x >= 20.0f &&
		// ���� �������� �����̰� �ִٸ�
		mBallVel.x < 0.0f)
	{
		mBallVel.x *= -1.0f;	// ���� x��ġ ����
	}

}


void Game::GenarateOutput()
{
	// ��� �� ����
	SDL_SetRenderDrawColor(
		mRenderer,
		0,	// R
		0,	// G
		255,// B
		255	// A
	);
	SDL_RenderClear(mRenderer);	 // ���� �׸��� �������� �ĸ���� �����

	SDL_SetRenderDrawColor(mRenderer, 0, 255, 0, 255);	// wall���� ���� ����
	// �簢�� wall ����ü
	SDL_Rect wall{
		0,	// ���� ���x
		0,	// ���� ���y
		1024,	// �ʺ�
		thickness // ����
	};
	SDL_RenderFillRect(mRenderer, &wall);	// top wall �׸��� 

	// y���� �ٲ㼭 bottom wall �׸���
	wall.y = 768 - thickness;
	SDL_RenderFillRect(mRenderer, &wall);

	// ������ wall �׸���
	wall.x = 1024 - thickness;
	wall.y = 0;
	wall.w = thickness;
	wall.h = 1024;
	SDL_RenderFillRect(mRenderer, &wall);

	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 255);	// ball�� ���� ����
	// ball �׸���
	SDL_Rect ball{
		static_cast<int>(mBallPos.x - thickness / 2),
		static_cast<int>(mBallPos.y - thickness / 2),
		thickness,
		thickness
	};
	SDL_RenderFillRect(mRenderer, &ball);

	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);	// paddle�� ���� ����
	// paddle �׸���
	SDL_Rect paddle{
		static_cast<int>(mPaddlePos.x),
		static_cast<int>(mPaddlePos.y - paddleH / 2),
		thickness,
		static_cast<int>(paddleH)
	};
	SDL_RenderFillRect(mRenderer, &paddle);

	SDL_RenderPresent(mRenderer);	// ���� �ĸ� ���� ��ȯ
}