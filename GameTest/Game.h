#pragma once
#include "SDL.h"

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

	// SDL�� ������ ������
	SDL_Window* mWindow;

	//������ ��� ����Ǿ�� �ϴ��� �Ǵ�
	bool mIsRunning;
	// ������
	SDL_Renderer* mRenderer;
};
