#include "openai.hpp"

#include <fstream>

int main() {
    // Load the OpenAI API key from a file. You can also specify directly the token in your code as a string.
    std::string mytoken;
    std::ifstream infile("token.txt");
    std::getline(infile, mytoken);

    openai::configure(mytoken); // Configure and launch the OpenAI instance
    
    {
        auto image = openai::image().create(R"({
            "prompt": "A cute baby sea otter",
            "n": 2,
            "size": "1024x1024"
        })"_json);
        std::cout << image.dump(2) << '\n'; 
    }
}
