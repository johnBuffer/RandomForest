#pragma once
#include "tree2.hpp"
#include <SFML/Graphics.hpp>


struct TreeRenderer
{
	static void render(const Tree2& tree, sf::RenderTarget& target)
	{
		sf::VertexArray va(sf::Lines, 0);
		for (const Tree2::Branch& branch : tree.branches) {
			const auto& nodes = branch.nodes;
			const int64_t nodes_count = nodes.size();
			for (int64_t i(0); i < nodes_count - 1; ++i) {
				const Tree2::Node& node_1 = tree.getNode(nodes[i]);
				const Tree2::Node& node_2 = tree.getNode(nodes[i + 1]);
				va.append(sf::Vertex(sf::Vector2f(node_1.position.x, node_1.position.y)));
				va.append(sf::Vertex(sf::Vector2f(node_2.position.x, node_2.position.y)));
			}
		}

		target.draw(va);
	}
};
