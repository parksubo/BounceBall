#pragma once
#include "SDL.h"

// x,y 좌표 저장 백터
struct Vector2
{
	float x;
	float y;
};

// Game class
class Game
{
public:
	Game();
	// 초기화
	bool Initialize();
	// 게임이 끝나기 전까지 게임 루프를 실행
	void RunLoop();
	// 게임 종료
	void Shutdown();

private:
	// 게임 루프를 위한 헬퍼 함수
	void ProcessInput();
	void UpdateGame();
	void GenarateOutput();

	
	SDL_Window* mWindow;	// SDL이 생성한 윈도우
	SDL_Renderer* mRenderer;	// 렌더러
	Uint32 mTicksCount;	//

	bool mIsRunning;	//게임이 계속 실행되어야 하는지 판단
	

	int mPaddleDir;		// 패들 방향
	Vector2 mPaddlePos;	// 패들 위치
	Vector2 mBallPos;	// 공 위치	
	Vector2 mBallVel;	// 공 속력

};
