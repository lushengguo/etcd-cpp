#include <boost/program_options.hpp>
#include <rpc/server.h>
#include <spdlog/spdlog.h>

#include "utils.h"
#include "raft/node.h"

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

void bind_method(rpc::server &server, raft_node &raft)
{
    server.bind("get", [&raft](const std::string &key) { return raft.get(key); });
    server.bind("put", [&raft](const std::string &key, const std::string &value) { return raft.put(key, value); });
}

int main(int argc, char **argv)
{
    init_spdlog();

    try
    {
        uint16_t port;
        std::string cluster_config;
        size_t raft_id;

        // clang-format off
        po::options_description desc("Allowed options");
        desc.add_options()
            ("help", "produce help message")
            ("port,p", po::value<uint16_t>(&port), "port to listen on")
            ("cluster_config,c", po::value<std::string>(&cluster_config), "etcd's cluster configuration file path")
            ("current_service_id,i", po::value<size_t>(&raft_id), "current service id in cluster");
        // clang-format on
        po::variables_map vm;
        po::store(po::parse_command_line(argc, argv, desc), vm);
        if (vm.count("help"))
        {
            SPDLOG_INFO("{}", desc);
            return 0;
        }
        po::notify(vm);
        if (!vm.count("port") || !vm.count("cluster_config"))
        {
            SPDLOG_ERROR("Error: --port and --cluster_config are required options.\n");
            SPDLOG_ERROR("{}", desc);
            return 1;
        }

        rpc::server server(vm["port"].as<uint16_t>());
        raft_node raft;
        if (not raft.background_run(cluster_config, raft_id))
        {
            SPDLOG_ERROR("failed to background_run raft service");
            return 1;
        }

        bind_method(server, raft);
        server.run();
    }
    catch (std::exception &e)
    {
        SPDLOG_ERROR("error: {}", e.what());
        return 1;
    }
}