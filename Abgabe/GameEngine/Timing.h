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

		float m_fps;
		float m_maxFPS;
		float m_frameTime;
		unsigned int m_startTicks;
		};


	}