#include <iostream>
#include "tinyxml2.h"
#include <string>
#include <thread>
void SetNewVolume(tinyxml2::XMLElement* rootElement, int volumeLevel);

void modifyXMLFile(const char* filename) {
    tinyxml2::XMLDocument doc;
    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* config = doc.FirstChildElement("Config");
    if (config == nullptr) {
        std::cerr << "Root element <Config> not found!" << std::endl;
        return;
    }

    tinyxml2::XMLElement* graphics = config->FirstChildElement("Graphics");
    if (graphics) {
        tinyxml2::XMLElement* resolution = graphics->FirstChildElement("Resolution");
        if (resolution) {
            resolution->SetAttribute("width", 2560);
            resolution->SetAttribute("height", 1440);
            std::cout << "Resolution updated: 2560x1440" << std::endl;
        }
    }

    tinyxml2::XMLElement* fullscreen = config->FirstChildElement("Graphics")->FirstChildElement("Fullscreen");
    if (fullscreen) {
        fullscreen->SetText(false);
        std::cout << "Fullscreen updated to false" << std::endl;
    }
    
    std::thread t([config](){SetNewVolume(config, 100);});
    t.join();

    if (doc.SaveFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to save the modified XML file!" << std::endl;
    } else {
        std::cout << "XML file updated and saved!" << std::endl;
    }
}

void readXMLFile(const char* filename)
{
    tinyxml2::XMLDocument doc;

    if (doc.LoadFile(filename) != tinyxml2::XML_SUCCESS) {
        std::cerr << "Failed to load XML file!" << std::endl;
        return;
    }
    auto* config = doc.FirstChildElement("Config");
    if (config) {
        auto* graphics = config->FirstChildElement("Graphics");
        if (graphics) {
            auto* resolution = graphics->FirstChildElement("Resolution");
            if (resolution) {
                int width = resolution->IntAttribute("width");
                int height = resolution->IntAttribute("height");

                std::cout << "Resolution: " << width << "x" << height << '\n';
            }
            bool fullscreen = graphics->FirstChildElement("Fullscreen")->BoolText();
            std::cout << "Fullscreen: " << (fullscreen ? "Yes" : "No") << '\n';
        }
    }
}

void SetNewVolume(tinyxml2::XMLElement* rootElement, const int volumeLevel)
{
    tinyxml2::XMLElement* audio = rootElement->FirstChildElement("Audio");
    if (audio) {
        tinyxml2::XMLElement* volume = audio->FirstChildElement("Volume");
        if (volume) {
            volume->SetText(volumeLevel);
        }
    }
}


int main() {
    readXMLFile("config.xml");
    modifyXMLFile("config.xml");
    return 0;
}

