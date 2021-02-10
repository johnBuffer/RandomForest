#pragma once
#include "vec2.hpp"


namespace v2
{
	struct PhysicSegment
	{
		Vec2 attach_point;
		Vec2 moving_point;
		Vec2 velocity;
		Vec2 acceleration;
		Vec2 direction;
		float length;
		float delta_angle;
		float last_angle;

		PhysicSegment(Vec2 attach, Vec2 moving)
			: attach_point(attach)
			, moving_point(moving)
			, velocity(0.0f, 0.0f)
			, acceleration(0.0f, 0.0f)
			, direction((moving - attach).getNormalized())
			, length((moving - attach).getLength())
			, delta_angle(0.0f)
			, last_angle(direction.getAngle())
		{

		}

		void applyConstraint()
		{
			const Vec2 dir_vec = direction.getNormal();
			velocity = dir_vec * velocity.dot(dir_vec);
		}

		void correctShift()
		{
			moving_point = attach_point + direction * length;
		}

		void updateDirection()
		{
			direction = (moving_point - attach_point).getNormalized();
		}

		void updateDeltaAngle()
		{
			const float new_angle = direction.getAngle();
			delta_angle = new_angle - last_angle;
			last_angle = new_angle;
		}

		void update(float dt)
		{
			const Vec2 gravity(0.0f, 1000.0f);
			acceleration = gravity;
			velocity += acceleration * dt;
			applyConstraint();
			moving_point += velocity * dt;
			// Post update computations
			updateDirection();
			correctShift();
			updateDeltaAngle();
		}

	};

	struct Node
	{
		Vec2 position;
		uint32_t branch_id;
	};

	struct Branch
	{
		std::vector<Node> nodes;
		PhysicSegment segment;
		Vec2 target_direction;

		void update(float dt)
		{
			segment.update(dt);
		}

		void rotate(const RotMat2& mat)
		{
			const Vec2 origin = nodes.front().position;
			for (Node& n : nodes) {
				n.position.rotate(origin, mat);
			}
		}

		void finalize()
		{
			segment = PhysicSegment();
		}
	};

	struct Tree
	{
		std::vector<Branch> branches;

		Tree()
		{

		}

		void update(float dt)
		{
			for (Branch& b : branches) {
				b.update(dt);
			}
		}

		void rotateBranch(Branch& b)
		{
			const RotMat2 mat(b.segment.delta_angle);
			b.rotate(mat);
			for (const Node& n : b.nodes) {
				if (n.branch_id) {
					branches[n.branch_id].rotate(mat);
				}
			}
		}
	};
}
