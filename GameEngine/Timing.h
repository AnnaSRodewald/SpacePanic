#pragma once


namespace GameEngine{

	class FpsLimiter{
	public:
		FpsLimiter();
		void init(float maxFPS);

		void setMaxFPS(float maxFPS);

		void beginFrame();

		//end will return the current FPS
		float end();

	private:
		void calculateFPS();

		float _fps;
		float _maxFPS;
		float _frameTime;
		unsigned int _startTicks;
		};


	}