#pragma once

#include <boost/asio.hpp>

void init_spdlog();

std::shared_ptr<boost::asio::io_context> get_io_context();