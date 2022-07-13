# DRM-reader

- 以下の機能をC++20で実装
- ファイルはUTF8でエンコーディングされていることを想定

1. DRM(Digital RoadMap database)ファイルからレコードIDに該当する情報を読み込む
   1. `extract_nodes()`: 基本地図ノードを読み込む
   1. `extract_links()`: 基本地図リンクを読み込む
   1. `extract_places()`: 地名を読み込む
1. 取得した情報をcsvファイルに保存
   1. `save_nodes()`: 読み込んだ基本地図ノードをcsvファイルに保存
   1. `save_links()`: 読み込んだ基本地図リンクをcsvファイルに保存
   1. `save_places()`: 読み込んだ地名をcsvファイルに保存

## how to use

```cpp
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
```

## 保存されるCSVファイル

保存されるcsvファイルのフォーマットは以下の通り．

### 基本地図ノード

以下のようなフォーマットのcsvファイルが生成される．

| ID | X座標 | Y座標 |
|:---:|:---:|:---:|
| `id_1` | `x_1` | `y_1`|
| ... | ... | ... |
| `id_N` | `x_N` | `y_N`|

### 基本地図リンク

| ノード1ID | ノード2ID | リンク町 | 補間点座標 |
|:---:|:---:|:---:|:---:|
| `link_1_node1_id` | `link_1_node2_id` | `length_1`| `link_1_x_1`, `link_1_y_1`, ..., `link_1_x_num_1`, `link_1_y_num_1` |
| ... | ... | ... | ... |
| `link_N_node1_id` | `link_N_node2_id` | `length_N`| `link_N_x_1`, `link_N_y_1`, ..., `link_1_x_num_N`, `link_1_y_num_N` |

### 地名

| 地名 | X座標 | Y座標 |
|:---:|:---:|:---:|
| 地名`1` | `x_1` | `y_1`|
| ... | ... | ... |
| 地名`N` | `x_N` | `y_N`|
