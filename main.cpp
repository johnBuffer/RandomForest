#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <sstream>
#include <cmath>

const float PI = 3.141592653;


struct Point3D
{
	Point3D() :
		x(0.0f),
		y(0.0f),
		z(0.0f)
	{}

	Point3D(float x, float y, float z) :
		x(x),
		y(y),
		z(z)
	{}

	float get2DLength() const
	{
		return sqrt(x*x + y*y);
	}

	float getLength() const
	{
		return sqrt(x*x + y*y + z*z);
	}

	Point3D get2DNormal() const
	{
		return Point3D(-y, x, 0.0f);
	}

	Point3D get2DNormalized()
	{
		float length2D = get2DLength();
		return Point3D(x / length2D, y / length2D, 0.0f);
	}

	Point3D getProjection() const
	{
		if (z != 0)
			return Point3D(x / z, y / z, 0.0f);

		return *this;
	}
	
	Point3D rotate(float angle) const
	{
		float new_x = x *  cos(angle) + z * sin(angle);
		float new_z = x * -sin(angle) + z * cos(angle);

		return Point3D(new_x, y, new_z);
	}

	float x, y, z;
};

Point3D operator*(float f, const Point3D& p)
{
	return Point3D(f*p.x, f*p.y, f*p.z);
}

Point3D operator/(float f, const Point3D& p)
{
	return Point3D(f/p.x, f/p.y, f/p.z);
}

Point3D operator+(const Point3D& p1, const Point3D& p2)
{
	return Point3D(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z);
}

Point3D operator-(const Point3D& p1, const Point3D& p2)
{
	return Point3D(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}


struct Tree3D
{
	Tree3D() = default;

	Tree3D(float thickness, float factor, const Point3D& position, const Point3D& direction) :
		factor_(factor),
		thickness_(thickness),
		position_(position),
		direction_(direction),
		super_(nullptr)
	{
		
	}

	Tree3D(float thickness, const Point3D& direction, Tree3D* super) :
		thickness_(thickness),
		direction_(direction),
		super_(super)
	{
		if (super)
			factor_ = super->factor_;
	}

	void draw(sf::RenderTarget* target, float angle) const
	{
		sf::VertexArray va(sf::Quads, 0);
		addToVertexArray(va, position_, angle);
		
		target->draw(va);
	}

	void addToVertexArray(sf::VertexArray& va, const Point3D& position, float angle) const
	{
		sf::Vertex v1, v2, v3, v4;

		const Point3D direction = direction_.rotate(angle);
		const Point3D normal = direction.get2DNormal().get2DNormalized();
		const Point3D bot_normal = (0.5f*thickness_) * normal;
		Point3D top_normal = normal;
		if (!subs_.empty())
			top_normal = factor_ * bot_normal;

		Point3D upper_mid = position + direction;
		Point3D upper_l = upper_mid - top_normal;
		Point3D upper_r = upper_mid + top_normal;

		v1.position = sf::Vector2f(position.x - bot_normal.x, position.y - bot_normal.y);
		v2.position = sf::Vector2f(upper_l.x, upper_l.y);
		v3.position = sf::Vector2f(upper_r.x, upper_r.y);
		v4.position = sf::Vector2f(position.x + bot_normal.x, position.y + bot_normal.y);

		v1.color = sf::Color();
		v2.color = sf::Color();
		v3.color = sf::Color();
		v4.color = sf::Color();

		va.append(v1);
		va.append(v2);
		va.append(v3);
		va.append(v4);

		for (Tree3D* sub : subs_)
		{
			float sub_width = factor_ * thickness_ * 0.5f;
			const Point3D& sub_direction = sub->direction_.rotate(angle);

			Point3D sub_normal = sub_direction.get2DNormal().get2DNormalized();
			Point3D sub_position;

			sf::Vertex v5, v6, v7, v8;

			if (sub_direction.x > 0)
			{
				sub_position = upper_r - sub_width * sub_normal;

				v5.position = sf::Vector2f(upper_l.x, upper_l.y);
				v6.position = sf::Vector2f(upper_r.x - (2.0f * sub_width) * sub_normal.x, upper_r.y - (2.0f * sub_width) * sub_normal.y);
				v7.position = sf::Vector2f(upper_r.x, upper_r.y);
				v8.position = sf::Vector2f(upper_r.x, upper_r.y);
			}
			else
			{
				sub_position = upper_l + sub_width * sub_normal;

				v5.position = sf::Vector2f(upper_r.x, upper_r.y);
				v6.position = sf::Vector2f(upper_l.x + (2.0f * sub_width) * sub_normal.x, upper_l.y + (2.0f * sub_width) * sub_normal.y);
				v7.position = sf::Vector2f(upper_l.x, upper_l.y);
				v8.position = sf::Vector2f(upper_l.x, upper_l.y);
			}

			va.append(v5);
			va.append(v6);
			va.append(v7);
			va.append(v8);

			sub->addToVertexArray(va, sub_position, angle);
		}
	}

	void addSubTree(const Point3D& direction)
	{
		subs_.emplace_back(new Tree3D(thickness_*factor_, direction, this));
	}

	void growth(uint32_t n_subs, uint32_t n_times)
	{
		if (n_times)
		{
			for (uint32_t i(n_subs); i--;)
			{
				float length = direction_.getLength();
				//float angle = (rand() % 90) / 360.0f * 2.0f * PI;
				Point3D direction(rand() % 250 - 125, -rand()%120-50.0f, 0.0f);

				subs_.emplace_back(new Tree3D(thickness_*factor_, direction, this));
				subs_.back()->growth(n_subs, n_times - 1);
			}
		}
	}

	void computeGeometry()
	{
		vertices_.push_back(position_);
		computeGeometry(vertices_, position_);
	}

	void computeGeometry(std::vector<Point3D>& vertices, const Point3D& position)
	{
		const Point3D next_position = position + direction_;
		vertices.push_back(next_position);
		
		for (Tree3D* sub : subs_)
		{
			computeGeometry(vertices, next_position);
		}
	}

	float factor_;
	float thickness_;
	Point3D position_;
	Point3D direction_;

	Tree3D* super_;
	std::vector<Tree3D*> subs_;
	std::vector<Point3D> vertices_;
};


int main()
{
    constexpr uint32_t WinWidth = 1600;
	constexpr uint32_t WinHeight = 900;

    sf::RenderWindow window(sf::VideoMode(WinWidth, WinHeight), "Fractale", sf::Style::Default);
    window.setVerticalSyncEnabled(true);

	Tree3D tree(50.0f, 0.75f, Point3D(800.0f, 900.0f, 0.0f), Point3D(0.0f, -200.0f, 0.0f));
	//tree.growth(2, 4);
	tree.addSubTree(Point3D(-200, -200, 0.0f));
	tree.addSubTree(Point3D(200, -200, 0.0f));

	float angle = 0.0;

	while (window.isOpen())
	{
		angle += 0.01f;
        sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::Black);

		tree.draw(&window, angle);

        window.display();
    }

    return 0;
}
