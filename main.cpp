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
			return Point3D(x / z, y / z, 1.0f);

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
		sf::VertexArray va(sf::Points, vertices_.size());
		
		uint32_t i(0);
		for (const Point3D& p : vertices_)
		{
			const Point3D& rotated = p.rotate(angle);
			const Point3D& projected = rotated.getProjection();
			va[i].position = sf::Vector2f(projected.x, projected.y);
			++i;
		}
		
		sf::RenderStates rs;
		rs.transform.translate(position_.x, position_.y);

		target->draw(va, rs);
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
		vertices_.push_back(Point3D());
		computeGeometry(vertices_, Point3D());
	}

	void computeGeometry(std::vector<Point3D>& vertices, const Point3D& position)
	{
		const Point3D next_position = position + direction_;
		vertices.push_back(next_position);
		
		for (Tree3D* sub : subs_)
		{
			sub->computeGeometry(vertices, next_position);
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

	Tree3D tree(10.0f, 0.75f, Point3D(800.0f, 450.0f, 50.0f), Point3D(0.0f, -20.0f, 0.0f));
	//tree.growth(2, 4);
	tree.addSubTree(Point3D(-10, -10, 0.0f));
	tree.addSubTree(Point3D(10, -10, 0.0f));
	tree.computeGeometry();

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
