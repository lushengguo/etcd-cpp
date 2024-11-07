#pragma once

#include <string>
#include <unordered_map>
#include <optional>
#include <memory>
#include <rpc/client.h>

#include "raft/rpc_def.h"

class raft_node : std::enable_shared_from_this<raft_node>
{
  public:
    bool background_run(const std::string cluster_json_configuration, size_t current_node_id);

  public:
    std::optional<std::string> get(const std::string &key) const;
    bool put(const std::string &key, const std::string &value);

  private:
    // serve
    append_entries_response append_entries(const append_entries_request &request);
    request_vote_response request_vote(const request_vote_request &request);

    // request to other nodes
    append_entries_response append_entries(const append_entries_request &request, size_t node_id);
    request_vote_response request_vote(const request_vote_request &request, size_t node_id);

  private:
    std::unordered_map<std::string, std::string> __data;
    std::unordered_map<size_t, std::shared_ptr<rpc::client>> __clients;

    std::string __ip;
    uint16_t __port = 0;
    size_t __id = 0;
};