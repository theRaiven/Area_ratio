#include <iostream>
#include <vector>
#include <math.h>

using namespace std;
const double PI = acos(-1.0);

struct Point
{
public: 
    float x, y;

    Point(float x = 0, float y = 0) : x(x), y(y) {}

    Point operator-(const Point& p)
    {
        return Point( x - p.x, y - p.y );
    }
    Point operator+(const Point& p)
    {
        return Point(x + p.x, y + p.y);
    }
    Point operator*(float c)
    {
        return Point(x*c, y*c);
    }
    
    Point Rotate( float angle)
    {
        return Point
        (
            x * cos(angle) - y * sin(angle),
            x * sin(angle) + y * cos(angle)
        );
    }
    float cross(Point p)
    {
        return { x * p.y - y * p.x };
    }
};

class Polygonal
{
private:
    vector<Point> polyg;
public:
    Polygonal(vector<Point> p) : polyg(p) {}
    Polygonal() {}

    float Polygon_area()
    {
        float area = 0;
        int n = polyg.size();
        for (int i = 0; i < n; i++)
        {
            area += (polyg[i].x * polyg[(i + 1) % n].y - polyg[(i + 1) % n].x * polyg[i].y);
        }
        return fabs(area) / 2.0;
    }
    bool is_left(Point a, Point b, Point c)
    {
        return ((b - a).cross(c - a)) > 0;
    }

    float GetIntersectionPoint(Point B_table, Point A_table, Point P_cloth, Point Q_cloth)
    {
        return ((B_table - A_table).cross(P_cloth - A_table)) / ((Q_cloth - P_cloth).cross(B_table - A_table));
    }

    vector<Point> Polygon_crossing(const vector<Point>& cloth, const vector<Point>& table)
    {
        polyg = cloth;

        for (int i = 0; i < table.size(); ++i)
        {
            vector<Point> input = polyg;
            polyg.clear();

            Point A_table = table[i];
            Point B_table = table[(i + 1) % table.size()];

            for (int j = 0; j < input.size(); ++j)
            {
                Point P_cloth = input[j];
                Point Q_cloth = input[(j + 1) % input.size()];
                bool inP = is_left(A_table, B_table, P_cloth);
                bool inQ = is_left(A_table, B_table, Q_cloth);

                float t;

                if (inP && inQ)
                {
                    polyg.push_back(Q_cloth);
                }
                else if (inP && !inQ)
                {
                    t = GetIntersectionPoint(B_table, A_table, P_cloth, Q_cloth);
                    polyg.push_back(P_cloth + (Q_cloth - P_cloth) * t);
                }
                else if (!inP && inQ)
                {
                    t = GetIntersectionPoint(B_table, A_table, P_cloth, Q_cloth);
                    polyg.push_back(P_cloth + (Q_cloth - P_cloth) * t);
                    polyg.push_back(Q_cloth);
                }

            }
        }
        return polyg;
    };
};


float Area(float a, float b, float c)
{
    if (b <= a && c <= a)
    {
        return round((b * c) * 10000) / 10000;
    }
    else if(b> a && c> a)
    {
        return round((a * a) * 10000) / 10000;
    }
    else if (c >= sqrt(2) * a && b + a / 10 > a)
    {
        return round((b * a) * 10000) / 10000;
    }

    else
    {
        float maxArea = 0;
        vector<Point> table
        {
            {-a / 2, -a / 2},
            {a / 2, -a / 2},
            {a / 2, a / 2},
            {-a / 2, a / 2}
        };

        for (float angle = 0; angle < PI / 4.0; angle += 0.0001)
        {
            vector<Point> cloth
            {
                {-b / 2,-c / 2},
                { b / 2,-c / 2},
                { b / 2,c / 2},
                {-b / 2,c / 2},
            };
            for (auto& point : cloth) point = point.Rotate(angle);

            Polygonal temp_cloth;
            temp_cloth.Polygon_crossing(cloth, table);

            float area = temp_cloth.Polygon_area();

            if (area > maxArea) maxArea = area;
        }
    

    return round((maxArea-0.00002)*10000)/10000;
    }
}

int main()
{
	float table_length;
	float tablecloth_width;
	float tablecloth_length;
 	cin >> table_length >> tablecloth_width >> tablecloth_length;
	cout << Area(table_length, tablecloth_width, tablecloth_length);
    return 0;
}
