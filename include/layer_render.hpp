#pragma once
#include <SFML/Graphics.hpp>
#include "tree.hpp"
#include "grass/grass.hpp"


struct LayerRender
{
	sf::VertexArray branches_va;
	sf::VertexArray leaves_va;
	sf::VertexArray grass_va;

	LayerRender()
		: branches_va(sf::Quads)
		, leaves_va(sf::Quads)
		, grass_va(sf::Quads)
	{
	}

	void init(const std::vector<Tree>& trees, const std::vector<Grass>& grass)
	{
		// Init trees
		uint64_t nodes_count = 0;
		uint64_t leaves_count = 0;
		for (const Tree& t : trees) {
			nodes_count += t.getNodesCount() - t.branches.size();
			leaves_count += t.leaves.size();
		}
		branches_va.resize(4 * nodes_count);
		leaves_va.resize(4 * leaves_count);

		// Init grass
		uint64_t points_count = 0;
		for (const Grass& g : grass) {
			points_count += g.points.size() - 1;
		}
		grass_va.resize(4 * points_count);
	}

	void render(std::vector<Tree>& trees)
	{
		// Branches
		{
			uint64_t global_offset(0);
			for (const Tree& t : trees) {
				for (const Branch& b : t.branches) {
					const uint64_t nodes_count = b.nodes.size() - 1;
					for (uint64_t i(0); i < nodes_count; ++i) {
						// Current node
						{
							const Node& n = *(b.nodes[i]);
							const float width = 0.5f * n.width;
							const Vec2 n_vec = n.growth_direction.getNormal() * width;
							branches_va[global_offset + 4 * i + 0].position = sf::Vector2f(n.pos.x + n_vec.x, n.pos.y + n_vec.y);
							branches_va[global_offset + 4 * i + 1].position = sf::Vector2f(n.pos.x - n_vec.x, n.pos.y - n_vec.y);
						}
						// Next node
						{
							const Node& n = *(b.nodes[i + 1]);
							const float width = 0.5f * n.width;
							const Vec2 n_vec = n.growth_direction.getNormal() * width;
							branches_va[global_offset + 4 * i + 2].position = sf::Vector2f(n.pos.x - n_vec.x, n.pos.y - n_vec.y);
							branches_va[global_offset + 4 * i + 3].position = sf::Vector2f(n.pos.x + n_vec.x, n.pos.y + n_vec.y);
						}
					}
					global_offset += 4 * nodes_count;
				}
			}
		}
		// Leaves
		{
			uint64_t global_offset(0);
			for (const Tree& t : trees) {
				uint64_t i(0);
				const float leaf_length = 30.0f;
				const float leaf_width = 30.0f;
				for (const Leaf& l : t.leaves) {
					const Vec2 leaf_dir = l.getDir().getNormalized();
					const Vec2 dir = leaf_dir * leaf_length;
					const Vec2 nrm = leaf_dir.getNormal() * (0.5f * leaf_width);
					const Vec2 attach = l.getPosition();
					const Vec2 pt1 = attach + nrm;
					const Vec2 pt2 = attach + nrm + dir;
					const Vec2 pt3 = attach - nrm + dir;
					const Vec2 pt4 = attach - nrm;
					// Geometry
					leaves_va[global_offset + 4 * i + 0].position = sf::Vector2f(pt1.x, pt1.y);
					leaves_va[global_offset + 4 * i + 1].position = sf::Vector2f(pt2.x, pt2.y);
					leaves_va[global_offset + 4 * i + 2].position = sf::Vector2f(pt3.x, pt3.y);
					leaves_va[global_offset + 4 * i + 3].position = sf::Vector2f(pt4.x, pt4.y);
					// Texture
					leaves_va[global_offset + 4 * i + 0].texCoords = sf::Vector2f(0.0f, 0.0f);
					leaves_va[global_offset + 4 * i + 1].texCoords = sf::Vector2f(1024.0f, 0.0f);
					leaves_va[global_offset + 4 * i + 2].texCoords = sf::Vector2f(1024.0f, 1024.0f);
					leaves_va[global_offset + 4 * i + 3].texCoords = sf::Vector2f(0.0f, 1024.0f);
					// Color
					leaves_va[global_offset + 4 * i + 0].color = l.color;
					leaves_va[global_offset + 4 * i + 1].color = l.color;
					leaves_va[global_offset + 4 * i + 2].color = l.color;
					leaves_va[global_offset + 4 * i + 3].color = l.color;
					++i;
				}
				global_offset += 4 * i;
			}
		}
	}

	void render(std::vector<Grass>& grass)
	{
		uint64_t global_offset = 0;
		for (const Grass& g : grass) {
			const float initial_width = 1.0f;
			float width = initial_width;
			const uint64_t points_count = g.points.size() - 1;
			// Add points
			for (uint64_t i(0); i < points_count; ++i) {
				const Vec2& current_pt = g.points[i]->coords;
				const Vec2& next_pt = g.points[i + 1]->coords;
				const Vec2 v = (next_pt - current_pt).getNormalized();
				// Geometry
				grass_va[global_offset + 4 * i + 0].position = sf::Vector2f(current_pt.x - width * v.y, current_pt.y + width * v.x);
				grass_va[global_offset + 4 * i + 1].position = sf::Vector2f(current_pt.x + width * v.y, current_pt.y - width * v.x);
				width *= 0.5f;
				grass_va[global_offset + 4 * i + 2].position = sf::Vector2f(next_pt.x + width * v.y, next_pt.y - width * v.x);
				grass_va[global_offset + 4 * i + 3].position = sf::Vector2f(next_pt.x - width * v.y, next_pt.y + width * v.x);
				// Color
				grass_va[global_offset + 4 * i + 0].color = g.color;
				grass_va[global_offset + 4 * i + 1].color = g.color;
				grass_va[global_offset + 4 * i + 2].color = g.color;
				grass_va[global_offset + 4 * i + 3].color = g.color;
			}
			global_offset += 4 * points_count;
		}
	}
};


struct LayerRenderer
{
	sf::RenderTarget& target;
	sf::Texture texture;

	LayerRenderer(sf::RenderTarget& target_)
		: target(target_)
	{
		texture.loadFromFile("../res/leaf.png");
	}

	void render(const LayerRender& layer, sf::RenderStates states)
	{
		target.draw(layer.branches_va, states);
		states.texture = &texture;
		target.draw(layer.leaves_va, states);
	}
};

