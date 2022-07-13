#include <iostream>

#include "drm_reader.hpp"

int main()
{
    std::string filename_drm{"drm-utf8-encoding.txt"};
    std::string filename_nodes{"nodes.csv"};
    std::string filename_links{"links.csv"};
    std::string filename_places{"places.csv"};

    // 基本地図ノードの取得と保存
    auto nodes{extract_nodes(filename_drm)};
    std::cout << "#nodes: " << nodes.size() << std::endl;
    for(const auto& node : nodes)
    {
        std::cout << node << std::endl;
    }
    std::cout << std::endl;
    save_nodes(nodes, filename_nodes);

    // 基本地図リンクの取得と保存
    std::vector<Link> links{extract_links(filename_drm)};
    std::cout << "#links: " << links.size() << std::endl;
    for(const auto& link : links)
    {
        std::cout << link << std::endl;
    }
    save_links(links, filename_links);

    // 地名の取得と保存
    std::vector<Place> places{extract_places(filename_drm)};
    std::cout << "#places: " << places.size() << std::endl;
    for(const auto& place: places)
    {
        std::cout << place << std::endl;
    }
    save_places(places, filename_places);

    return 0;
}
