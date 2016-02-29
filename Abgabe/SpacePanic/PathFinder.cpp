#include "PathFinder.h"


//Location east = Location{ 1, 0 };
//Location south =  Location{ 0, -1 };
//Location west = Location{ -1, 0 };
//Location north = Location{ 0, 1 };
std::array<SquareGrid::Location, 4> SquareGrid::DIRS2{ { Location{ 1, 0 }, Location{ 0, -1 }, Location{ -1, 0 }, Location{ 0, 1 } } };

PathFinder::PathFinder()
{
}


PathFinder::~PathFinder()
{
}

//template<typename SquareGrid>
//std::vector<glm::vec2> PathFinder::determinePathTo(Level& level, std::vector<Monster*>& monsters, glm::vec2 startP, glm::vec2 goalP){
//
//	SquareGrid::Location start = std::tie(startP.x, startP.y);
//	SquareGrid::Location goal = std::tie(goalP.x, goalP.y);
//
//	std::vector<glm::vec2> result;
//
//	std::vector<LevelNode> levelMap = level.getLevelMap();
//	std::unordered_map<typename SquareGrid::Location, typename SquareGrid::Location> came_from;
//	std::unordered_map<typename SquareGrid::Location, int> cost_so_far;
//
//	a_star_search(level.getMap(), start, goal, came_from, cost_so_far);
//	draw_grid(level.getMap(), 2, nullptr, &came_from);
//	std::cout << std::endl;
//	draw_grid(level.getMap(), 3, &cost_so_far, nullptr);
//	std::cout << std::endl;
//
//	std::vector<SquareGrid::Location> path = reconstruct_path(start, goal, came_from);
//	draw_grid(level.getMap(), 3, nullptr, nullptr, &path);
//
//	result = reconstruct_path2(start, goal, came_from);
//	return result;
//}



int PathFinder::heuristic(SquareGrid::Location a, SquareGrid::Location b) {
	int x1, y1, x2, y2;
	std::tie(x1, y1) = a;
	std::tie(x2, y2) = b;
	return abs(x1 - x2) + abs(y1 - y2);
}


std::unordered_map<SquareGrid::Location, SquareGrid::Location>& PathFinder::a_star_search
(const SquareGrid& squareGrid,
SquareGrid::Location start,
SquareGrid::Location goal,
unordered_map< SquareGrid::Location, SquareGrid::Location>& came_from,
unordered_map<SquareGrid::Location, int>& cost_so_far)
{
	PriorityQueue<SquareGrid::Location> frontier;
	frontier.put(start, 0);

	came_from[start] = start;
	cost_so_far[start] = 0;

	while (!frontier.empty()) {
		auto current = frontier.get();

		if (current == goal) {
			break;
		}

		for (auto next : squareGrid.neighbors(current)) {
			int new_cost = cost_so_far[current] + squareGrid.cost(current, next);
			if (!cost_so_far.count(next) || new_cost < cost_so_far[next]) {
				cost_so_far[next] = new_cost;
				int priority = new_cost + heuristic(next, goal);
				frontier.put(next, priority);
				came_from[next] = current;
			}
		}
	}

	return came_from;
}



std::vector<SquareGrid::Location> PathFinder::reconstruct_path(
	SquareGrid::Location start,
	SquareGrid::Location goal,
	std::unordered_map<SquareGrid::Location, SquareGrid::Location>& came_from
	) {
	std::vector<SquareGrid::Location> path;
	SquareGrid::Location current = goal;
	path.push_back(current);
	while (current != start) {
		current = came_from[current];
		path.push_back(current);
	}
	std::reverse(path.begin(), path.end());
	return path;
}


std::vector<glm::vec2> PathFinder::reconstruct_path2(
	SquareGrid::Location start,
	SquareGrid::Location goal,
	std::unordered_map<SquareGrid::Location, SquareGrid::Location>& came_from
	) {
	std::vector<glm::vec2> path;
	SquareGrid::Location current = goal;
	int x, y;
	std::tie(x, y) = current;
	path.push_back(glm::vec2(x, y));
	while (current != start) {
		current = came_from[current];
		int x2, y2;
		std::tie(x2, y2) = current;
		path.push_back(glm::vec2(x2,y2));
	}
	std::reverse(path.begin(), path.end());
	return path;
}


void PathFinder::draw_grid(const SquareGrid& graph, int field_width,
	std::unordered_map<SquareGrid::Location, int>* distances,
	std::unordered_map<SquareGrid::Location, SquareGrid::Location>* point_to,
	std::vector<SquareGrid::Location>* path) {
	for (int y = 0; y != graph.height; ++y) {
		for (int x = 0; x != graph.width; ++x) {
			SquareGrid::Location id{ x, y };
			std::cout << std::left << std::setw(field_width);
			if (graph.walls.count(id)) {
				std::cout << string(field_width, '#');
			}
			else if (point_to != nullptr && point_to->count(id)) {
				int x2, y2;
				tie(x2, y2) = (*point_to)[id];
				// TODO: how do I get setw to work with utf8?
				if (x2 == x + 1) { std::cout << "\u2192 "; }
				else if (x2 == x - 1) { std::cout << "\u2190 "; }
				else if (y2 == y + 1) { std::cout << "\u2193 "; }
				else if (y2 == y - 1) { std::cout << "\u2191 "; }
				else { std::cout << "* "; }
			}
			else if (distances != nullptr && distances->count(id)) {
				std::cout << (*distances)[id];
			}
			else if (path != nullptr && find(path->begin(), path->end(), id) != path->end()) {
				std::cout << '@';
			}
			else {
				std::cout << '.';
			}
		}
		std::cout << std::endl;
	}
}

std::basic_iostream<char>::basic_ostream& operator<<(std::basic_iostream<char>::basic_ostream& out, std::tuple<int, int> loc) {
	int x, y;
	std::tie(x, y) = loc;
	out << '(' << x << ',' << y << ')';
	return out;
}