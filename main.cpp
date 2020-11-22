#include <Magick++.h> 
#include <argparse/argparse.hpp>
#include <experimental/random>
#include <iostream>
#include <filesystem>
#include <vector>
#include <cstdint>

using namespace Magick; 

class Shrinker
{
public:
    static constexpr std::string_view program_version = "1.0";

public:
    static constexpr std::string_view input_image_arg = "input_image_file";
    static constexpr std::string_view tile_width_arg = "tile_width";
    static constexpr std::string_view tile_height_arg = "tile_height";
    static constexpr std::string_view space_width_arg = "space_width";
    static constexpr std::string_view space_height_arg = "space_height";
    static constexpr std::string_view output_image_arg = "output_image_file";

public:
    Shrinker(int argc, char** argv)
    {
        parse_args_(argc, argv);
        InitializeMagick(*argv);
    }

    void run()
    {
        Image tileset;
        try
        {
            tileset.read(input_image_path_.generic_string());
            unsigned width = tileset.size().width() / (tile_width_ + space_width_);
            unsigned height = tileset.size().height() / (tile_height_ + space_height_);
            shrink_(tileset, width, height, 0, 0);
        }
        catch (Exception& error)
        {
            std::cout << "Caught exception: " << error.what() << std::endl;
            exit(EXIT_FAILURE);
        }
    }

private:
    void shrink_(Image& image, unsigned width, unsigned height, unsigned start_column, unsigned start_row)
    {
        std::vector<Image> tiles;
        tiles.reserve(width * height);

        for (unsigned row = start_row; row < height; ++row)
        {
            for (unsigned column = start_column; column < width; ++column)
            {
                unsigned offx = tile_width_ * column + space_width_ * column;
                unsigned offy = tile_height_ * row + space_height_ * row;
                Image tile(image, Geometry(tile_width_, tile_height_, offx, offy));
                tiles.push_back(std::move(tile));
            }
        }

        Montage montage_opts;
        montage_opts.tile(std::to_string(width) + "x");
        montage_opts.geometry(std::to_string(tile_width_) + "x" + std::to_string(tile_height_) + "+0+0");
        std::vector<Image> tilesets;
        tilesets.reserve(16);
        montageImages(&tilesets, tiles.begin(), tiles.end(), montage_opts);
        if (tilesets.size() == 1)
        {
            tilesets.front().write(output_image_path_);
        }
        else
        {
            unsigned index = 0;
            for (Image& tileset : tilesets)
            {
                std::string output_filename(output_image_path_.stem().generic_string());
                output_filename += '_';
                output_filename += std::to_string(index++);
                output_filename += output_image_path_.extension().generic_string();
                tileset.write(output_filename);
            }
        }
    }

private:
    void parse_args_(int argc, char** argv)
    {
        argparse::ArgumentParser parser(*argv, program_version.data());
        add_arguments_(parser);

        try
        {
            parser.parse_args(argc, argv);
        }
        catch (const std::runtime_error& err)
        {
            std::cout << err.what() << std::endl;
            std::cout << parser;
            exit(-1);
        }

        input_image_path_ = parser.get<std::string>(input_image_arg);
        tile_width_ = parser.get<int>(tile_width_arg);
        tile_height_ = parser.get<int>(tile_height_arg);
        space_width_ = parser.get<int>(space_width_arg);
        space_height_ = parser.get<int>(space_height_arg);
        output_image_path_ = parser.get<std::string>(output_image_arg);

        std::cout << input_image_path_ << std::endl;
        std::cout << tile_width_ << std::endl;
        std::cout << tile_height_ << std::endl;
        std::cout << space_width_ << std::endl;
        std::cout << space_height_ << std::endl;
        std::cout << output_image_path_ << std::endl;
    }

    void add_arguments_(argparse::ArgumentParser& parser)
     {
        parser.add_argument(input_image_arg)
                .help("Input image");
        parser.add_argument(tile_width_arg)
                .help("Tile width")
                .nargs(1)
                .default_value(int{ 32 })
                .action([](const std::string& value) { return std::stoi(value); });
        parser.add_argument(tile_height_arg)
                .help("Tile height")
                .nargs(1)
                .default_value(int{ 32 })
                .action([](const std::string& value) { return std::stoi(value); });
        parser.add_argument(space_width_arg)
                .help("Space width")
                .nargs(1)
                .action([](const std::string& value) { return std::stoi(value); });
        parser.add_argument(space_height_arg)
                .help("Space height")
                .nargs(1)
                .action([](const std::string& value) { return std::stoi(value); });
        parser.add_argument(output_image_arg)
                .help("Output image");
     }

private:
    std::filesystem::path input_image_path_;
    unsigned tile_width_;
    unsigned tile_height_;
    unsigned space_width_;
    unsigned space_height_;
    std::filesystem::path output_image_path_;
};

int main(int argc, char** argv)
{ 
    Shrinker shrinker(argc, argv);
    shrinker.run();
    std::cout << "EXIT SUCCESS" << std::endl;
    return EXIT_SUCCESS;
}
