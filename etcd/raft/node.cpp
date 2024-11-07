#include "raft/node.h"
#include "raft/rpc_def.h"
#include "spdlog/spdlog.h"
#include "utils.h"

#include <optional>
#include <rpc/server.h>
#include <boost/json.hpp>
#include <boost/asio.hpp>

bool raft_node::background_run(const std::string cluster_json_configuration, size_t current_node_id)
{
    try
    {
        boost::json::value o = boost::json::parse(cluster_json_configuration);
        auto &array = o.as_array();
        bool found_myself = true;

        for (auto &element : array)
        {
            auto &obj = element.as_object();
            size_t id = obj["id"].to_number<size_t>();
            std::string ip = obj["ip"].as_string().c_str();
            uint16_t port = obj["port"].to_number<uint16_t>();
            if (id == current_node_id)
            {
                found_myself = false;
                __id = current_node_id;
                __ip = ip;
                __port = port;
                SPDLOG_INFO("current node id:{} ip:{} port:{}", id, ip, port);
            }
            else
            {
                __clients[id] = std::make_shared<rpc::client>(ip, port);
                SPDLOG_INFO("peer node id:{} ip:{} port:{}", id, ip, port);
            }
        }

        if (found_myself)
        {
            SPDLOG_ERROR("cannot find current node id:{} in cluster configuration", current_node_id);
            return false;
        }

        auto io_context = get_io_context();
        boost::asio::post(*io_context, [this]() {
            rpc::server server(__port);
            server.bind("append_entries",
                        [this](const append_entries_request &request) { return this->append_entries(request); });
            server.bind("request_vote",
                        [this](const request_vote_request &request) { return this->request_vote(request); });
            server.run();
        });

        return true;
    }
    catch (const std::exception &e)
    {
        SPDLOG_ERROR("{}", e.what());
        return false;
    }
}

std::optional<std::string> raft_node::get(const std::string &key) const
{
    return __data.at(key);
}

bool raft_node::put(const std::string &key, const std::string &value)
{
    return false;
}

append_entries_response raft_node::append_entries(const append_entries_request &request)
{
    return append_entries_response{};
}

request_vote_response raft_node::request_vote(const request_vote_request &request)
{
    return request_vote_response{};
}

append_entries_response raft_node::append_entries(const append_entries_request &request, size_t node_id)
{
    return append_entries_response{};
}

request_vote_response raft_node::request_vote(const request_vote_request &request, size_t node_id)
{
    return request_vote_response{};
}
