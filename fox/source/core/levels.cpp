#include "pch.h"
#include "fox.res.id.h"
#include "source/core/levels.h"

std::string_view level_tiles[] =
{
    // Level 0
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . o . . . x x . . . o . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . . . . . x x . . . . . . . "sv
    ". . . o . . . x x . . . o . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv,

    // Level 1
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x o . . . . o x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x . . . . . . x x . . . "sv
    ". . . x x o . . . . o x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . x x x x x x x x x x . . . "sv
    ". . . . . . . . . . . . . . . . "sv
    ". . . . . . . . . . . . . . . . "sv,
};

static game::level_data get_normal(size_t index)
{
    game::level_data level{};
    level.index = index;
    level.level_type = game::level_type::normal;
    level.max_timer = 120;
    level.max_moves = 200;

    std::string_view tiles = ::level_tiles[index % _countof(::level_tiles)];
    assert(tiles.size() == game::constants::TILE_COUNT_X * game::constants::TILE_COUNT_Y * 2);

    for (size_t i = 0, h = 0; i < tiles.size(); i += 2, h++)
    {
        const char ch = tiles[i];
        switch (ch)
        {
            case 'x':
                level.tiles[h] = game::tile_type::panel0;
                break;

            case 'X':
                level.tiles[h] = game::tile_type::panel1;
                break;

            case 'o':
                level.tiles[h] = game::tile_type::bomb;
                break;
        }
    }

    return level;
}

game::levels::levels()
{
    this->init_resources();
}

void game::levels::init_resources()
{
    this->levels_.clear();

    std::shared_ptr<ff::resource_value_provider> level_provider = ff::get_resource<ff::resource_value_provider>(assets::game::LEVELS);
    assert_ret(level_provider);

    std::vector<ff::value_ptr> level_values = level_provider->get_resource_value("levels")->get<std::vector<ff::value_ptr>>();
    this->levels_.reserve(level_values.size());

    for (ff::value_ptr level_value : level_values)
    {
        game::level_data level{};
        ff::dict level_dict = level_value->get<ff::dict>();

        for (ff::value_ptr prop_value : level_dict.get<std::vector<ff::value_ptr>>("properties"))
        {
            ff::dict prop_dict = prop_value->get<ff::dict>();
            std::string prop_name =  prop_dict.get<std::string>("name");

            if (prop_name == "moves")
            {
                level.max_moves = prop_dict.get<size_t>("value");
            }
            else if (prop_name == "timer")
            {
                level.max_timer = static_cast<size_t>(prop_dict.get<double>("value") * 60);
            }
        }

        for (ff::value_ptr layer_value : level_dict.get<std::vector<ff::value_ptr>>("layers"))
        {
            ff::dict layer_dict = layer_value->get<ff::dict>();
            std::string layer_type = layer_dict.get<std::string>("type");

            if (layer_type == "tilelayer" &&
                layer_dict.get<std::string>("compression").empty() &&
                layer_dict.get<std::string>("encoding") == "base64" &&
                layer_dict.get<int>("width") == game::constants::TILE_COUNT_X &&
                layer_dict.get<int>("height") == game::constants::TILE_COUNT_Y &&
                layer_dict.get<bool>("visible"))
            {
                int warp_count = 0;
                const std::string& layer_data64 = layer_dict.get<std::string>("data");
                std::shared_ptr<ff::data_base> layer_data = ff::compression::decode_base64(layer_data64);
                if (layer_data->size() == static_cast<size_t>(game::constants::TILE_COUNT_X) * static_cast<size_t>(game::constants::TILE_COUNT_Y) * sizeof(int))
                {
                    const int* layer_ints = reinterpret_cast<const int*>(layer_data->data());
                    for (int y = 0; y < game::constants::TILE_COUNT_Y; y++)
                    {
                        for (int x = 0; x < game::constants::TILE_COUNT_X; x++, layer_ints++)
                        {
                            game::tile_type tile = game::tile_type::none;

                            switch (*layer_ints)
                            {
                                case 2:
                                    tile = game::tile_type::panel0;
                                    break;

                                case 3:
                                    tile = game::tile_type::panel1;
                                    break;

                                case 4:
                                    tile = game::tile_type::bomb;
                                    break;

                                case 5:
                                    tile = game::tile_type::points;
                                    break;

                                case 6:
                                    switch (warp_count++)
                                    {
                                        case 0: tile = game::tile_type::warp0; break;
                                        case 1: tile = game::tile_type::warp1; break;
                                        case 2: tile = game::tile_type::warp2; break;
                                        case 3: tile = game::tile_type::warp3; break;
                                    }
                                    break;

                                default:
                                    continue;
                            }

                            level.tile(ff::point_int(x, y), tile);
                        }
                    }
                }
            }
            else if (layer_type == "objectgroup" && layer_dict.get<bool>("visible"))
            {
            }
        }

        this->levels_.push_back(std::move(level));
    }
}

game::level_data game::levels::get_level(game::game_type type, game::game_diff diff, size_t index)
{
    assert_ret_val(this->levels_.size(), game::level_data{});

    return this->levels_[index % this->levels_.size()];
}
