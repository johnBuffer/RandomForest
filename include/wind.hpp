#pragma once


struct Wind
{
	float width;
	float strength;
	float pos_x;
	float speed;

	Wind(float w, float force, float spd)
		: width(w)
		, strength(force)
		, speed(spd)
		, pos_x(-w*0.5f)
	{

	}

	void update(float dt, float max_x)
	{
		pos_x += speed * dt;
		if (pos_x - width * 0.5f > max_x) {
			pos_x = -width * 0.5f;
		}
	}

	bool isOver(const Vec2& pos) const
	{
		return (pos.x > pos_x - width * 0.5f) && (pos.x < pos_x + width * 0.5f);
	}
};
