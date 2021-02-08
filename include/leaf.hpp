#pragma once
#include "vec2.hpp"
#include "number_generator.hpp"
#include "pinned_segment.hpp"


struct Leaf
{
	Node::Ptr attach;
	Vec2 last_attach_pos;

	Particule free_particule;
	Particule broken_part;

	Vec2 target_direction;
	Vec2 acceleration;
	float joint_strenght;

	sf::Color color;
	float cut_threshold;
	float size;

	Leaf(Node::Ptr root, const Vec2& dir)
		: attach(root)
		, last_attach_pos(root->pos)
		, free_particule(root->pos + dir)
		, target_direction(dir)
		, joint_strenght(RNGf::getRange(1.0f, 2.0f))
		, cut_threshold(0.3f + RNGf::getUnder(0.8f))
		, size(1.0f)
	{
		color = sf::Color::Black;// sf::Color(255, static_cast<uint8_t>(168 + RNGf::getRange(80.0f)), 0);
	}

	void solveAttach()
	{
		const float length = 1.0f;
		const Vec2 delta = free_particule.position - attach->pos;
		const float dist_delta = 1.0f - delta.getLength();
		if (std::abs(dist_delta) > cut_threshold) {
			cut();
		}
			
		free_particule.move(delta.getNormalized() * dist_delta);
	}

	void solveLink()
	{
		const float length = 1.0f;
		const Vec2 delta = free_particule.position - broken_part.position;
		const float dist_delta = 1.0f - delta.getLength();
		free_particule.move(delta.getNormalized() * (0.5f * dist_delta));
		broken_part.move(delta.getNormalized() * (-0.5f * dist_delta));
	}

	Vec2 getDir() const
	{
		if (attach) {
			return free_particule.position - attach->pos;
		}
		
		return free_particule.position - broken_part.position;
	}

	Vec2 getPosition() const
	{
		if (attach) {
			return attach->pos;
		}
		return broken_part.position;
	}

	void translate()
	{
		const Vec2 delta_pos = attach->pos - last_attach_pos;
		free_particule.position += delta_pos;
		free_particule.old_position += delta_pos;
		last_attach_pos = attach->pos;
	}

	void cut()
	{
		broken_part = Particule(attach->pos);
		attach = nullptr;
		target_direction = Vec2(0.0f, 1.0f);
	}

	void applyWind(const Wind& wind)
	{
		const float ratio = (1.0f - std::abs(wind.pos_x - free_particule.position.x) / (0.5f * wind.width));
		const float wind_force = wind.strength * (wind.speed ? ratio : 1.0f);
		free_particule.acceleration += Vec2(1.0f, RNGf::getRange(2.0f)) * wind_force;
		if (!attach) {
			broken_part.acceleration += Vec2(1.0f, RNGf::getRange(2.0f)) * wind_force;
		}
	}

	void update(float dt)
	{
		if (attach) {
			translate();
			solveAttach();
		}
		else {
			free_particule.acceleration += Vec2(RNGf::getRange(1.0f), RNGf::getRange(1.0f));
			broken_part.acceleration += Vec2(RNGf::getRange(1.0f), RNGf::getRange(1.0f));
			solveLink();
		}

		free_particule.update(dt);
		free_particule.acceleration = target_direction * joint_strenght;
		broken_part.update(dt);
		broken_part.acceleration = target_direction * joint_strenght;
	}
};
