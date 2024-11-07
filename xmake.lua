set_languages "c++20"
set_toolchains("clang")
add_rules("mode.debug", "mode.release")
add_requireconfs("*", { debug = is_mode("debug") })

add_requires("conan::boost/1.86.0", { alias = "boost" })
add_defines("BOOST_ASIO_HAS_CO_AWAIT", "BOOST_ASIO_HAS_STD_COROUTINE")

add_requires("conan::spdlog/1.14.1", { alias = "spdlog", configs = { header_only = true } })
add_requires("conan::rpclib/2.3.0", { alias = "rpclib" })
add_packages("spdlog", "boost", "rpclib")

add_includedirs("etcd")

target "etcd_cpp_server"
    set_kind("binary")
    add_files("etcd/*.cpp", "etcd/**/*.cpp")
    remove_files("etcd/etcd_client.cpp")

target "etcd_cpp_client"
    set_kind("binary")
    add_files("etcd/*.cpp", "etcd/**/*.cpp")
    remove_files("etcd/etcd_server.cpp")

after_build(function (target)
    import("core.project.task")
    task.run("generate_compile_commands")
end)

task("generate_compile_commands")
on_run(function ()
    import("core.project.project")
    import("core.base.task")
    task.run("project", {kind = "compile_commands"})
end)
