#include <boost/program_options.hpp>
#include <rpc/client.h>
#include <spdlog/spdlog.h>

#include "utils.h"

namespace po = boost::program_options;

template <>
struct fmt::formatter<po::options_description> : fmt::formatter<std::string>
{
    template <typename FormatContext>
    auto format(const po::options_description &desc, FormatContext &ctx)
    {
        std::ostringstream oss;
        oss << desc;
        return fmt::formatter<std::string>::format(oss.str(), ctx);
    }
};

int main(int argc, char **argv)
{
    init_spdlog();

    std::string server_ip;
    uint16_t server_port;
    try
    {
        // clang-format off
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("server_ip,i", po::value<std::string>(&server_ip), "server address")
            ("server_port,p", po::value<uint16_t>(&server_port), "server port");
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help"))
        {
            SPDLOG_INFO("{}", desc);
            return 0;
        }
        po::notify(vm);
        if (not vm.count("server_ip") or not vm.count("server_port"))
        {
            SPDLOG_ERROR("Error: --server_ip and --server_port are required options.\n");
            SPDLOG_ERROR("{}", desc);
            return 1;
        }

        rpc::client client(server_ip, server_port);
        auto result = client.call("add", 2, 3).as<int>();
        SPDLOG_INFO("The result is:{} ", result);
        return 0;
    }
    catch (const std::exception &e)
    {
        SPDLOG_ERROR("{}", e.what());
        return 1;
    }
}