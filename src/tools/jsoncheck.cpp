#include "core/json.hpp"

#include <exception>
#include <iostream>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

int main(int argc, char* argv[])
{
    int to_return = 0;
    core::logging_init(false);
    try
    {
        std::vector<std::string> files;
        bool update_file = false;

        char c;
        while ((c = getopt(argc, argv, "huf:")) != -1)
            switch (c)
            {
            case 'h':
                std::cout << "Printing help" << std::endl;
                return 0;
                break;
            case 'u':
                update_file = true;
                break;
            case 'f':
                files.push_back(std::string(optarg));
                break;
            case '?':
                if (optopt == 'f')
                    fprintf(
                        stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint(optopt))
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf(
                        stderr, "Unknown option character `\\x%x'.\n", optopt);
                return 1;
            default:
                abort();
            }

        for (auto file : files)
        {
            std::cout << "Parsing file " << file << std::endl;
            core::JsonFileReader reader(file);
            if (update_file)
            {
                reader.write_file(file);
                std::cout << "Updating file " << file << std::endl;
            }
        }
        return 0;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exiting with exception:" << std::endl
                  << "\t" << e.what() << std::endl;
        to_return = 1;
    }
    return to_return;
}
