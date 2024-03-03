#ifndef TIMER__H
#define TIMER__H

#define DLL_EXPORT __declspec(dllexport)

class DLL_EXPORT Timer
{
public:
	Timer();
	void Start();
	float Read();
	float Read_Sec();

	void Pause();
	void Resume();

	void Reset();

	bool paused = false;

private:
	int started_at = 0;
	int time_paused = 0;
	int paused_at = 0;
	int resumed_at = 0;

};

#endif // !TIMER__H
