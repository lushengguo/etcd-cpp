set_languages "c++20"

add_rules("mode.debug", "mode.release")

add_requireconfs("*", { debug = is_mode("debug") })

add_requires("protobuf-cpp")
add_requires("conan::grpc/1.67.1", { alias = "grpc" })
add_requires("conan::spdlog/1.14.1", { alias = "spdlog", configs = { header_only = true } })
add_requires("conan::boost/1.86.0", { alias = "boost" })
add_packages("spdlog", "boost", "grpc", "protobuf-cpp")

add_defines("BOOST_ASIO_HAS_CO_AWAIT", "BOOST_ASIO_HAS_STD_COROUTINE")

set_toolchains("clang")
add_cxflags("-v")
add_ldflags("-v")

add_rules("protobuf.cpp")

target "etcd_cpp_server"
    set_kind("binary")
    add_files("etcd/*.cpp", "etcd/**/*.cc")
    remove_files("etcd/etcd_client.cpp")
    add_files("etcd/proto/*.proto", {proto_rootdir = "etcd", proto_grpc_cpp_plugin = true})

target "etcd_cpp_client"
    set_kind("binary")
    add_files("etcd/*.cpp", "etcd/**/*.cc")
    remove_files("etcd/etcd_server.cpp")