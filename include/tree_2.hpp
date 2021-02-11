#pragma once
#include "vec2.hpp"
#include "pinned_segment.hpp"


namespace v2
{
	struct PhysicSegment
	{
		Vec2 attach_point;
		Vec2 direction;
		Particule moving_point;
		float length;
		float delta_angle;
		float last_angle;

		PhysicSegment() = default;

		PhysicSegment(Vec2 attach, Vec2 moving)
			: attach_point(attach)
			, direction((moving - attach).getNormalized())
			, moving_point(moving)
			, length((moving - attach).getLength())
			, delta_angle(0.0f)
			, last_angle(direction.getAngle())
		{

		}

		void translate(const Vec2& v)
		{
			attach_point += v;
			moving_point.position += v;
			moving_point.old_position += v;
		}

		void updateDeltaAngle()
		{
			const float new_angle = (moving_point.position - attach_point).getAngle();
			delta_angle = new_angle - last_angle;
			last_angle = new_angle;
		}

		void solveAttach()
		{
			const Vec2 delta = moving_point.position - attach_point;
			const float dist_delta = length - delta.getLength();
			moving_point.move(delta.getNormalized() * dist_delta);
		}

		void update(float dt)
		{
			moving_point.acceleration += direction;
			solveAttach();
			moving_point.update(dt);
			updateDeltaAngle();
		}
	};

	struct Node
	{
		Vec2 position;
		Vec2 last_position;
		Vec2 direction;
		uint32_t index;
		float length;
		float width;

		// Connected branch
		uint32_t branch_id;

		Node()
			: index(0)
			, width(1.0f)
			, branch_id(0)
		{}

		Node(const Vec2& pos, const Vec2& dir, uint32_t i, float l, float w, uint32_t connected_branch = 0)
			: position(pos)
			, last_position(pos)
			, direction(dir)
			, index(i)
			, length(l)
			, width(w)
			, branch_id(connected_branch)
		{}

		void resetPositionDelta()
		{
			last_position = position;
		}

		Vec2 getDelta() const
		{
			return position - last_position;
		}

		Vec2 getEnd() const
		{
			return position + direction * length;
		}
	};

	struct NodeRef
	{
		uint32_t branch_id;
		uint32_t node_id;
	};

	struct Leaf
	{
		PhysicSegment segment;
		NodeRef attact;
	};

	struct Branch
	{
		std::vector<Node> nodes;
		uint32_t level;
		// Physics
		PhysicSegment segment;
		Vec2 target_direction;

		Branch()
			: level(0)
		{}

		Branch(const Node& node, uint32_t lvl)
			: nodes{node}
			, level(lvl)
		{}

		void update(float dt)
		{
			segment.update(dt);
		}

		void translate(const Vec2& v)
		{
			segment.translate(v);
			for (Node& n : nodes) {
				n.position += v;
			}
		}

		void finalizeUpdate()
		{
			for (Node& n : nodes) {
				n.resetPositionDelta();
			}
		}

		void rotateTargetDir(const RotMat2& mat)
		{
			target_direction.rotate(mat);
		}

		void initializePhysics()
		{
			segment = PhysicSegment(nodes.front().position, nodes.back().position);
			const float joint_strength(1000.0f * std::pow(0.8f, level));
			segment.direction = segment.direction * joint_strength;
		}
	};

	struct Tree
	{
		std::vector<Branch> branches;

		Tree()
		{}

		void update(float dt)
		{
			// Physics
			for (Branch& b : branches) {
				b.update(dt);
			}
			// Apply resulting rotations
			for (Branch& b : branches) {
				rotateBranchTarget(b);
			}
			// Apply resulting translations
			translateBranch(branches.front(), Vec2());
			// Finalize update
			for (Branch& b : branches) {
				b.finalizeUpdate();
			}
		}

		void rotateBranchTarget(Branch& b)
		{
			const RotMat2 mat(b.segment.delta_angle);
			const Vec2 origin = b.nodes.front().position;
			for (Node& n : b.nodes) {
				n.position.rotate(origin, mat);
				if (n.branch_id) {
					branches[n.branch_id].rotateTargetDir(mat);
				}
			}
		}

		void translateBranch(Branch& b, const Vec2& v)
		{
			b.translate(v);
			for (Node& n : b.nodes) {
				if (n.branch_id) {
					const Vec2 delta = n.getDelta();
					translateBranch(branches[n.branch_id], delta);
				}
			}
		}

		uint64_t getNodesCount() const
		{
			uint64_t res = 0;
			for (const Branch& b : branches) {
				res += b.nodes.size();
			}
			return res;
		}

		void generateSkeleton()
		{
			for (Branch& b : branches) {
				b.initializePhysics();
			}
		}
	};
}
