#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <memory>

struct PPMImage {
    int width, height, max_color_value;
    std::vector<unsigned char> data;
    std::vector<float> norm;
    
    void normalize() {
        norm.resize(width * height * 3);
        for(uint i = 0; i < width * height * 3; i++) {
            norm[i] = std::min(std::max(static_cast<float>(data[i]) / max_color_value, 0.0f), 1.0f);
        }
    }

    void loadNorm() {
        data.resize(width * height * 3);
        for(uint i = 0; i < width * height * 3; i++) {
            data[i] = static_cast<unsigned char>(norm[i] * max_color_value);
        }
    }

    void fromFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file: " + filename);
        }
    
        std::string magic_number;
        file >> magic_number;
        if (magic_number != "P6") {
            throw std::runtime_error("Invalid PPM file format. Expected P6.");
        }
    
        // Consume the newline character after the magic number
        file.ignore();
    
        // Function to skip comments and empty lines
        auto skip_comments_and_empty_lines = [&](std::istream& is) {
            std::string line;
            while (true) {
                std::getline(is, line);
                if (is.eof()) return std::stringstream();
                if(!line.empty() && line[0] != '#') return std::stringstream(line);
            }
        };
    
    
        std::stringstream dimensions_stream = skip_comments_and_empty_lines(file);
        dimensions_stream >> std::ws; // Consume leading whitespaces
        dimensions_stream >> width >> height;
    
        std::stringstream max_value_stream = skip_comments_and_empty_lines(file);
        max_value_stream >> std::ws;
        max_value_stream >> max_color_value;
    
    
        if (file.fail()) {
            throw std::runtime_error("Error reading image dimensions or max color value.");
        }
        
        data.resize(width * height * 3);
    
        file.read(reinterpret_cast<char*>(data.data()), data.size());
    
        if (file.fail()) {
             throw std::runtime_error("Error reading pixel data.");
        }
    }

    void toFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            throw std::runtime_error("Could not open file for writing: " + filename);
        }

        file << "P6\n";
        file << width << " " << height << "\n";
        file << max_color_value << "\n";
        file.write(reinterpret_cast<const char*>(data.data()), data.size());

    }
};