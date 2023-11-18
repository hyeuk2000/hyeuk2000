#include "openai.hpp"
#include <iostream>
#include <string>

class query{
    public:
        query(std::string string){
            openai::start();

            openai::_detail::Json str = {
                {"model", "gpt-3.5-turbo"},
                {"messages", {{{"role","user"},{"content",string}}}},
                {"max_tokens", 100},
                {"temperature", 0}
            };
            auto chat = openai::chat().create(str); // Using user-defined (raw) string literals
            output = chat.dump(2);
        }
        std::string get_output(){
            return output;
        }
    private:
        std::string output;
};

int main() {
    std::string temp;
    std::cin >> temp;
    query q(temp);
    std::cout << q.get_output() << "\n";
    return 0;
}
