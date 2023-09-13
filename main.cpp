#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <functional>
#include <fstream>
#include <exception>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;

std::mutex m;
const static char separator = ';';
static std::exception_ptr globalExceptionPtr = nullptr;

void usage();
std::vector<std::string>& get_all_file_paths(const std::string dir_path);
std::function<void(const std::string&, const std::string&)> routine = [](const std::string& file_path,
                                                                const std::string& output_file_path)
{
    try {
        std::ifstream input_file(file_path);
        std::vector<std::string> strings;
        std::string s;
        while (std::getline(input_file, s, separator))
        {
            strings.push_back(s + "\n");
        }
        input_file.close();

        std::lock_guard<std::mutex> guard(m);

        std::ofstream output_file(output_file_path, std::ios::app);
        output_file << "[" << file_path << "]" << std::endl;

        for (size_t i = 0; i < strings.size(); ++i)
        {
            output_file << strings.at(i);
        }

        // std::cout << "From Thread ID : "<< std::this_thread::get_id() << std::endl;
        // std::cout << "path : "<< file_path << std::endl;

    } catch (...) {
        globalExceptionPtr = std::current_exception();
    }
};

int main(int argc, char** argv)
{
    if (argc < 4)
    {
        usage();
        return -1;
    }

    std::string directory_path = argv[1];
    std::string result_file_name = argv[2];
    size_t threads_nb = atoi(argv[3]);

    if (threads_nb < 1)
    {
        std::cout << "Number of threads must be >= 1" << std::endl;
        return -1;
    }
    else if (threads_nb > std::thread::hardware_concurrency())
    {
        threads_nb = std::thread::hardware_concurrency();
    }

    std::remove(result_file_name.c_str());
    std::vector<std::thread> threads_vector(threads_nb);
    auto all_files = get_all_file_paths(directory_path);

    size_t i = 0;
    for (const auto& it : all_files)
    {
        if (i == threads_vector.size())
        {
            for (auto& it : threads_vector)
            {
                if (it.joinable())
                {
                    it.join();
                }
            }
            i = 0;
        }
        threads_vector.at(i++) = std::thread(routine, it, result_file_name);
    }

    for (auto& it : threads_vector)
    {
        if (it.joinable())
        {
            it.join();
        }
    }

    if (globalExceptionPtr)
     {
        try
        {
            std::rethrow_exception(globalExceptionPtr);
        }
        catch (const std::exception &ex)
        {
            std::cout << "Thread exited with exception: " << ex.what() << "\n";
        }
     }

    return 0;
}

std::vector<std::string>& get_all_file_paths(const std::string dir_path)
{
    fs::path path_to_show = dir_path;
    thread_local std::vector<std::string> vec;

    if (fs::is_directory(path_to_show))
    {
        for (const auto& it : fs::directory_iterator(path_to_show))
        {
            vec.push_back(it.path());
        }
    }
    return vec;
}

void usage()
{
    std::cout << "Usage: NTO-9-test <path to files directory> "
                 "<result file name> "
                 "<number of threads>" << std::endl;
}
