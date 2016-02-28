#pragma once
#include "Agent.h"
#include "Player.h"
//#include "LevelNode.h"
#include "PathFinder.h"


//the compiler wouldn't let me use the code above
//class PriorityQueue
//{
//public:
//	
//	inline bool empty() const { return elements.empty(); }
//
//	inline void put(LevelNode item, int priority){
//		elements.emplace(priority, item);
//	}
//
//	inline LevelNode get() {
//		LevelNode best_item = elements.top().second;
//		elements.pop();
//		return best_item;
//	}
//
//private:
//	typedef std::pair<int, LevelNode> PQElement;
//	std::priority_queue<PQElement, std::vector<PQElement>, std::greater<PQElement>> elements;
//};






class Monster :
	public Agent
{
public:
	Monster();
	~Monster();

	void init(float speed, glm::vec2 position, const glm::vec2 drawDims, const glm::vec2 collisionDims);

	virtual void update(const std::vector<std::string>& levelData,
		std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;
	virtual void update(std::vector<Box>& levelBoxes, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	virtual void update(Level& level, std::vector<Player*>& players, std::vector<Monster*>& monsters, float deltaTime) override;

	virtual bool collideWithHalfHole(std::vector<Box>& levelBoxes) override;
	virtual bool collideWithHole(std::vector<Box>& levelBoxes) override;

	void setDirection(glm::vec2 newDirection);

	void draw(GameEngine::SpriteBatch& spriteBatch);

	void kill(Player* killedBy);

	float getHealth() override {
		return m_health;
	};

	bool isInHole() const {
		return m_inHole;
	}
	bool isInHalfHole() const {
		return m_inHalfHole;
	}

	Box& getHole() const {
		return m_holeBox;
	}

	bool isAlive(){
		return !m_died;
	}

	Player* getKiller() const { return m_killedBy; }

	int getHolteCounter() const { return m_inHoleCounter; }

	bool sawPlayer() const { return m_sawPlayer; }

	void deactivateSawPlayer() {
		m_sawPlayer = false; 
		m_futurePath.clear();
	}


private:

	Player* getNearestPlayer(std::vector<Player*>& Player);
	void changeDirectionTo(Box& box);
	std::vector<glm::vec2> determinePathToPlayer(Level& level, std::vector<Monster*>& monsters, Player& player);
	std::vector<glm::vec2> determinePathTo(Level& level, std::vector<Monster*>& monsters, glm::vec2 startP, glm::vec2 goalP);
	bool changeDirectionToFuturePath();

	//std::unordered_map<LevelNode, LevelNode>& useAStarSearch(
	//	std::vector<LevelNode>& levelMap, 
	//	std::vector<Monster*>& monsters, 
	//	LevelNode& start, 
	//	LevelNode& goal, 
	//	std::unordered_map<LevelNode, LevelNode>& came_from,
	//	std::unordered_map<LevelNode, int>& cost_so_far);

	//std::vector<LevelNode> reconstruct_path(
	//	LevelNode& start,
	//	LevelNode& goal,
	//	std::unordered_map<LevelNode, LevelNode>& came_from);

	//int heuristic(glm::vec2 a, glm::vec2 b);

	int m_directionSteps = -1;
	bool m_sawPlayer = false;
	bool m_wasOnLadder = false;
	bool m_inHole = false;
	bool m_inHalfHole = false;
	int m_inHoleCounter = 0;
	Box& m_holeBox = Box();
	//m_holeBox.init(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f), GameEngine::ResourceManager::getTexture("Textures/red_bricks.png"), GameEngine::ColorRGBA8(255, 255, 255, 255));
	bool m_died = false;
	Player* m_killedBy;
	bool m_inAir = false;
	std::vector<glm::vec2> m_futurePath;
	bool m_calculatedNewPath = false;
    bool m_reachedNextStep = true;
	PathFinder pathfinder = PathFinder();
	float m_animTime = 0.0f;
};

