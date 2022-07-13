/**
 * @file drm_reader.cpp
 * @brief DRM (Digital RoadMap database)の読み込み用ライブラリ
 * @author charmie11
*/

#include <fstream>
#include <sstream>
#include <set>

#include "drm_reader.hpp"

Coordinate::Coordinate():
    x{}, y{}
{
}

Coordinate::Coordinate(int x_, int y_):
    x{x_}, y{y_}
{
}

Coordinate::Coordinate(const std::string &str)
{
    if(str.length() != 10)
    {
        throw std::invalid_argument("Coordinate as string must be length 10");
    }
    x = std::stoi(str.substr(0, 5));
    y = std::stoi(str.substr(5, 5));
}

std::ostream& operator<< (std::ostream &os, const Coordinate &coordinate)
{
    return os << "(" << coordinate.x << ", " << coordinate.y << ")";
}

Node::Node(int id_, int type_, int x_, int y_, int elevation_):
        id{id_},
        type{type_},
        coordinate{Coordinate{x_, y_}},
        elevation{elevation_}
{
}

Node::Node(int id_, int type_, const Coordinate& coordinate_, int elevation_):
    id{id_},
    type{type_},
    coordinate{coordinate_},
    elevation{elevation_}
{
}

Node::Node(const std::string& line)
{
    if(line.substr(0, 2) != RECORD_IDS[eRecordID::NODE])
    {
        throw std::invalid_argument("the line is not node");
    }
    id = std::stoi(line.substr(2, 4));
    type = std::stoi(line.substr(21, 1));
    coordinate = Coordinate{
        std::stoi(line.substr(8, 5)),
        std::stoi(line.substr(13, 5))
    };
    elevation = std::stoi(line.substr(18, 3));
}

std::ostream& operator<< (std::ostream &os, const Node &node)
{
    return os << "Node("
              << "ID: " << node.id
              << ", Type: " << node.type
              << ", Coordinate: " << node.coordinate
              << ", elevation: " << node.elevation
              << ")";
}

bool Node::operator<(const Node& rhs) const
{
    return id < rhs.id;
}

std::vector<Node> extract_nodes(const std::string& filename)
{
    std::ifstream ifs{filename};
    std::string line;
    std::vector<Node> nodes;
    while(std::getline(ifs, line))
    {
        if(line.substr(0, 2) != RECORD_IDS[eRecordID::NODE])
            continue;
        nodes.emplace_back(Node{line});
    }

    return nodes;
}

bool save_nodes(
    const std::vector<Node>& nodes,
    const std::string& filename
)
{
    std::ofstream ofs{filename};
    ofs << "ID,X座標,Y座標" << std::endl;
    for(const auto& node : nodes)
    {
        ofs << node.id << ","
            << node.coordinate.x << ","
            << node.coordinate.y << std::endl;
    }
    return true;
}

Link::Link(
    int node1_id_, int node2_id_,
    int internal_record_id_, int road_type_, int length_, int link_type_,
    const std::string& coordinates_
):
    node1_id{node1_id_}, node2_id{node2_id_},
    internal_record_id{internal_record_id_}, road_type{road_type_},
    length{length_}, link_type{link_type_}
{
    int num_coordinates{static_cast<int>(coordinates_.length()) / 10};
    for(int n = 0; n < num_coordinates; ++n) {
        coordinates.emplace_back(Coordinate{coordinates_.substr(n * 10, 10)});
    }
}

std::ostream& operator<< (std::ostream &os, const Link &link)
{
    std::string coordinates;
    for(auto coordinate: link.coordinates)
    {
        coordinates += "(" + std::to_string(coordinate.x) + ", " + std::to_string(coordinate.y) + "), ";
    }
    return os << "Link("
              << "Nodes: (" << link.node1_id << ", " << link.node2_id << ")"
              << ", Internal record ID: " << link.internal_record_id
              << ", Road type: " << link.road_type
              << ", Length: " << link.length
              << ", Link type: " << link.link_type
              << ", Coordiantes: " << coordinates
              << ")";
}

bool Link::operator<(const Link& rhs) const
{
    return node1_id != rhs.node1_id ? node1_id < rhs.node1_id : node2_id < rhs.node2_id;
}

std::vector<Link> extract_links(const std::string& filename)
{
    std::ifstream ifs{filename};
    std::string line;
    std::vector<Link> links;

    std::set<std::string> node_pairs;
    std::string coordinates;
    int num_coordinates;
    while(std::getline(ifs, line))
    {
        if (line.substr(0, 2) != RECORD_IDS[eRecordID::LINK])
            continue;

        auto node_pair{line.substr(2, 8)};
        if(node_pairs.find(node_pair) == node_pairs.end())
        {
            // register the previous link
            if(coordinates.length() > 0)
            {
                links.emplace_back(Link{
                        std::stoi(line.substr(2, 4)), std::stoi(line.substr(6, 4)),
                        std::stoi(line.substr(10, 2)),
                        std::stoi(line.substr(13, 1)),
                        std::stoi(line.substr(44, 5)),
                        std::stoi(line.substr(49, 1)),
                        coordinates.substr(0, num_coordinates*10)
                });
            }
            // start the new link
            node_pairs.insert(node_pair);
            num_coordinates = std::stoi(line.substr(88, 3));
            coordinates = line.substr(91, 160);
        }
        else
        {
            // add the current line to the corresponding link
            coordinates += line.substr(91, 160);
        }
    }

    return links;
}

bool save_links(
    const std::vector<Link>& links,
    const std::string& filename
)
{
    std::ofstream ofs{filename};
    ofs << "ノード1ID,ノード2ID,リンク長,補間点座標" << std::endl;
    for(const auto& link : links)
    {
        ofs << link.node1_id << ","
            << link.node2_id << ","
            << link.length << ",";
        for(const auto& coordinate : link.coordinates)
        {
            ofs << coordinate.x << ","
                << coordinate.y << ",";
        }
        ofs << std::endl;
    }
    return true;
}

Place::Place(int id_, int type_, int x_, int y_, const std::string& name_):
        id{id_},
        type{type_},
        coordinate{Coordinate{x_, y_}},
        name{name_}
{
}

Place::Place(int id_, int type_, const Coordinate& coordinate_, const std::string& name_):
        id{id_},
        type{type_},
        coordinate{coordinate_},
        name{name_}
{
}

Place::Place(const std::string& line)
{
    if(line.substr(0, 2) != RECORD_IDS[eRecordID::PLACE])
    {
        throw std::invalid_argument("the line is not place");
    }
    id = std::stoi(line.substr(3, 4));
    type = std::stoi(line.substr(11, 2));
    coordinate = Coordinate{
            std::stoi(line.substr(23, 5)),
            std::stoi(line.substr(28, 5))
    };
    int num_characters{std::stoi(line.substr(33, 2))};
    // assuming that the file encoding is UTF8
    name = line.substr(35, 3*num_characters);
}

std::ostream& operator<< (std::ostream &os, const Place &place)
{
    return os << "Place("
              << "ID: " << place.id
              << ", Type: " << place.type
              << ", Coordinate: " << place.coordinate
              << ", Name: " << place.name
              << ")";
}

bool Place::operator<(const Place& rhs) const
{
    return id < rhs.id;
}

std::vector<Place> extract_places(const std::string& filename)
{
    std::ifstream ifs{filename};
    std::string line;
    std::vector<Place> places;
    while(std::getline(ifs, line))
    {
        if(line.substr(0, 2) != RECORD_IDS[eRecordID::PLACE])
            continue;
        places.emplace_back(Place{line});
    }

    return places;
}

bool save_places(
    const std::vector<Place>& places,
    const std::string& filename
)
{
    std::ofstream ofs{filename};
    ofs << "地名,X座標,Y座標" << std::endl;
    for(const auto& place : places)
    {
        ofs << place.name << ","
            << place.coordinate.x << ","
            << place.coordinate.y << std::endl;
    }
    return true;
}
