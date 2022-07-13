/**
 * @file drm_reader.hpp
 * @brief DRM (Digital RoadMap database)の読み込み用ライブラリ
 * @author charmie11
*/

#ifndef DRM_READER_H
#define DRM_READER_H

#include <string>
#include <vector>

/** レコードID */
enum eRecordID {
    NODE,
    LINK,
    PLACE
};
static const char *RECORD_IDS[] = {
"21",
"22",
"46"
};

struct Coordinate
{
    Coordinate();
    Coordinate(int x_, int y_);
    explicit Coordinate(const std::string& str);

    friend std::ostream& operator<< (std::ostream &os, const Coordinate &Coordinate);

    //! x座標
    int x;
    //! y座標
    int y;
};

struct Node
{
    Node(int id_, int type_, int x_, int y_, int elevation_);
    Node(int id_, int type_, const Coordinate& coordinate_, int elevation_);
    explicit Node(const std::string& line);

    friend std::ostream& operator<< (std::ostream &os, const Node &node);
    bool operator<(const Node& rhs) const;

    //! ノードID
    int id;
    //! ノードの種類
    int type;
    //! 座標
    Coordinate coordinate;
    //! 標高
    int elevation;
};

/**
 * @fn
 *
 * @brief DRMファイルから基本地図ノードを読み込む
 * @param (filename) DRMファイルのファイル名
 * @return 読み込んだ全基本地図ノード
 */
std::vector<Node> extract_nodes(const std::string& filename);

/**
 * @fn
 *
 * @brief DRMファイルから読み込んだ基本地図ノード情報をcsvファイルとして保存
 * @param (places) 読み込んだ全基本地図ノード
 * @param (filename) DRMファイルのファイル名
 */
bool save_nodes(
    const std::vector<Node>& nodes,
    const std::string& filename
);

struct Link
{
    Link(
        int node1_id_, int node2_id_,
        int internal_record_id_, int road_type_, int length_, int link_type_,
        const std::string& coordinates_
    );

    friend std::ostream& operator<< (std::ostream &os, const Link &node);
    bool operator<(const Link& rhs) const;

    //! ノード1のID
    int node1_id;
    //! ノード1のID
    int node2_id;
    //! 内部記録ID
    int internal_record_id;
    //! 道路の種類
    int road_type;
    //! リンク長
    int length;
    //! リンクの種類
    int link_type;
    std::vector<Coordinate> coordinates;  // list of x and y coordinates
};

/**
 * @fn
 *
 * @brief DRMファイルから基本地図リンクを読み込む
 * @param (filename) DRMファイルのファイル名
 * @return 読み込んだ全基本地図リンク
 */
std::vector<Link> extract_links(const std::string& filename);

/**
 * @fn
 *
 * @brief DRMファイルから読み込んだ基本地図リンク情報をcsvファイルとして保存
 * @param (places) 読み込んだ全基本地図リンク
 * @param (filename) DRMファイルのファイル名
 */
bool save_links(
    const std::vector<Link>& links,
    const std::string& filename
);

struct Place
{
    Place(int id_, int type_, int x_, int y_, const std::string& name_);
    Place(int id_, int type_, const Coordinate& coordinate_, const std::string& name_);
    explicit Place(const std::string& line);

    friend std::ostream& operator<< (std::ostream &os, const Place &place);
    bool operator<(const Place& rhs) const;

    //! 地名のアイテムID
    int id;
    //! 地名の種類
    int type;
    //! 地名の座標
    Coordinate coordinate;
    //! 地名の文字列
    std::string name;
};

/**
 * @fn
 *
 * @brief DRMファイルから地名を読み込む
 * @param (filename) DRMファイルのファイル名
 * @return 読み込んだ地名
 */
std::vector<Place> extract_places(const std::string& filename);

/**
 * @fn
 *
 * @brief DRMファイルから読み込んだ地名情報をcsvファイルとして保存
 * @param (places) 読み込んだ全地名
 * @param (filename) DRMファイルのファイル名
 */
bool save_places(
    const std::vector<Place>& places,
    const std::string& filename
);

#endif  // DRM_READER_H

