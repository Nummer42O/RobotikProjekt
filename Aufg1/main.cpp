#include <utility>
#include <stdexcept>
#include <list>
#include <vector>
#include <string>
#include <iostream>

template<typename TYPE>
class Vec2 {
    public:
        TYPE x, y;

        Vec2() {
            x = 0;
            y = 0;
        }
        template<typename otherType>
        Vec2(otherType X, otherType Y): x(static_cast<TYPE>(X)), y(static_cast<TYPE>(Y)) {}

        ~Vec2() {}

        Vec2(const Vec2<TYPE> &other): x(other.x), y(other.y) {}
        Vec2<TYPE> &operator=(const Vec2<TYPE> &other) {
            x = other.x;
            y = other.y;

            return *this;
        }

        Vec2(Vec2<TYPE> &&other) {
            std::swap(x, other.x);
            std::swap(y, other.y);
        }
        Vec2<TYPE> &operator=(Vec2<TYPE> &&other) {
            std::swap(x, other.x);
            std::swap(y, other.y);

            return *this;
        }

        template<typename friendType>
        friend std::ostream &operator<<(std::ostream &stream, const Vec2<friendType> &vector) {
            stream << '(' << vector.x << ", " << vector.y << ')' << std::flush;

            return stream;
        }
};

template<typename TYPE>
class Polygon {
    protected:
        Vec2<TYPE> *points;
        unsigned int n;
    public:
        Polygon(unsigned int N): n(N) {
            points = new Vec2<TYPE>[n];
        }
        Polygon(unsigned int N, Vec2<TYPE> *data): n(N) {
            points = new Vec2<TYPE>[n];

            for (unsigned int i = 0; i < n; i++) {
                points[i] = data[i];
            }
        }

        ~Polygon() {
            delete[] points;
        }

        Polygon(const Polygon<TYPE> &other) {
            if (other.n != n) {
                delete[] points;

                points = new Vec2<TYPE>[other.n];

                n = other.n;
            }

            for (unsigned int i = 0; i < n; i++) {
                points[i] = other.points[i];
            }
        }

        Polygon(Polygon<TYPE> &&other): n(other.n) {
            std::swap(points, other.points);
        }

        template<typename friendType>
        friend std::ostream &operator<<(std::ostream &stream, const Polygon<friendType> &polygon) {
            if (polygon.n > 6) {
                stream << '{' << polygon.points[0]             << ", " << polygon.points[1]             << ", " << polygon.points[2] << ", ..., "
                              << polygon.points[polygon.n - 3] << ", " << polygon.points[polygon.n - 2] << ", " << polygon.points[polygon.n - 1];
            } else {
                stream << '{' << polygon.points[0];

                for (unsigned int i = 1; i < polygon.n; i++) {
                    stream << ", " << polygon.points[i];
                }
            }

            stream << '}' << std::flush;

            return stream;
        }
};

template<typename TYPE>
class Rect: public Polygon<TYPE> {
    public:
        Rect(Vec2<TYPE> top_left, Vec2<TYPE> bottom_right): Polygon<TYPE>(4) {
            this->points[0] = top_left;
            this->points[1] = Vec2<TYPE>(bottom_right.x, top_left.y);
            this->points[2] = bottom_right;
            this->points[3] = Vec2<TYPE>(top_left.x, bottom_right.y);
        }


        template<typename friendType>
        friend std::ostream &operator<<(std::ostream &stream, const Rect<friendType> &rect) {
            stream << '{' << rect.points[0] << ", " << rect.points[1] << ", "
                          << rect.points[2] << ", " << rect.points[3] << '}' << std::flush;

            return stream;
        }
};

template<typename TYPE>
Rect<TYPE> getBBox(const std::list<Vec2<TYPE>> points) {
    Vec2<TYPE> top_left(0, 0), bottom_right(0, 0);
    TYPE x, y;

    for (auto it = points.begin(); it != points.end(); it++) {
        x = it->x;
        y = it->y;

        if (x < top_left.x) {
            top_left.x = x;
        } else if (x > bottom_right.x) {
            bottom_right.x = x;
        }
        if (y < top_left.y) {
            top_left.y = y;
        } else if (y > bottom_right.y) {
            bottom_right.y = y;
        }
    }

    return Rect<TYPE>(top_left, bottom_right);
}

//src: https://de.wikipedia.org/wiki/Punkt-in-Polygon-Test_nach_Jordan
template<typename TYPE>//      point, poly point 1, poly point 2
int crossProductTest(Vec2<TYPE> a, Vec2<TYPE> b, Vec2<TYPE> c) {
    if (a.x == b.x && b.x == c.x) {
        if ((b.x <= a.x && a.x <= c.x) || (c.x <= a.x && a.x <= b.x)) {
            return 0;
        } else {
            return 1;
        }
    }
    if (a.y == b.y && a.x == b.x) {
        return 0;
    }
    if (b.y > c.y) {
        std::swap(b, c);
    }
    if (a.y <= b.y || a.y > c.y) {
        return 1;
    }
    TYPE delta = (b.x - a.x) * (c.y - a.y) - (b.y - a.y) * (c.x - a.x);
    if (delta > 0) {
        return -1;
    } else if (delta < 0) {
        return 1;
    } else {
        return 0;
    }
}

//src: https://de.wikipedia.org/wiki/Punkt-in-Polygon-Test_nach_Jordan
template<typename TYPE>
bool inPolygon(unsigned int size, Vec2<TYPE> *polygon, const Vec2<TYPE> point) {
    Vec2<TYPE> previous = polygon[size - 1];
    int t = -1;

    for (unsigned int i = 0; i < size; i++) {
        t *= crossProductTest(point, previous, polygon[i]);

        if (t == 0) {
            t = -1;
            break;
        } else {
            previous = polygon[i];
        }
    }

    return t == 1;
}

template<typename TYPE>
Polygon<TYPE> getMaxPolygon(const std::list<Vec2<TYPE>> points) {
    unsigned int ammount_points = points.size();
    Vec2<TYPE> *data = new Vec2<TYPE>[ammount_points], point;

    auto iter = points.begin();

    switch (ammount_points) {
        case 1:
            data[0] = *(iter++);

            return Polygon<TYPE>(1, data);
        case 2:
            data[0] = *(iter++);
            data[1] = *(iter++);

            return Polygon<TYPE>(2, data);
        default:
            data[0] = *(iter++);
            data[1] = *(iter++);
            data[2] = *(iter++);
    }

    unsigned int i = 3;

    while (iter != points.end()) {
        point = *(iter++);
        if (!inPolygon(i, data, point)) {
            data[i++] = point;
        }
    }

    return Polygon<TYPE>(i, data);
}

int main(int argc, char* argv[]) {
    std::list<Vec2<int>> lst;
    std::string arg;
    unsigned int comma_pos;

    for (unsigned int i = 1; i < argc; i++) {
        arg = argv[i];
        comma_pos = arg.find(',');

        // std::cout << std::stoi(arg.substr(0, comma_pos++)) << '\n'
        //           << std::stoi(arg.substr(comma_pos, arg.size() - comma_pos)) << std::endl;

        lst.push_back(Vec2<int>(std::stoi(arg.substr(0, comma_pos)),
                                std::stoi(arg.substr(comma_pos + 1, arg.size() - comma_pos - 1))));
    }

    std::cout << "bounding box:      " << getBBox(lst) << '\n'
              << "enclosing polygon: " << getMaxPolygon(lst) << std::endl;

    return EXIT_SUCCESS;
}