#pragma once
#include <string>
#include <vector>

struct entry
{
    uint64_t term;
    uint64_t index;
    std::string command;

    MSGPACK_DEFINE(term, index, command);
};

struct append_entries_request
{
    uint64_t term;
    uint64_t leader_id;
    uint64_t prev_log_index;
    uint64_t prev_log_term;
    std::vector<entry> entries;
    uint64_t leader_commit;

    MSGPACK_DEFINE(term, leader_id, prev_log_index, prev_log_term, entries, leader_commit);
};
struct append_entries_response
{
    uint64_t term;
    bool success;

    MSGPACK_DEFINE(term, success);
};

struct request_vote_request
{
    uint64_t term;
    uint64_t candidate_id;
    uint64_t last_log_index;
    uint64_t last_log_term;

    MSGPACK_DEFINE(term, candidate_id, last_log_index, last_log_term);
};

struct request_vote_response
{
    uint64_t term;
    bool vote_granted;

    MSGPACK_DEFINE(term, vote_granted);
};