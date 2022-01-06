#pragma once
#include "SDL.h"

// x,y ��ǥ ���� ����
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
	// �ʱ�ȭ
	bool Initialize();
	// ������ ������ ������ ���� ������ ����
	void RunLoop();
	// ���� ����
	void Shutdown();

private:
	// ���� ������ ���� ���� �Լ�
	void ProcessInput();
	void UpdateGame();
	void GenarateOutput();

	
	SDL_Window* mWindow;	// SDL�� ������ ������
	SDL_Renderer* mRenderer;	// ������
	Uint32 mTicksCount;	//

	bool mIsRunning;	//������ ��� ����Ǿ�� �ϴ��� �Ǵ�
	

	int mPaddleDir;		// �е� ����
	Vector2 mPaddlePos;	// �е� ��ġ
	Vector2 mBallPos;	// �� ��ġ	
	Vector2 mBallVel;	// �� �ӷ�

};
