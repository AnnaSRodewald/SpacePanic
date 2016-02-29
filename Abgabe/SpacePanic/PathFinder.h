#pragma once

#include <glm\glm.hpp>
#include <iostream>
#include <iomanip>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <vector>
#include <utility>
#include <queue>
#include <tuple>
#include <algorithm>
#include <functional>
#include <string>


using std::unordered_map;
using std::unordered_set;
using std::array;
using std::vector;
using std::queue;
using std::priority_queue;
using std::pair;
using std::tuple;
using std::tie;
using std::string;

//// got the code from www.redblobgames.com/pathfinding/a-star/implementation.html 
// Helpers for SquareGrid::Location

namespace std {
	// I know, this is technically not allowed
	template <>
	struct hash<tuple<int, int> > {
		inline size_t operator()(const tuple<int, int>& location) const {
			int x, y;
			tie(x, y) = location;
			return x * 1812433253 + y;
		}
	};
}



template<typename L>
struct SimpleGraph {
	typedef L Location;
	typedef typename std::vector<Location>::iterator iterator;
	std::unordered_map<Location, std::vector<Location> > edges;

	inline const std::vector<Location>  neighbors(Location id) {
		return edges[id];
	}
};

struct SquareGrid {
	typedef std::tuple<int, int> Location;
	static std::array<Location, 4> DIRS2;

	int width, height;
	std::unordered_set<Location> walls;

	SquareGrid(int width_, int height_)
		: width(width_), height(height_) {}

	inline bool in_bounds(Location id) const {
		int x, y;
		std::tie(x, y) = id;
		return 0 <= x && x < width && 0 <= y && y < height;
	}

	inline bool passable(Location id) const {
		return !walls.count(id);
	}

	inline int cost(Location a, Location b) const {
		return 1;
	}

	std::vector<Location> neighbors(Location id) const {
		int x, y, dx, dy;
		std::tie(x, y) = id;
		std::vector<Location> results;

		for (auto dir : DIRS2) {
			std::tie(dx, dy) = dir;
			Location next(x + dx, y + dy);
			if (in_bounds(next) && passable(next)) {
				results.push_back(next);
			}
		}

		if ((x + y) % 2 == 0) {
			// aesthetic improvement on square grids
			std::reverse(results.begin(), results.end());
		}

		return results;
	}
};


template<typename T, typename Number = int>
struct PriorityQueue {
	typedef std::pair<Number, T> PQElement;
	std::priority_queue<PQElement, std::vector<PQElement>,
		std::greater<PQElement> > elements;

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


class PathFinder

{
public:
	PathFinder();
	~PathFinder();


	
	int heuristic(SquareGrid::Location a, SquareGrid::Location b);

	
	std::unordered_map<SquareGrid::Location, SquareGrid::Location>& a_star_search
		(const SquareGrid& squareGrid,
		 SquareGrid::Location start,
		SquareGrid::Location goal,
		unordered_map< SquareGrid::Location, SquareGrid::Location>& came_from,
		unordered_map<SquareGrid::Location, int>& cost_so_far);

	std::vector<SquareGrid::Location> reconstruct_path(
		SquareGrid::Location start,
		SquareGrid::Location goal,
		unordered_map<SquareGrid::Location, SquareGrid::Location>& came_from
		);

	
	std::vector<glm::vec2> reconstruct_path2(
		SquareGrid::Location start,
		SquareGrid::Location goal,
		std::unordered_map<SquareGrid::Location, SquareGrid::Location>& came_from
		);

	void draw_grid(const SquareGrid& graph, int field_width,
		std::unordered_map<SquareGrid::Location, int>* distances = nullptr,
		std::unordered_map<SquareGrid::Location, SquareGrid::Location>* point_to = nullptr,
		std::vector<SquareGrid::Location>* path = nullptr);

};

