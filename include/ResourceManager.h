#include <string>

class ResourceManager {
public:
    static std::string getImagePath(const std::string& imageName) {
        return "resources/input_images/" + imageName;
    }

    static std::string getOutputPath(const std::string& outputName) {
        return "resources/output_images/" + outputName;
    }

    static std::string getConfigPath() {
        return "resources/config.json";
    }
};
