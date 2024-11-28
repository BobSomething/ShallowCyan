#include "piece.hpp"
#include "board.hpp"

int main(int argc, char* argv[]) {
    for(int i = 0; i < argc; i++){
        std::cout << "Displaying argument #" 
        << std::to_string(i)
        << ": `"
        << std::string(argv[i])
        << "'"
        << std::endl;
    }
    return 0;
}
