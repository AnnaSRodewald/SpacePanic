#pragma once
#include "Agent.h"
#include "Player.h"

// got the code from http://www.redblobgames.com/pathfinding/a-star/implementation.html 
template<typename T, typename Number = int>
struct PriorityQueue {
	typedef pair<Number, T> PQElement;
	priority_queue<PQElement, vector<PQElement>,
		std::greater<PQElement >> elements;

	inline bool empty() const { return elements.empty(); }

	inline void put(T item, Number priority) {
		elements.emplace(priority, item);
	}

	inline T get() {
		T best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};



class Monster :
	public Agent
{
public:
	Monster();
	~Monster();

	void init(float speed, glm::vec2 position, glm::vec2 dimensions);

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


private:

	Player* getNearestPlayer(std::vector<Player*>& Player);
	void changeDirectionTo(Box& box);
	std::vector<Node&> determinePathToPlayer(Level& level, std::vector<Monster*>& monsters, Player& player);

	std::unordered_map<Node&, Node&> useAStarAgl(
		std::vector<Node&> levelMap, 
		std::vector<Monster*>& monsters, 
		Node& start, 
		Node& goal, 
		std::unordered_map<Node&, Node&>& came_from,
		std::unordered_map<Node&, int>& cost_so_far);

	std::vector<Node&> reconstruct_path(
		Node& start,
		Node& goal,
		std::unordered_map<Node&, Node&>& came_from);

	int heuristic(glm::vec2 a, glm::vec2 b);

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
	std::vector<Node&> m_futurePath;
};

